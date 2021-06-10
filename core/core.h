#ifndef __CORE_H__
#define __CORE_H__

#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmimgle/dcmimage.h"


class Dicom
{
public:
	Dicom() = default;
	Dicom(std::string filename);
	virtual bool LoadFile(std::string filename);

	virtual int GetWidth()const;
	virtual int GetHeight()const;

	virtual bool GetWindow(double& win_center, double& win_width)const;
	virtual bool SetWindow(double  win_center, double  win_width);

	virtual int GetNumberOfFrames()const;
	virtual const unsigned char*  GetOutputData(int frame)const;

private:
	std::shared_ptr<DcmFileFormat> df;
	std::shared_ptr<DicomImage> di;
};

#endif//__CORE_H__