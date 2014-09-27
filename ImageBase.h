#pragma once
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
using namespace cv;
using namespace std;
class ImageBase
{
public:
	ImageBase(void);
	~ImageBase(void);
	void crop(Mat img, Mat &dst, Rect rect);

public:
	static void saveImage(Mat &img, String fileName);
	static void loadImage(Mat &img,String fileName);
	static void normalizeImage(Mat &img);
	static void powImage(Mat &src, Mat &dst, double power);
	static void convertImage(Mat& src);
	static void transformImage(Mat& src);

};

