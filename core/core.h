#ifndef __CORE_H__
#define __CORE_H__

#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include <string>
#include <map>

class DicomFile
{
public:
	DicomFile() = default;
	DicomFile(std::string filename);
	virtual bool LoadFile(std::string filename);

	virtual int GetWidth()const;
	virtual int GetHeight()const;

	virtual bool GetWindow(double& win_center, double& win_width)const;
	virtual bool GetDefaultWindow(double& win_center, double& win_width)const;
	virtual bool SetWindow(double  win_center, double  win_width);

	virtual int GetNumberOfFrames()const;
	virtual const unsigned char*  GetOutputData(int frame)const;

	virtual int GetNumberOfImage()const;
	virtual std::string GetSeriesInstanceUID()const;
	virtual bool GetMinMaxValues(double &min, double &max)const;


private:
	std::shared_ptr<DcmFileFormat> df;
	std::shared_ptr<DicomImage> di;
};

class DicomSeries
{
public:
	bool Push(std::shared_ptr<DicomFile> df);
	bool Pop(int key);
	void Clear();
	int GetNumberOfDicoms()const;
	std::shared_ptr<DicomFile> GetDicom(int n_th);

	void GetSeriesDcmMinMaxValue(double& min, double& max) const;

private:
	std::string _SeriesInstanceUID;
	std::map<int, std::shared_ptr<DicomFile>> _series_map;
};
#endif//__CORE_H__



//std::vector<std::thread> threads;
//std::vector<std::shared_ptr<DicomFile>> dfs;
//dfs.resize(fileCount);
//for (int i = 0; i < fileCount; i++)
//{
//	auto f = [&](QString fileName, int i) {
//		std::shared_ptr<DicomFile> df = std::make_shared<DicomFile>();
//		df->LoadFile(fileName.toStdString());
//		dfs[i] = df;
//	};
//
//	QString fileName = dir.absoluteFilePath(dir[i]);
//	threads.push_back(std::thread(f, fileName, i));
//}
//for (int i = 0; i < fileCount; i++)
//{
//	threads[i].join();
//}
//for (int i = 0; i < fileCount; i++)
//{
//	series.Push(dfs[i]);
//}
//return true;