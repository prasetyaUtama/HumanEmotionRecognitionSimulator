// HumanEmotionRecognition.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MainWindow.h"
#include "opencv2\opencv.hpp"
#include "Features.h"
#include <iostream>

using namespace std;


int main(int argc, char*argv[])
{
	MainWindow main = MainWindow(argc,argv);
	/*Mat inputNetwork(70,1,CV_64F);
	Features features = Features();
	features.getFeatures("images/TestData/3.tiff",inputNetwork);
	
	
	waitKey(0); */
	system("pause");
	
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}

