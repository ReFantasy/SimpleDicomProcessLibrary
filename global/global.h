#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <string>
#include <vector>

/**
 * @brief Write 8bit image to a ppm file.
 * @param filename
 * @param image_width
 * @param image_height
 * @param image_data
*/
void WriteToPPM(std::string filename, unsigned int image_width, unsigned int image_height, const unsigned char* image_data);


int StringToInt(std::string str);
double StringToDouble(std::string str);


#ifdef _WIN32
#include <windows.h>
#include <gdiplus.h>
#include <gdiplusheaders.h>
#include <gdiplusbase.h>
#include <gdiplusbitmap.h>
#include <atlimage.h>

extern void* g_bitmap;

std::vector<char> CreateBitmap(int width, int height, size_t imgsize, void* pdata);


// 窗口处理函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//注册窗口类
BOOL Register(LPSTR lpClassName, WNDPROC wndProc, HINSTANCE g_hInstance);

//创建主窗口
HWND CreateMain(LPSTR lpClassName, LPSTR lpWndName, HINSTANCE g_hInstance);

//显示窗口
void Display(HWND hWnd);


//消息循环
void Message();

struct RAWIMAGE
{
	int width;
	int height;
	unsigned char* data;
};

class Window
{
public:
	int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
		_In_opt_ HINSTANCE hPrevInstance = HINSTANCE{},
		_In_ LPWSTR    lpCmdLine = LPWSTR{},
		_In_ int       nCmdShow = 0);

	HWND hWnd;
	HINSTANCE hInstance;
};




#endif
#endif//__GLOBAL_H__