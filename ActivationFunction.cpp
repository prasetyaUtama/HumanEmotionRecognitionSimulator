#include "stdafx.h"
#include "ActivationFunction.h"


ActivationFunction::ActivationFunction(void)
{
}


ActivationFunction::~ActivationFunction(void)
{
}


Mat ActivationFunction::activationFunction(Mat data){
	return data;
}
String ActivationFunction::name()
{
	return "ActivationFunction";
}