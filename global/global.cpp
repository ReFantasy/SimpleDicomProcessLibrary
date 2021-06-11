#include "global.h"
#include <fstream>
#include <iostream>

#define PPMTYPE "P3"
#define MAXPIXELVALUE "255"


void WriteToPPM(std::string filename, unsigned int image_width, unsigned int image_height, const unsigned char* image_data)
{
	std::fstream io(filename, std::ios::out);

	io << PPMTYPE << "\n"
		<< image_width << char(0x20) << image_height
		<< "\n"
		<< MAXPIXELVALUE
		<< "\n";

	for (int j = 0; j < image_height; ++j)
	{
		for (int i = 0; i < image_width; ++i)
		{

			int ir = static_cast<int>(image_data[j * image_width + i]);
			int ig = static_cast<int>(image_data[j * image_width + i]);
			int ib = static_cast<int>(image_data[j * image_width + i]);

			io << ir << char(0x20) << ig << char(0x20) << ib << '\n';
		}
	}

	io.close();
}


int StringToInt(std::string str)
{
	return std::atoi(str.c_str());
}

double StringToDouble(std::string str)
{
	return std::atof(str.c_str());
}

#ifdef _WIN32
#pragma comment(lib, "gdiplus.lib") 
#include <gdiplusheaders.h>
void* g_dcm_data;

std::vector<char> CreateBitmap(int width, int height, size_t imgsize, void* pdata)
{
	static BITMAPFILEHEADER targetfileheader;
	static BITMAPINFOHEADER targetinfoheader;
	memset(&targetfileheader, 0, sizeof(BITMAPFILEHEADER));
	memset(&targetinfoheader, 0, sizeof(BITMAPINFOHEADER));

	//构造灰度图的文件头
	targetfileheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;
	targetfileheader.bfSize = width * height + sizeof(RGBQUAD) * 256 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	targetfileheader.bfReserved1 = 0;
	targetfileheader.bfReserved2 = 0;
	targetfileheader.bfType = 0x4d42;

	//构造灰度图的信息头
	targetinfoheader.biBitCount = 8;
	targetinfoheader.biSize = sizeof(BITMAPINFOHEADER);
	targetinfoheader.biHeight = height;
	targetinfoheader.biWidth = width;
	targetinfoheader.biPlanes = 1;
	targetinfoheader.biCompression = BI_RGB;
	targetinfoheader.biSizeImage = height * width;
	targetinfoheader.biXPelsPerMeter = 0;
	targetinfoheader.biYPelsPerMeter = 0;
	targetinfoheader.biClrImportant = 0;
	targetinfoheader.biClrUsed = 0;

	//构造灰度图的调色版
	RGBQUAD rgbquad[256];
	for (int i = 0; i < 256; i++)
	{
		rgbquad[i].rgbBlue = i;
		rgbquad[i].rgbGreen = i;
		rgbquad[i].rgbRed = i;
		rgbquad[i].rgbReserved = 0;
	}

	int headsize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;
	int size = headsize + imgsize;

	std::vector<char> vec_char(size);
	char* bmp = vec_char.data();
	memset(bmp, 0, size);
	memcpy(bmp, &targetfileheader, sizeof(BITMAPFILEHEADER));
	memcpy(&bmp[sizeof(BITMAPFILEHEADER)], &targetinfoheader, sizeof(BITMAPINFOHEADER));
	memcpy(&bmp[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)], rgbquad, sizeof(RGBQUAD) * 256);
	memcpy(&bmp[headsize], pdata, imgsize);

	return vec_char;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char* p = (char*)wParam;
	std::cout << p << std::endl;

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);//可以使GetMessage返回0
		break;
	case WM_PAINT:
		std::cout << "start to draw dicom image" << std::endl;
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: 在此处添加使用 hdc 的任何绘图代码...
			 
			CImage c;
			c.Load()
			EndPaint(hWnd, &ps);

		}
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL Register(LPSTR lpClassName, WNDPROC wndProc, HINSTANCE g_hInstance)
{
	WNDCLASSEX wce = { 0 };
	wce.cbSize = sizeof(wce);
	wce.cbClsExtra = 0;
	wce.cbWndExtra = 0;
	wce.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wce.hCursor = NULL;
	wce.hIcon = NULL;
	wce.hIconSm = NULL;
	wce.hInstance = g_hInstance;
	wce.lpfnWndProc = wndProc;
	wce.lpszClassName = lpClassName;
	wce.lpszMenuName = NULL;
	wce.style = CS_HREDRAW | CS_VREDRAW;
	ATOM nAtom = RegisterClassEx(&wce);
	if (nAtom == 0)
		return FALSE;
	return true;
}

HWND CreateMain(LPSTR lpClassName, LPSTR lpWndName, HINSTANCE g_hInstance)
{
	const int wind_width = 512;
	const int wind_height = 512;
	HWND hWnd = CreateWindowEx(0, lpClassName, lpWndName,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, wind_width, wind_height, NULL, NULL, g_hInstance, NULL);
	return hWnd;
}
void Display(HWND hWnd)
{
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
}


void Message()
{
	MSG nMsg = { 0 };
	while (GetMessage(&nMsg, NULL, 0, 0))
	{
		TranslateMessage(&nMsg);
		DispatchMessage(&nMsg);
	}

}

int APIENTRY Window::wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	BOOL nRet = Register("Main", WndProc, hInstance);
	if (!nRet)
	{
		MessageBox(NULL, "注册失败", "Infor", MB_OK);
		return 0;
	}
	hWnd = CreateMain("Main", "window", hInstance);
	Display(hWnd);
	Message();
	return 0;

}
#endif
