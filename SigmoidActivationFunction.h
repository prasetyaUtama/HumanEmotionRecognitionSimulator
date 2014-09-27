#pragma once
#include "activationfunction.h"
class SigmoidActivationFunction :
	public ActivationFunction
{
public:
	SigmoidActivationFunction(void);
	~SigmoidActivationFunction(void);
	 Mat activationFunction(Mat data);
	 String name();
};

