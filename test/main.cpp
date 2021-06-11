#include <iostream>
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



using namespace std;
int main()
{
	DicomSeries series;
	//auto b = series.ReadDir("/Users/refantasy/Desktop/CT1001051/401_351");
	auto b = series.ReadDir("C:/Users/ReFantasy/Desktop/CT1008803/201_384");
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

}
//#include <iostream>
//#include <thread>
//#include <chrono>
//#include <vector>
//
//using namespace std;
//static const int MAX = 10e8; // ①
//static double sum = 0; // ②
//
//
//
//void concurrent_task(int min, int max)
//{
//	auto worker = [](std::shared_ptr<DicomFile> df, std::string filename)
//	{
//
//	};
//
//	vector<thread> threads;
//	std::vector<std::shared_ptr<DicomFile>> dfs;
//
//	for (int t = 0; t < 10; t++)
//	{ 
//
//		auto df = std::make_shared<DicomFile>();
//		dfs.push_back(df);
//		threads.push_back(thread(worker,df, "1")); // ③
//
//	}
//	for (auto& t : threads)
//	{
//		t.join(); 
//	}
//
//}
//
//int main()
//{
//	concurrent_task(0, 10000000);
//}