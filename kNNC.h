#pragma once
#include <opencv2\core\core.hpp>
#include "ml.h"
#include "Network.h"
#include "stdafx.h"

using namespace cv;
class kNNC
{
public:
	kNNC(void);
	~kNNC(void);
	kNNC(Network *Network);
	void train();
	void test();
	void saveKNN(String fileName);
	void setNetwork(Network *Network);
	void loadKNN(String filename, int numInput, int numDesired);
	double getError();

private:
	double error;
	Mat actualOutputTrain;
	Mat errorTrain;
	Network* network;
	CvKNearest *knn;
};

