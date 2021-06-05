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
	//auto cond = dicom.LoadFile("C:/Users/ReFantasy/Desktop/ser007img00001.DCM");
	auto cond = dicom.LoadFile("\\\\WIN-5307K1TI0MH\\Storage\\1316253\\1.3.12.2.1107.5.4.5.35175.30000020072604323270300000963.512XA_5f1f75665aef9954.dcm");
	if (cond)
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
	data = dicom.Get8BitsImage(25);
	std::cout << "WriteToPPM" << std::endl;
	WriteToPPM("test.ppm", dicom.GetImageHeight(), dicom.GetImageWidth(), (const unsigned char*)(data));

	free(data);

	//WriteToPPM("test.ppm", 768, 768, (const unsigned char*)dicom.GetRawImageData());

}