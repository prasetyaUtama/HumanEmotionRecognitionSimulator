#pragma once
#include "opencv2\core\core.hpp"
#include "opencv2\objdetect\objdetect.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <iostream>
#include <stdio.h>

//my class
#include "ImageBase.h"
using namespace cv;
using namespace std;
typedef struct  _FeaturesROI
{
	Mat eyeROI;
	Mat mouthROI; 
} FeaturesROI;

class FacialFeatures
{

private:
	CascadeClassifier face_cascade;
	CascadeClassifier eyes_cascade;
	CascadeClassifier mouth_cascade;
	ImageBase imageBase;
	Rect rectROI;


public:
	FacialFeatures(void);
	~FacialFeatures(void);
	void FaceDetection(Mat &img);
	void EyeDetection(Mat &img, Mat &dst);
	void MouthDetection(Mat &img, Mat&dst);
	FeaturesROI getFeaturesROI();


};
