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