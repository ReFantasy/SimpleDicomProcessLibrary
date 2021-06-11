#include <iostream>
#include <windows.h>
#include "global.h"
#include "core.h"

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmjpeg/djdecode.h"
#include "dcmtk/dcmdata/dcmetinf.h"
#include "dcmtk/ofstd/ofstd.h"

#include <thread>
#include <vector>

using namespace std;



int main()
{
	DicomSeries series;
	//auto b = series.ReadDir("/Users/refantasy/Desktop/CT1001051/401_351");
	auto b = series.ReadDir("C:/Users/ReFantasy/Desktop/a");
	if (!b)
	{
		cout << "read fail" << endl;
		return 0;
	}
	cout << "read files success: file count  " << series.GetTotalFrames() << endl;

	series.SetWindow(-500, 10);

	auto df = series.GetDicom(100);
	auto data = df->GetOutputData(0);
	WriteToPPM("test.ppm", df->GetWidth(), df->GetHeight(), data);


	//////////////////////////////////////////////////////////////////////////

	void* pdata = NULL;
	auto imgsize = df->GetDicomImage()->createWindowsDIB(pdata, 512 * 512, 0, 8, 1, 0);
	cout << imgsize << endl;

	auto bmp = CreateBitmap(df->GetDicomImage()->getWidth(), df->GetDicomImage()->getHeight(), imgsize, pdata);
	FILE* fp = fopen("1.bmp", "wb");
	if (fp)
	{
		fwrite(bmp.data(), sizeof(char), bmp.size(), fp);
		fclose(fp);
	}

	//////////////////////////////////////////////////////////////////////////

	std::shared_ptr<Window> pwin = std::make_shared<Window>();
	auto f = [](std::shared_ptr<Window> pwin) {
		pwin->wWinMain(pwin->hInstance);
	};

	std::thread t(f, pwin);

	char* name = "hello";
	while (1)
	{
		SendMessage(pwin->hWnd, WM_PAINT, (WPARAM)(name), (LPARAM)(name));
	}
}