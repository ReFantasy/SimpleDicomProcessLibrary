#ifndef __CORE_H__
#define __CORE_H__
#include <string>
#include <memory>
#include "dcmtk/dcmdata/dcfilefo.h"

class Dicom
{
public:
	bool LoadFile(std::string filename);
	/**
	 * @brief Get raw image data pointer to first pixel.Do not free the returned pointer
	 * @param frame 
	 * @return 
	*/
	const char* GetRawImageData(int frame = 0)const;

	char* Get8BitsImage(int frame = 0)const;

	int GetImageWidth() const;
	int GetImageHeight()const;
	int GetNumberOfFrames()const;

	void GetWindow(double& win_center, double& win_width)const;
	void SetWindow(double win_center, double win_width);
	
	int BitsAllocatedOfPerPixel()const;
	int BitsStoredOfPerPixel()const;
	int SamplesOfPerPixel()const;
	bool FindAndGetOFString(const DcmTagKey& tagKey, std::string& value) const;

public:
	OFCondition FindAndGetOFString(const DcmTagKey& tagKey, OFString& value,
		                           const unsigned long pos = 0,
		                           const OFBool searchIntoSub = OFFalse) const;

private:
	// Rescale Intercept
	int GetRescaleIntercept()const;
	// Rescale Slope
	double GetRescaleSlope()const;

	

private:
	int _image_rows = 0;
	int _image_cols = 0;
	double _window_width = 0;
	double _window_center = 0;


private:
	std::shared_ptr<DcmFileFormat> dff;
};
#endif//__CORE_H__