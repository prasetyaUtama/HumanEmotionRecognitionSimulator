#pragma once
#include <opencv2\core\core.hpp>
#include<opencv2/ml/ml.hpp>
#include "Network.h"
#include "stdafx.h"
#include <time.h>
class SVMC
{
public:
	SVMC(void);
	~SVMC(void);
	SVMC(Network *Network);
public:
	void train();
	void test();
	void saveSVM(String fileName);
	void loadSVM(String fileName);
	double getError();
	int supportVectorCount();
private:
	double error;
	Mat actualOutputTrain;
	Mat errorTrain;
	Network* network;
	CvSVM svm;

};

