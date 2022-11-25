#pragma once

#ifdef NANGLEDLL_EXPORTS
#define NANGLEDLL_API __declspec(dllexport)
#else
#define NANGLEDLL_API __declspec(dllimport)
#endif

extern "C" NANGLEDLL_API void draw(HDC hdc, int x, int y, int radius);
