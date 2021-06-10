#include "core.h"
#include "global.h"

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmjpeg/djdecode.h"
#include "dcmtk/dcmdata/dcmetinf.h"
#include "dcmtk/ofstd/ofstd.h"


Dicom::Dicom(std::string filename)
{
	OFCondition status =  df->loadFile(filename.c_str());
	if (!status.good())
	{
		df = std::make_shared<DcmFileFormat>();
	}
	else
	{
		di = std::make_shared<DicomImage>(df->getDataset(), df->getDataset()->getCurrentXfer());
	}
}

bool Dicom::LoadFile(std::string filename)
{
	auto tmp_df = std::make_shared<DcmFileFormat>();

	OFCondition status = tmp_df->loadFile(filename.c_str());
	if (status.good())
	{
		df = tmp_df;
		di = std::make_shared<DicomImage>(tmp_df->getDataset(), tmp_df->getDataset()->getCurrentXfer());
		return true;
	}
	return false;
}

int Dicom::GetWidth() const
{
	return di->getWidth();
}

int Dicom::GetHeight() const
{
	return di->getHeight();
}

bool Dicom::GetWindow(double& win_center, double& win_width) const
{
	return di->getWindow(win_center, win_width);
}

bool Dicom::SetWindow(double win_center, double win_width)
{
	return di->setWindow(win_center, win_width);
}

int Dicom::GetNumberOfFrames() const
{
	return di->getNumberOfFrames();
}

const unsigned char* Dicom::GetOutputData(int frame)const
{
	int frames = GetNumberOfFrames();
	if (frame >= frames)
		return nullptr;

	return (const unsigned char*)di->getOutputData(8, frame);
}
