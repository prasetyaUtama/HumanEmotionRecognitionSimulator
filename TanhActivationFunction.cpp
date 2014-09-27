#include "stdafx.h"
#include "TanhActivationFunction.h"


String TanhActivationFunction::name()
{
	return "TanhActivationFunction";
}
TanhActivationFunction::TanhActivationFunction(void)
{
}


TanhActivationFunction::~TanhActivationFunction(void)
{
}
Mat TanhActivationFunction::activationFunction(Mat data)
{
	
	for (int y = 0; y < data.rows;y++)
	{
		for(int x = 0; x < data.cols;x++){
			data.at<double>(y,x) = (exp(2 * data.at<double>(y,x)) - 1) / (exp(2 * data.at<double>(y,x)) + 1);

		}
	}
	return data;
}