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

	unsigned char* Create8BitsImage(int frame = 0)const;

	int GetImageWidth() const;
	int GetImageHeight()const;
	int GetNumberOfFrames()const;

	void GetWindow(double& win_center, double& win_width)const;
	void GetDefaultWindow(double& win_center, double& win_width)const;
	void SetWindow(double win_center, double win_width);

	bool GetPixelSpacing(double &sx, double &sy)const;
	
	
	bool FindAndGetOFString(const DcmTagKey& tagKey, std::string& value) const;
	OFCondition FindAndGetOFString(const DcmTagKey& tagKey, OFString& value,
		                           const unsigned long pos = 0,
		                           const OFBool searchIntoSub = OFFalse) const;

private:
	int BitsAllocatedOfPerPixel()const;
	int BitsStoredOfPerPixel()const;
	int SamplesOfPerPixel()const;
	// Rescale Intercept
	int GetRescaleIntercept()const;
	// Rescale Slope
	double GetRescaleSlope()const;

	

private:
	double _window_width = 0;
	double _window_center = 0;
	std::shared_ptr<DcmFileFormat> dff;
};
#endif//__CORE_H__