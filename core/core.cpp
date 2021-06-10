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


DicomFile::DicomFile(std::string filename)
{
	OFCondition status = df->loadFile(filename.c_str());
	if (!status.good())
	{
		df = std::make_shared<DcmFileFormat>();
	}
	else
	{
		di = std::make_shared<DicomImage>(df->getDataset(), df->getDataset()->getCurrentXfer());
	}
}

bool DicomFile::LoadFile(std::string filename)
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

int DicomFile::GetWidth() const
{
	return di->getWidth();
}

int DicomFile::GetHeight() const
{
	return di->getHeight();
}

bool DicomFile::GetWindow(double& win_center, double& win_width) const
{
	return di->getWindow(win_center, win_width);
}

bool DicomFile::GetDefaultWindow(double& win_center, double& win_width) const
{
	OFString c, w;
	auto res1 = df->getDataset()->findAndGetOFString(DCM_WindowCenter, c);
	auto res2 = df->getDataset()->findAndGetOFString(DCM_WindowWidth, w);
	if ((!res1.good()) || (!res2.good()))
		return false;
	win_center = std::atof(c.c_str());
	win_width = std::atof(w.c_str());

	return true;
}

bool DicomFile::SetWindow(double win_center, double win_width)
{
	return di->setWindow(win_center, win_width);
}

int DicomFile::GetNumberOfFrames() const
{
	return di->getNumberOfFrames();
}

const unsigned char* DicomFile::GetOutputData(int frame)const
{
	int frames = GetNumberOfFrames();
	if (frame >= frames)
		return nullptr;

	return (const unsigned char*)di->getOutputData(8, frame);
}

int DicomFile::GetNumberOfImage() const
{
	Sint32 number = -1;
	df->getDataset()->findAndGetSint32(DCM_InstanceNumber, number);
	return number;

}

std::string DicomFile::GetSeriesInstanceUID()const
{
	OFString id;
	df->getDataset()->findAndGetOFString(DCM_SeriesInstanceUID, id);
	return std::string(id.c_str());
}

bool DicomSeries::Push(std::shared_ptr<DicomFile> df)
{
	if (_series_map.empty())
	{
		_SeriesInstanceUID = df->GetSeriesInstanceUID();
		int num = df->GetNumberOfImage();
		_series_map.insert({ num,df });
	}
	else
	{
		if (_SeriesInstanceUID != df->GetSeriesInstanceUID())
			return false;
		int num = df->GetNumberOfImage();
		if (_series_map.find(num) != _series_map.end())
			return false;

		_series_map.insert({ num,df });
	}

	return true;
}

bool DicomSeries::Pop(int num)
{
	if (_series_map.find(num) == _series_map.end())
		return false;

	_series_map.erase(num);
	return  true;
}

void DicomSeries::Clear()
{
	_SeriesInstanceUID = std::string{};
	_series_map.clear();
}

int DicomSeries::GetNumberOfDicoms() const
{
	return _series_map.size();
}

std::shared_ptr<DicomFile> DicomSeries::GetDicom(int n_th)
{
	if (n_th >= _series_map.size())
		return nullptr;

	int n = 0;
	auto iter = _series_map.begin();
	while (n != n_th)
	{
		n++;
		iter++;
	}

	return iter->second;
}

void DicomSeries::GetSeriesDcmMinMaxValue(double& min, double& max) const
{
	double min_value = DBL_MAX;
	double max_value = DBL_MIN;
	for (auto it = _series_map.begin(); it != _series_map.end(); it++)
	{
		it->second->GetMinMaxValues(min, max);
		if (min_value > min)
			min_value = min;
		if (max_value < max)
			max_value = max;
	}
	min = min_value;
	max = max_value;
}

bool DicomFile::GetMinMaxValues(double& min, double& max) const
{
	if (df == nullptr)
		return false;
	return di->getMinMaxValues(min, max);
}