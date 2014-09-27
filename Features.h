#pragma once
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace cv;
class Features
{
public:
	Features(void);
	~Features(void);
	bool getFeatures(String fileName, Mat &features);
	Mat asMatrixRow(Mat data);

};

