#pragma once
#include <opencv2\core\core.hpp>
#include<opencv2/ml/ml.hpp>
#include "Network.h"
#include "stdafx.h"
#include <time.h>
using namespace cv;
using namespace std;

class MLP
{
public:
	MLP(void);
	~MLP(void);
	MLP(Network *network);

public:
	void train(Scalar hiddenCount);
	void test();
	void saveMLP(String fileName);
	void loadMLP(String fileName);
	double getError();

private:
	double error;
	Mat actualOutputTrain;
	Mat errorTrain;
	Network* network;
	CvANN_MLP ann;


};

