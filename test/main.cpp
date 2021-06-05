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

int main()
{
	Dicom dicom;
	//if (dicom.LoadFile("‪C:/Users/ReFantasy/Desktop/CT数据/平海涛/a/ser007img00001.dcm"))
	if (dicom.LoadFile("C:/Users/ReFantasy/Desktop/test.DCM"))
	{
		std::cout << "Load success" << std::endl;
	}
	else
	{
		std::cout << "Load Fail" << std::endl;
		return 0;
	}

	std::cout << "rows: " << dicom.GetImageHeight() << std::endl;
	std::cout << "frames: " << dicom.GetNumberOfFrames() << std::endl;
	std::cout << "DCM_BitsAllocated: " << dicom.BitsAllocatedOfPerPixel() << std::endl;
	std::cout << "DCM_BitsStored: " << dicom.BitsAllocatedOfPerPixel() << std::endl;

	std::string sp;
	dicom.FindAndGetOFString(DCM_PixelSpacing, sp);
	std::cout << "Pixel Spacing: " << sp << std::endl;

	char* data;
	//dicom.SetWindow(300, 1500);
	data = dicom.GetImage8Bits();
	std::cout << "WriteToPPM" << std::endl;
	WriteToPPM("test.ppm", 512, 512, (const unsigned char*)(data));

	free(data);

	//WriteToPPM("test.ppm", 768, 768, (const unsigned char*)dicom.GetRawImageData());

}