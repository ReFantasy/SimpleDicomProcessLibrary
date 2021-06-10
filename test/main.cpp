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
	Dicom dicom;
	auto cond = dicom.LoadFile("C:/Users/ReFantasy/Desktop/ser007img00001.DCM");
	if (!cond)
		return 0;

	std::cout << "rows: " << dicom.GetHeight()<< std::endl;
	std::cout << "frames: " << dicom.GetNumberOfFrames() << std::endl;
	////std::cout << "DCM_BitsAllocated: " << dicom.BitsAllocatedOfPerPixel() << std::endl;
	////std::cout << "DCM_BitsStored: " << dicom.BitsAllocatedOfPerPixel() << std::endl;

	//std::string sp;
	//dicom.FindAndGetOFString(DCM_PixelSpacing, sp);
	//std::cout << "Pixel Spacing: " << sp << std::endl;

	
	//std::cout << "PixelSpacing: " << dicom.GetPixelSpacing() << std::endl;

	dicom.SetWindow(-500, 10);

	const unsigned char* data = nullptr;
	data = dicom.GetOutputData(0);
	//cout << res << endl;
	printf("addr:%x\n", data);
	std::cout << "WriteToPPM" << std::endl;
	WriteToPPM("a.ppm",  dicom.GetWidth(),dicom.GetHeight(), data);

	//free(data);

	//WriteToPPM("test.ppm", 768, 768, (const unsigned char*)dicom.GetRawImageData());

}