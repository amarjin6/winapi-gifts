#include <windows.h>

#include <iostream>

#include <fstream>

#include <filesystem>

#include <string>

#include "ConcurrentList.h"



struct FileResult

{

    std::string path;

    int count;

};



VOID CALLBACK processFileCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Parameter, PTP_WORK Work);



std::string directoryPath;

std::string stringToSearch;

ConcurrentList<FileResult&>* list;



volatile long processingCount;

CONDITION_VARIABLE processingVar;

CRITICAL_SECTION processingCrit;



int main()

{

    directoryPath = "E:\\osisp_test";

    stringToSearch = "hello";



    InitializeCriticalSection(&processingCrit);

    InitializeConditionVariable(&processingVar);

    InterlockedExchange(&processingCount, 0);



    list = new ConcurrentList<FileResult&>();

    auto iter = std::filesystem::directory_iterator(directoryPath);



    for (auto& entry : iter)

    {

        if (entry.is_regular_file())

        {

            InterlockedIncrement(&processingCount);

            std::string* path = new std::string(entry.path().string());

            auto work = CreateThreadpoolWork(processFileCallback, (PVOID)(path), NULL);

            SubmitThreadpoolWork(work);

        }

    }



    EnterCriticalSection(&processingCrit);

    while (InterlockedCompareExchange(&processingCount, 0, 0) != 0)

        SleepConditionVariableCS(&processingVar, &processingCrit, INFINITE);

    LeaveCriticalSection(&processingCrit);



    auto reader = list->createReader();

    while (reader->moveNext())

    {

        auto value = reader->value();

        std::cout << value.path << " - " << value.count << "\n";

    }

}



VOID processFileCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Parameter, PTP_WORK Work)

{

    std::string path = *(std::string*)Parameter;

    std::ifstream stream;

    std::string line;



    try

    {

        stream.open(path);

        int counter = 0;



        while (std::getline(stream, line))

        {

            int startPos = 0;

            while ((startPos = line.find(stringToSearch, startPos)) != std::string::npos)

            {

                ++startPos;

                ++counter;

            }

        }



        if (counter != 0)

        {

            auto fileResult = new FileResult();

            fileResult->count = counter;

            fileResult->path = path;



            list->pushBack(*fileResult);

        }

    }

    catch (std::runtime_error)

    {

        stream.close();

        InterlockedDecrement(&processingCount);



        if (InterlockedCompareExchange(&processingCount, 0, 0) == 0)

            WakeAllConditionVariable(&processingVar);

    }



    stream.close();

    InterlockedDecrement(&processingCount);



    if (InterlockedCompareExchange(&processingCount, 0, 0) == 0)

        WakeAllConditionVariable(&processingVar);

}