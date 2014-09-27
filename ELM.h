#pragma once
#include <opencv2\core\core.hpp>
#include "Network.h"
#include "stdafx.h"
#include <sstream>
#include <string>
#include <iostream>
#include <time.h>
using namespace cv;
using namespace std;
class ELM
{
public:
	ELM(void);
	~ELM(void);
	ELM(Network *network);

public:
	void train(double C);
	void test();
	double getError();
	Mat biasMatrix();
	Mat generateInputWeight();
	Mat generateBias();
	Mat classify();
private:
	double error;
	Mat actualOutputTrain;
	Mat errorTrain;
	Network* network;
};

