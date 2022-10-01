#include <windows.h>
#include <windowsx.h>
#include <winnt.h>

#pragma comment(lib, "Msimg32.lib")

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void paintWind(HWND hwnd);

void mvrght(HWND hwnd);

void mvlft(HWND hwnd);

void mvup(HWND hwnd);

void mvdwn(HWND hwnd);

void mvPctr(HWND hwnd);



HINSTANCE h;

CONST INT TRNSPCLR = 0xFFFFFF, ELA = 100, ELB = 100;

int x = 0, y = 0, dx = 10, dy = 10, size_x = 100, size_y = 100, col = 60, mdx = 10, sgndx = 1, sgndy = 1, kol = 0, x2, y2;

HBITMAP gBM;

boolean onPict = false, onMove = false, mvx = false, mvy = false;



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)

{
	h = hInstance;
	const wchar_t CLASS_NAME[] = L"Sample Window Class";
	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hbrBackground = (HBRUSH)(3);

	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(
		0,                             
		CLASS_NAME,                    
		L"Figure",				
		WS_OVERLAPPEDWINDOW,            
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, // Size and position
		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// Run the message loop.
	MSG msg = { };

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	switch (uMsg)
	{
	case WM_CLOSE:

	{
		if (MessageBox(hwnd, L"Already Done?", L"Figure", MB_OKCANCEL) == IDOK)
		{
			DestroyWindow(hwnd);
		}

		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_PAINT:
	{
		paintWind(hwnd);
		return 0;
	}
	case WM_CREATE:
	{
		HBITMAP hBmp = (HBITMAP)LoadImage(NULL, L"Z:/OSISP/lr_1/linux.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		gBM = hBmp;
		break;
	}
	case WM_ERASEBKGND:
	{
		return 1;
	}
	case WM_MOUSEMOVE:
	{
		if (onMove)
			return 0;

		x = GET_X_LPARAM(lParam) - (size_x) / 2;
		y = GET_Y_LPARAM(lParam) - (size_y) / 2;

		x2 = x + size_x;
		y2 = y + size_y;

		if (x < 0) x = 0;
		if (y < 0) y = 0;
		if (x2 > clientRect.right) x = clientRect.right - size_x;
		if (y2 > clientRect.bottom) y = clientRect.bottom - size_y;

		InvalidateRect(hwnd, &clientRect, true);

		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		if (onMove)
			return 0;

		int key = GET_KEYSTATE_WPARAM(wParam);
		if (key == MK_SHIFT)
		{
			mvx = true;
			mvy = false;

			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
				sgndx = -1;
			else
				sgndx = 1;
		}

		else
		{
			mvy = true;
			mvx = false;

			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
				sgndy = -1;
			else
				sgndy = 1;
		}

		mvPctr(hwnd);
		mvx = false;
		mvy = false;

		return 0;
	}
	case WM_KEYDOWN:
	{
		if (wParam != VK_SPACE)
			if (onMove)
				return 0;

		switch (wParam)
		{
		case VK_RIGHT:
			mvrght(hwnd);
			break;
		case VK_LEFT:
			mvlft(hwnd);
			break;
		case VK_UP:
			mvup(hwnd);
			break;
		case VK_DOWN:
			mvdwn(hwnd);
			break;
		case VK_SPACE:
			if (!onPict)
			{
				onPict = true;
				BITMAP bm;
				GetObject(gBM, sizeof(bm), &bm);
				size_x = bm.bmWidth;
				size_y = bm.bmHeight;

				InvalidateRect(hwnd, &clientRect, true);
				mvrght(hwnd);
				mvdwn(hwnd);
			}

			else
			{
				onPict = false;
				size_x = ELA;
				size_y = ELB;
			}

			break;
		}
		break;

		return 0;
	}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


void mvrght(HWND hwnd)
{
	x += mdx;
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	if (x + size_x > clientRect.right)
	{
		sgndx *= -1;
		x = clientRect.right - size_x;
	}

	InvalidateRect(hwnd, &clientRect, true);
	return;
}



void mvdwn(HWND hwnd)
{
	y += mdx;
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	if (y + size_y > clientRect.bottom)
	{
		y = clientRect.bottom - size_y;
		sgndy *= -1;
	}

	InvalidateRect(hwnd, &clientRect, true);
	return;
}



void mvlft(HWND hwnd)
{
	x -= mdx;
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	if (x < 0)
	{
		sgndx *= -1;
		x = 0;
	}

	InvalidateRect(hwnd, &clientRect, true);
	return;
}



void mvup(HWND hwnd)
{
	y -= mdx;
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	if (y < 0)
	{
		sgndy *= -1;
		y = 0;
	}

	InvalidateRect(hwnd, &clientRect, true);
	return;
}



void mvPctr(HWND hwnd)
{
	if (mvx)
		if (sgndx == 1)
			mvrght(hwnd);
		else
			mvlft(hwnd);
	else
		if (sgndy == 1)
			mvdwn(hwnd);
		else
			mvup(hwnd);
}

void paintWind(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	HBITMAP hbmBack = CreateCompatibleBitmap(hdc, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
	HDC backDc = CreateCompatibleDC(hdc);
	HGDIOBJ previousBackBmp = SelectObject(backDc, hbmBack);
	FillRect(backDc, &clientRect, (HBRUSH)(4));

	if (onPict == false)
	{
		HBRUSH hBrush;
		hBrush = CreateSolidBrush(RGB(0, col, 0));
		HGDIOBJ oldBrush = SelectObject(backDc, hBrush);
		Ellipse(backDc, x, y, x + size_x, y + size_y);
		SelectObject(backDc, oldBrush);
	}

	else
	{
		HDC pict = CreateCompatibleDC(hdc);
		HGDIOBJ previousBmp = SelectObject(pict, gBM);
		BITMAP bm;
		GetObject(gBM, sizeof(bm), &bm);
		size_x = bm.bmWidth;
		size_y = bm.bmHeight;

		TransparentBlt(backDc, x, y, bm.bmWidth, bm.bmHeight, pict, 0, 0, bm.bmWidth, bm.bmHeight, TRNSPCLR);
		SelectObject(pict, previousBmp);		
		ReleaseDC(hwnd, pict);
	}

	SetBkColor(backDc, RGB(0x64, 0x95, 0xED));
	BitBlt(hdc, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, backDc, 0, 0, SRCCOPY);
	SelectObject(backDc, previousBackBmp);
	DeleteDC(backDc);
	EndPaint(hwnd, &ps);
}