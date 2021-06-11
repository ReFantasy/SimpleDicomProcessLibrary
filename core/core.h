#ifndef __CORE_H__
#define __CORE_H__

#include <memory>
#include <string>
#include <map>

class DcmFileFormat;
class DicomImage;

class DicomFile
{
public:
	DicomFile() = default;
	explicit DicomFile(const std::string& filename);
	virtual bool LoadFile(const std::string &filename);

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
	bool ReadDir(std::string dir);
	bool Add(std::shared_ptr<DicomFile> df);
	bool Delete(int key);
	std::shared_ptr<DicomFile> GetDicom(int n_th);

	int GetTotalFrames()const;
	void GetSeriesDcmMinMaxValue(double& min, double& max) const;

	bool GetWindow(double& win_center, double& win_width)const;
	bool SetWindow(double  win_center, double  win_width);

	void Clear();
private:
	std::string _SeriesInstanceUID;
	std::map<int, std::shared_ptr<DicomFile>> _series_map;
};
#endif//__CORE_H__
