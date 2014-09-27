#include "stdafx.h"
#include "FacialFeatures.h"




String XML_FACE_FILE = "resources/haarcascade_frontalface_default.xml";
String XML_MOUTH_FILE =  "resources/haarcascade_mcs_mouth.xml";
String XML_EYES_FILE =  "resources/haarcascade_mcs_eyepair_big.xml";
FeaturesROI featuresROI;


FacialFeatures::FacialFeatures(void)
{
}


FacialFeatures::~FacialFeatures(void)
{
}

void FacialFeatures::FaceDetection(Mat &img)
{
	imageBase =  ImageBase();
	std::vector<Rect> faces;

	face_cascade.load(XML_FACE_FILE);
	face_cascade.detectMultiScale( img, faces, 1.2, 2, 0|CV_HAAR_DO_CANNY_PRUNING, Size(20, 20) );

	//-- Detect faces

	for( int i = 0; i < (int)faces.size(); i++ )
	{
		rectangle( img, 
			Point( faces[i].x, faces[i].y),
			Point( faces[i].x+faces[i].width, faces[i].y+faces[i].height),
			Scalar( 255, 255, 255 ),
			1,
			8,
			0);

		rectROI = Rect (Point( faces[i].x, faces[i].y),
			Point( faces[i].x+faces[i].width, faces[i].y+faces[i].height));
		//resampleImage.Crop(img,rectROI,"face.tiff");


		//eyeROI = img(Rect(
		//            faces[i].x,            /* x = start from leftmost */
		//            (faces[i].y + faces[i].height/5.5), /* y = a few pixels from the top */
		//            faces[i].width,        /* width = same width with the face */
		//            faces[i].height/3.0    /* height = 1/3 of face height */));
		featuresROI.eyeROI = img(Rect(faces[i].x,faces[i].y,(int)faces[i].width,faces[i].height/1.50));
		
		featuresROI.mouthROI = img(Rect(faces[i].x,faces[i].y+(faces[i].height*2/3),(int)faces[i].width,faces[i].height/3.0));
		//mark in 15-09-2013
		//EyeDetection(eyeROI);
		//MouthDetection(mouthROI);



	}
	

}

void FacialFeatures::EyeDetection(Mat &img, Mat &dst)
{
	imageBase =  ImageBase();
	std::vector<Rect> eyes;
	int widthStart,heightStart;
	eyes_cascade.load(XML_EYES_FILE);
	eyes_cascade.detectMultiScale(img, eyes, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE , Size(25, 15));
	for( int j = 0; j < (int)eyes.size(); j++ )
	{
		/*rectangle( img, 
		Point( eyes[j].x-(int)(eyes[j].width*0.1),
		eyes[j].y-(int)(eyes[j].height*0.7)),

		Point( eyes[j].x+eyes[j].width+(int)(eyes[j].width*0.1), 
		eyes[j].y+(int)(eyes[j].height*0.2)+eyes[j].height),
		Scalar( 0, 255, 255 ),
		1,
		8,
		0);
		*/
		if(j == 0){
			widthStart = eyes[j].x;
			/*if((eyes[j].x-(int)(eyes[j].width*0.01))>=0)
			{
				widthStart = eyes[j].x-(int)(eyes[j].width*0.01);
			}*/
			heightStart = eyes[j].y - 10;
			/*if((eyes[j].y-(int)(eyes[j].height*0.7))>=0)
			{
				heightStart = eyes[j].y-(int)(eyes[j].height*0.7);
			}*/
		/*	rectROI = Rect (Point( widthStart,
				heightStart),

				Point( eyes[j].x+eyes[j].width+(int)(eyes[j].width*0.01), 
				eyes[j].y+eyes[j].height));*/


			rectROI = Rect (Point( widthStart,
				heightStart),

				Point( eyes[j].x+84 ,
				eyes[j].y+25));

			imageBase.crop(img,dst,rectROI);
		}


	}



}

void FacialFeatures::MouthDetection(Mat &img,Mat &dst)
{
	imageBase =  ImageBase();
	int widthStart,heightStart;
	std::vector<Rect> mouths;
	mouth_cascade.load(XML_MOUTH_FILE);
	mouth_cascade.detectMultiScale(img, mouths, 1.15, 4, 0|CV_HAAR_SCALE_IMAGE , Size(26, 15));
	for( int j = 0; j < (int)mouths.size(); j++ )
	{
		/*rectangle( img, 
		Point( mouths[j].x-(int)(mouths[j].width*0.1),
		mouths[j].y-(int)(mouths[j].height*0.1)),

		Point( mouths[j].x+mouths[j].width+(int)(mouths[j].width*0.1), 
		mouths[j].y+mouths[j].height),
		Scalar( 119, 250, 253 ),
		1,
		8,
		0);
		*/

		if(j==0){
			widthStart = mouths[j].x;
			/*if((mouths[j].x-(int)(mouths[j].width*0.05))>=0)
			{
				widthStart = mouths[j].x-(int)(mouths[j].width*0.05);
			}*/
			heightStart = mouths[j].y ;

		/*	if((mouths[j].y-5)>=0)
			{
				heightStart = mouths[j].y-5;
			}*/
			/*rectROI = Rect (Point( widthStart,
				heightStart),

				Point( mouths[j].x+mouths[j].width+(int)(mouths[j].width*0.05), 
				mouths[j].y+mouths[j].height+(mouths[j].height*0.05)));*/
			rectROI = Rect (Point( widthStart,
				heightStart),

				Point( mouths[j].x+48, 
				mouths[j].y+20));
			imageBase.crop(img,dst,rectROI);
		}

	}

}

//get features
FeaturesROI FacialFeatures::getFeaturesROI()
{
	return featuresROI;
}





