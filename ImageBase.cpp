#include "stdafx.h"
#include "ImageBase.h"


ImageBase::ImageBase(void)
{
}


ImageBase::~ImageBase(void)
{
}

void ImageBase::crop(Mat img, Mat &dst, Rect rect)
{
	Mat croppedImage;
	img(rect).clone().copyTo(croppedImage);
	dst = croppedImage;
	//SaveImage(croppedImage,fileName);

}

void ImageBase::saveImage(Mat &img,String fileName)
{
	//String imagePath = fileName;
	imwrite(fileName,img);

}

void ImageBase::loadImage(Mat &img, String fileName){
	img = imread(fileName);
}
void ImageBase::normalizeImage(Mat &img){
	//Max Min value
	double minVal; 
	double maxVal; 
	Point minLoc; 
	Point maxLoc;
	minMaxLoc( img, &minVal, &maxVal, &minLoc, &maxLoc );
	img = img/maxVal;
	/*Mat dst;
	normalize(img, dst, 0, 255, NORM_MINMAX, CV_32F);
	img = dst;*/
	
}
void ImageBase::convertImage(Mat& src)
{

	
	for (int y = 0; y < src.rows; y++)
	{

		for(int x = 0; x < src.cols;x++)
		{
			
			
			if (src.at<float>(y,x) <= 0.0)
			{
				src.at<float>(y,x) = 0.0;
			}
	/*		else {
				src.at<float>(y,x) = 1.0;
			}
			*/
			
			
		}
	}
}
void ImageBase::transformImage(Mat& src){
	for (int y = 0; y < src.rows; y++)
	{

		for(int x = 0; x < src.cols;x++)
		{
			
			
			if (src.at<float>(y,x) <= 0.25)
			{
				src.at<float>(y,x) = 0.0;
			}
			else
			{
				src.at<float>(y,x) = 1.0;
			}
			
			
			
		}
	}
}
void ImageBase::powImage(Mat &src, Mat &dst, double power){
	dst = Mat::zeros(src.rows,src.cols,CV_32F);
	double value;
	for (int y = 0;y < src.rows; y++)
	{
		for (int x = 0; x<src.cols;x++)
		{
			value = (double) src.at<float>(y,x);
			dst.at<float>(y,x) = (float) pow(value,power);
		}
	}
}