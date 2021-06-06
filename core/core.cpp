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
#include <thread>
#include <vector>



bool Dicom::LoadFile(std::string filename)
{
	dff = std::make_shared<DcmFileFormat>();
	OFCondition cond = dff->loadFile(filename.c_str());
	if (!cond.good())
	{
		std::cout << cond.text() << std::endl;
		return false;
	}

	OFString x;
	cond = dff->getDataset()->findAndGetOFString(DCM_WindowCenter, x);
	if (cond.good())
		_window_center = StringToDouble(x.c_str());

	cond = dff->getDataset()->findAndGetOFString(DCM_WindowWidth, x);
	if (cond.good())
		_window_width = StringToDouble(x.c_str());


	return true;
}

bool Dicom::FindAndGetOFString(const DcmTagKey& tagKey, std::string& value)const
{
	OFString ofvalue;
	auto cond = dff->getDataset()->findAndGetOFString(tagKey, ofvalue);
	value = std::string(ofvalue.c_str());
	return cond.good();
}

OFCondition Dicom::FindAndGetOFString(const DcmTagKey& tagKey, OFString& value, const unsigned long pos /*= 0*/, const OFBool searchIntoSub /*= OFFalse*/)const
{
	return dff->getDataset()->findAndGetOFString(tagKey, value, pos, searchIntoSub);
}

int Dicom::GetRescaleIntercept() const
{
	std::string intercep;
	auto cond = FindAndGetOFString(DCM_RescaleIntercept, intercep);
	if (cond)
		return StringToInt(intercep);
	else
		return 0;
}

double Dicom::GetRescaleSlope() const
{
	std::string slope;
	auto cond = FindAndGetOFString(DCM_RescaleSlope, slope);
	if (cond)
		return StringToDouble(slope);
	else
		return 1;
}

int Dicom::SamplesOfPerPixel() const
{
	std::string bits;
	if (FindAndGetOFString(DCM_SamplesPerPixel, bits))
		return StringToInt(bits);
	else
		return -1;
}

int Dicom::GetImageWidth() const
{
	std::string cols;
	if (FindAndGetOFString(DCM_Columns, cols))
		return StringToInt(cols);
	else
		return -1;
}

int Dicom::GetImageHeight() const
{
	std::string rows;
	if (FindAndGetOFString(DCM_Rows, rows))
		return StringToInt(rows);
	else
		return -1;
}

int Dicom::GetNumberOfFrames() const
{
	std::string total_frames;
	if (FindAndGetOFString(DCM_NumberOfFrames, total_frames))
		return StringToInt(total_frames);
	else
		return 1;
}

int Dicom::BitsAllocatedOfPerPixel() const
{
	std::string bits;
	if (FindAndGetOFString(DCM_BitsAllocated, bits))
		return StringToInt(bits);
	else
		return -1;
}

int Dicom::BitsStoredOfPerPixel() const
{
	std::string bits;
	if (FindAndGetOFString(DCM_BitsStored, bits))
		return StringToInt(bits);
	else
		return -1;
}

const char* Dicom::GetRawImageData(int frame) const
{
	if (frame >= GetNumberOfFrames())
		return nullptr;

	unsigned int bytes_per_pixel = (BitsAllocatedOfPerPixel() <= 8 ? 1 : 2);
	unsigned long raw_data_length = GetImageWidth() * GetImageHeight() * bytes_per_pixel;// *SamplesOfPerPixel();

	DcmElement* element = NULL;
	OFCondition cond = dff->getDataset()->findAndGetElement(DCM_PixelData, element);
	if (!cond.good())
	{
		std::cout << "Get pixel data element error:" << cond.text() << std::endl;
	}
	else
	{
		unsigned short* pData = nullptr;
		auto status = element->getUint16Array(pData);

		if (!status.good())
		{
			std::cout << "Get pixel data array error:" << cond.text() << std::endl;
		}
		else
		{
			return (const char*)pData + frame * (unsigned long long)raw_data_length;
		}
	}

	return nullptr;
}

unsigned char* Dicom::Create8BitsImage(int frame) const
{
	
	unsigned int w = GetImageWidth();
	unsigned int h = GetImageHeight();
	unsigned int bytes_per_pixel = (BitsAllocatedOfPerPixel() <= 8 ? 1 : 2);


	if (frame >= GetNumberOfFrames())
		return nullptr;

	auto rowdata = GetRawImageData(frame);
	if (rowdata == nullptr)return nullptr;

    // allocate memeory
	unsigned char* data = (unsigned char*)malloc(w * h);
	memset(data, 0, w * h);

	
	
	// copy image data
	if (bytes_per_pixel == 1)
	{
		memcpy(data, rowdata, w * h);

	}
	else if (bytes_per_pixel == 2)
	{
		double slpoe = GetRescaleSlope();
		int intercept = GetRescaleIntercept();

		short win_left = _window_center - _window_width / 2;
		short win_right = _window_center + _window_width / 2;
		short* rowdata_short = (short*)rowdata;

		std::vector<std::thread> threads;

		for (int row = 0; row < h; row++)
		{
			auto process_one_row = [&](int row)
			{
				for (int col = 0; col < w; col++)
				{
					short ct_value = rowdata_short[row * w + col] * slpoe + intercept; // CT Value

					ct_value = ct_value < win_left ? win_left : (ct_value > win_right ? win_right : ct_value);
					data[row * w + col] = (ct_value - win_left) / _window_width * 255;
				}

			};

			// multithread accerlerate
			threads.push_back(std::thread(process_one_row, row));

		}

		for (int i = 0; i < threads.size(); i++)
			threads[i].join();
	}
	else
	{
		free(data);
		data = nullptr;
	}

	return data;
}

void Dicom::GetWindow(double& win_center, double& win_width) const
{
	win_center = _window_center;
	win_width = _window_width;
}

void Dicom::GetDefaultWindow(double& win_center, double& win_width) const
{
	std::string win;
	FindAndGetOFString(DCM_WindowCenter, win);
	win_center = StringToDouble(win);
	FindAndGetOFString(DCM_WindowWidth, win);
	win_width = StringToDouble(win);
}

void Dicom::SetWindow(double win_center, double win_width)
{
	_window_center = win_center;
	_window_width = win_width;
}

bool Dicom::GetPixelSpacing(double& sx, double& sy) const
{
	if (!dff)return false;

	OFString sp;
	FindAndGetOFString(DCM_PixelSpacing, sp, 0);
	sx = StringToDouble(sp.c_str());
	FindAndGetOFString(DCM_PixelSpacing, sp, 1);
	sy = StringToDouble(sp.c_str());

	return true;
}

