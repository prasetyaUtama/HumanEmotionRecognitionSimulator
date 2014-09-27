#include "stdafx.h"
#include "Features.h"
#include "ImageBase.h"
#include "FacialFeatures.h"
#include "KernelGeneration.h"
#include <exception>

#define PI 3.1416

String myreplace(String &s,
				 String toReplace,
				 String replaceWith)
{
	return(s.replace(s.find(toReplace), toReplace.length(), replaceWith));
}
Features::Features(void)
{
}


Features::~Features(void)
{
}
Mat Features::asMatrixRow(Mat data){
	int row = data.rows * data.cols;
	Mat result(1,row,CV_32F);
	int j = 0;
	for (int i = 0; i < data.rows;i++)
	{
		for(int h = 0; h<data.cols;h++)
		{
			result.at<float>(0,j) = data.at<float>(i,h);
			j++;
		}
	}
	//cout << result;
	return result;
}

bool Features::getFeatures(String fileName, Mat &features){
	//Image
	Mat image;
	Mat imgGry;
	Mat imgHst;

	//Facial features
	FacialFeatures facialFeatures = FacialFeatures();


	//Gabor Kernel
	int gaborSizeKernel = 35;
	Mat gaborKernelReal(gaborSizeKernel,gaborSizeKernel,CV_32F);
	Mat gaborKernelImag(gaborSizeKernel,gaborSizeKernel,CV_32F);

	//Gabor selected parameter
	double lambda  = 4;
	double theta   = 1;
	double psi1     = 0;
	double psi2     = PI/2;
	double gamma   = 0.5;
	double bw      = 1;
	double N       = 8;

	//convolution kernel
	Mat convolutionKernel90;
	Mat convolutionKernel270;

	//Features
	Mat eye;
	Mat eyeEq;
	Mat mouth;

	//-------------------------------Gabor filtered------------------------//

	//eye;
	Mat eyeFilteredGabor;
	Mat eyeFilteredGaborTemp;
	Mat eyeFilteredGaborSum;

	//mouth
	Mat mouthFilteredGabor;
	Mat mouthFilteredGaborTemp;
	Mat mouthFilteredGaborSum;

	//Imag Filtered
	Mat mouthFilteredGaborImag;
	Mat eyeFilteredGaborImag;

	//Real Filtered
	Mat mouthFilteredGaborReal;
	Mat eyeFilteredGaborReal;

	//Gabor Power
	Mat eyeGaborPower;
	Mat mouthGaborPower;

	//---------------------------end of Gabor filtered---------------------//

	//Convolution Filtere
	Mat mouthFilteredConv90;
	Mat mouthFilteredConv270;
	Mat mouthFilteredConv;

	Mat eyeFilteredConv90;
	Mat eyeFilteredConv270;
	Mat eyeFilteredConv;

	//Result filtered
	Mat mouthFiltered;
	Mat eyeFiltered;
	/*Mat eyeFiltered;*/


	int eyeFeature_limit = 714;
	int mouthFeature_limit = 240;



	//String eye and mouth

	String eyeSave, mouthSave;

	/*------------Detecting Mouth and Eye----------------*/
	ImageBase::loadImage(image,fileName);
	//convert image to gray scale
	cvtColor( image, imgGry, CV_BGR2GRAY );
	equalizeHist(imgGry,imgHst);

	//detect mouth and eye
	try{
		facialFeatures.FaceDetection(imgGry);
		facialFeatures.EyeDetection(facialFeatures.getFeaturesROI().eyeROI,eye);
		facialFeatures.MouthDetection(facialFeatures.getFeaturesROI().mouthROI,mouth);

	}
	catch(Exception e)
	{
		cout << "Detection Failed";
		return false;
	}
	//equalizeHist(eye,eye);
	//equalizeHist(mouth,mouth);
	//imshow("eye",eye);
	//imshow("mouth",mouth);
	eye.convertTo(eye, CV_32F, 1.0/255, 0);
	mouth.convertTo(mouth, CV_32F, 1.0/255, 0);

	//----------------------------------------Eye Gabour---------------------//
	eyeFilteredGaborSum = Mat::zeros(eye.rows,eye.cols,CV_32F);
	for (int i = 1; i <= N ;i++)
	{
		KernelGeneration::gaborKernel(gaborSizeKernel,2.0,i,PI/2.5,PI/2,gaborKernelImag,gaborKernelReal);
		filter2D(eye,eyeFilteredGaborReal,CV_32F,gaborKernelReal, Point(-1,-1),0,BORDER_REPLICATE);
		filter2D(eye,eyeFilteredGaborImag,CV_32F,gaborKernelImag,Point(-1,-1),0,BORDER_REPLICATE);
		magnitude(eyeFilteredGaborReal,eyeFilteredGaborImag,eyeFilteredGaborTemp);
		ImageBase::powImage(eyeFilteredGaborTemp,eyeGaborPower,2.0);


		eyeFilteredGaborSum += eyeGaborPower;

	}

	ImageBase::powImage(eyeFilteredGaborSum,eyeFilteredGabor,0.5);
	ImageBase::normalizeImage(eyeFilteredGabor);

	//------------------------------------End of Eye Gabor----------------------//


	//----------------------------------------Mouth Gabour---------------------//
	mouthFilteredGaborSum = Mat::zeros(mouth.rows,mouth.cols,CV_32F);
	for (int i = 1; i <= N ;i++)
	{
		KernelGeneration::gaborKernel(gaborSizeKernel,2.0,i,PI/3,PI/2,gaborKernelImag,gaborKernelReal);
		filter2D(mouth,mouthFilteredGaborReal,CV_32F,gaborKernelReal, Point(-1,-1),0,BORDER_REPLICATE);
		filter2D(mouth,mouthFilteredGaborImag,CV_32F,gaborKernelImag,Point(-1,-1),0,BORDER_REPLICATE);
		magnitude(mouthFilteredGaborReal,mouthFilteredGaborImag,mouthFilteredGaborTemp);
		ImageBase::powImage(mouthFilteredGaborTemp,mouthGaborPower,2.0);


		mouthFilteredGaborSum += mouthGaborPower;

	}

	ImageBase::powImage(mouthFilteredGaborSum,mouthFilteredGabor,0.5);
	ImageBase::normalizeImage(mouthFilteredGabor);

	//------------------------------------Mouth of Eye Gabor----------------------//
	//ImageBase::convertImage(mouthFilteredGabor);

	//---------------------------------Convolution filter----------------------//
	KernelGeneration::getKernelSawSam90(convolutionKernel90);
	KernelGeneration::getKernelSawSam270(convolutionKernel270);
	//Mouth
	filter2D(mouth,mouthFilteredConv90,CV_32F,convolutionKernel90);
	filter2D(mouth,mouthFilteredConv270,CV_32F,convolutionKernel270);	
	ImageBase::convertImage(mouthFilteredConv90);
	ImageBase::convertImage(mouthFilteredConv270);
	//cout << mouthFilteredConv270;
	magnitude(mouthFilteredConv90,mouthFilteredConv270,mouthFilteredConv);	


	ImageBase::convertImage(mouthFilteredConv);


	//Eye

	filter2D(eye,eyeFilteredConv90,CV_32F,convolutionKernel90);
	filter2D(eye,eyeFilteredConv270,CV_32F,convolutionKernel270);	
	ImageBase::convertImage(eyeFilteredConv90);
	ImageBase::convertImage(eyeFilteredConv270);
	//cout << mouthFilteredConv270;
	magnitude(eyeFilteredConv90,eyeFilteredConv270,eyeFilteredConv);	


	ImageBase::convertImage(eyeFilteredConv);

	magnitude(mouthFilteredConv,mouthFilteredGabor,mouthFiltered);
	magnitude(eyeFilteredConv,eyeFilteredGabor,eyeFiltered);

	//ImageBase::normalizeImage(mouthFiltered);
	//ImageBase::normalizeImage(eyeFiltered);

	ImageBase::normalizeImage(mouthFiltered);
	ImageBase::normalizeImage(eyeFiltered);




	//imshow("Test5",eyeFiltered);

	//imshow("Test6",mouthFilteredConv);
	//imshow("Test7",mouthFiltered);
	//imshow("Test8",eyeFiltered);


	//------------------------------end of Convolution Filter------------------//

	//eye
	/*PCA eyeEigen(eyeFiltered, Mat(), CV_PCA_DATA_AS_ROW, eyeFeature_limit);*/
	//mouth
	//PCA mouthEigen(mouthFiltered, Mat(), CV_PCA_DATA_AS_ROW, mouthFeature_limit);
	//Mat eyeFeature = eyeEigen.mean.clone();
	//Mat mouthFeature = mouthEigen.mean.clone();
	Mat EyeResize;
	Mat MouthResize;
	pyrDown(eyeFiltered, EyeResize, Size(eyeFiltered.cols/2,eyeFiltered.rows/2));
	pyrDown(mouthFiltered, MouthResize, Size(mouthFiltered.cols/2,mouthFiltered.rows/2));

	ImageBase::normalizeImage(EyeResize);
	ImageBase::normalizeImage(MouthResize);
	Mat featuresEye, featureMouth;
	featuresEye = EyeResize.reshape(1,1);
	featureMouth = MouthResize.reshape(1,1);

	ImageBase::convertImage(featuresEye);
	ImageBase::convertImage(featureMouth);
	ImageBase::transformImage(featuresEye);
	ImageBase::transformImage(featureMouth);

	//ImageBase::transformImage(EyeResize);
	//ImageBase::transformImage(MouthResize);
	/*imshow("Test3",EyeResize);
	imshow("Test4",MouthResize);*/
	int index = eyeFeature_limit;

	//cout << eyeEigen.eigenvalues <<"\n\n";

	/*eyeSave = myreplace(fileName,"D:/train/suceess/","D:/train/result/eye/");*/
	/*mouthSave = myreplace(fileName,"D:/train/suceess/","D:/train/result/mouth/");*/

	/*eyeFiltered = eyeFiltered * 255;*/
	/*mouthFiltered = mouthFiltered * 255;*/
	/*ImageBase::saveImage(eyeFiltered,eyeSave);*/
	/*ImageBase::saveImage(mouthFiltered,mouthSave);*/

	//insert eye to input

		for (int i = 0; i < eyeFeature_limit;i++)
	{

		features.at<double>(0,i) =  featuresEye.at<float>(0,i);
		//cout << features.at<double>(i,0)<< " ";

	}

	

	////insert mouth to input
	for (int i = 0; i < mouthFeature_limit;i++)
	{
		features.at<double>(0,index) = featureMouth.at<float>(0,i);		
		index++;
	}
	return true;

	//cout << features;
}
