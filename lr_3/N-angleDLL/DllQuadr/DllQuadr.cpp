// MathLibrary.cpp : Defines the exported functions for the DLL.
#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>
#include "DllQuadr.h"
#include <windows.h>
#include <wingdi.h>
#pragma comment(lib, "Msimg32.lib")

#define pi 3.141593

#define n 4


void draw(HDC hdc, int x, int y, int radius)
{
	HBRUSH hBrush;
	hBrush = CreateSolidBrush(RGB(0, 0, 100));
	HGDIOBJ oldBrush = SelectObject(hdc, hBrush);

	float u = 2 * pi / n;
	POINT pp[n];

	pp[0].x = x + radius / 2;
	pp[0].y = y + radius / 2;

	for (int i = 1; i < n; i++)
	{
		pp[i].x = x + radius * cos(i * u);
		pp[i].y = y + radius * sin(i * u);
	}

	Polygon(hdc, pp, n);

	SelectObject(hdc, oldBrush);
}

