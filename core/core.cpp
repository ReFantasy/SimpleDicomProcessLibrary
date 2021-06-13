#include "core.h"
#include "global.h"
#include <vector>
#include <thread>
#include <memory>
#include <filesystem>

#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcmetinf.h"
#include "dcmtk/ofstd/ofstd.h"
#include "dcmtk/dcmjpeg/djdecode.h"
#include "easylogging++.h"


using namespace std;

DicomFile::DicomFile(const std::string& filename)
{
	/*OFCondition status = df->loadFile(filename.c_str());
	if (!status.good())
	{
		df = std::make_shared<DcmFileFormat>();
	}
	else
	{
        Decoder(df);
        di = std::make_shared<DicomImage>(df->getDataset(), df->getDataset()->getCurrentXfer());
	}*/

	LoadFile(filename);
}

bool DicomFile::LoadFile(const std::string &filename)
{
	auto tmp_df = std::make_shared<DcmFileFormat>();

	OFCondition status = tmp_df->loadFile(filename.c_str());

	if (status.good())
    {
        Decoder(tmp_df);
        df = tmp_df;
        di = std::make_shared<DicomImage>(tmp_df->getDataset(), tmp_df->getDataset()->getCurrentXfer());
		
		di->setMinMaxWindow();
		
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

const unsigned char* DicomFile::GetOutputData(int frame) const
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

std::string DicomFile::GetSeriesInstanceUID() const
{
	OFString id;
	df->getDataset()->findAndGetOFString(DCM_SeriesInstanceUID, id);
	return std::string(id.c_str());
}

bool DicomSeries::Add(std::shared_ptr<DicomFile> df)
{
	if (_series_map.empty())
	{
		_SeriesInstanceUID = df->GetSeriesInstanceUID();
		int num = df->GetNumberOfImage();
		_series_map.insert({ num, df });
	}
	else
	{
		if (_SeriesInstanceUID != df->GetSeriesInstanceUID())
			return false;
		int num = df->GetNumberOfImage();
		if (_series_map.find(num) != _series_map.end())
			return false;

		_series_map.insert({ num, df });
	}

	return true;
}

bool DicomSeries::Delete(int key)
{
	if (_series_map.find(key) == _series_map.end())
		return false;

	_series_map.erase(key);
	return true;
}

void DicomSeries::Clear()
{
	_SeriesInstanceUID = std::string{};
	_series_map.clear();
}

int DicomSeries::GetTotalFrames() const
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

bool DicomSeries::ReadDir(std::string dir)
{
	std::filesystem::path path(dir);
	if (!std::filesystem::exists(path))
		return false;

	std::filesystem::directory_entry entry(dir);
	if (!entry.is_directory())
		return false;

	
	std::vector<std::thread> threads;
	std::vector<std::shared_ptr<DicomFile>> dfs;

	std::filesystem::directory_iterator iters(dir);
	for (const auto& iter : iters)
	{
		auto load_file = [](const std::shared_ptr<DicomFile> df, const std::string& filename){
			df->LoadFile(filename);
		};

		auto df = std::make_shared<DicomFile>();
		dfs.push_back(df);
		threads.emplace_back(load_file, df, iter.path().string());
		
	}

	for (auto& t : threads)t.join();
	for (auto& df : dfs)Add(df);

	return true;
}

bool DicomSeries::GetWindow(double& win_center, double& win_width) const
{
	if(GetTotalFrames()<1)
		return false;
	_series_map.begin()->second->GetWindow(win_center, win_width);
	return true;
}

bool DicomSeries::SetWindow(double win_center, double win_width)
{
	bool res = true;
	for(auto it :_series_map)
	{
		res = it.second->SetWindow(win_center,win_width);
		if(res == false)
			break;;
	}
	return res;
}

bool DicomFile::GetMinMaxValues(double& min, double& max) const
{
	if (df == nullptr)
		return false;
    return di->getMinMaxValues(min, max);
}

void DicomFile::Decoder(std::shared_ptr<DcmFileFormat> df)
{
    DJDecoderRegistration::registerCodecs();
    df->getDataset()->chooseRepresentation(EXS_LittleEndianExplicit,NULL);
    //DJDecoderRegistration::cleanup();
}
