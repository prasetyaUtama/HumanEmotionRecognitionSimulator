#include "stdafx.h"
#include "SigmoidActivationFunction.h"


String SigmoidActivationFunction::name()
{
	return "SigmoidActivationFunction";
}
SigmoidActivationFunction::SigmoidActivationFunction(void)
{
}


SigmoidActivationFunction::~SigmoidActivationFunction(void)
{
}

Mat SigmoidActivationFunction::activationFunction(Mat data)
{
	for (int y = 0; y < data.rows;y++)
	{
		for(int x = 0; x < data.cols;x++){
			data.at<double>(y,x) = 1/(1+exp(-data.at<double>(y,x)));

		}
	}
	return data;
}
