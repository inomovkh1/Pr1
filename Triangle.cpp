#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#include <GdiPlus.h>
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

static TCHAR szWindowClass[] = _T("TriApp");


static TCHAR szTitle[] = _T("Triangle");

HINSTANCE hInst;


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);





int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Colored Figure Application"),
            NULL);

        return 1;
    }


    hInst = hInstance;


    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Colored Figure Application"),
            NULL);

        return 1;
    }

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);


    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

VOID OnPaint(HDC hdc, RECT rect)
{
    Gdiplus::Color colorMain(0xFF, 0xB2, 0x00, 0xFF);
    Gdiplus::Color colorTransparent(0x00, 0x00, 0x00, 0x00);
    Bitmap bmp(L"image.bmp");
    Graphics graphics(hdc);
    Gdiplus::Color pixelColor;
    RECT square;



    square.left = bmp.GetWidth();
    square.top = bmp.GetHeight();
    square.right = 0;
    square.bottom = 0;


    for (int x = 0; x < bmp.GetWidth(); x++)
    {
        for (int y = 0; y < bmp.GetHeight(); y++)
        {
            bmp.GetPixel(x, y, &pixelColor);
            if (pixelColor.GetValue() == colorMain.GetValue())
            {
                if (square.left > x) square.left = x;
                if (square.top > y) square.top = y;
                if (square.right < x) square.right = x;
                if (square.bottom < y) square.bottom = y;
            }
            else
            {
                bmp.SetPixel(x, y, colorTransparent);
            }
        }

    }

    LONG width = square.right - square.left;
    LONG height = square.bottom - square.top;
   
    graphics.DrawImage(& bmp, (INT)((rect.right - rect.left - width) / 2), (rect.bottom - rect.top - height) / 2, square.left, square.top, width, height, UnitPixel);
}


HBITMAP hBitmap = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    TCHAR greeting[] = _T("Triangle");



    switch (message)
    {
    case WM_CREATE:


        break;

    case WM_PAINT:

        PAINTSTRUCT     ps;
        HDC             hdc;

        RECT rect;
        GetClientRect(hWnd, &rect);

        hdc = BeginPaint(hWnd, &ps);

        OnPaint(hdc, rect);

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 1;
}