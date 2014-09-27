#pragma once
#include "activationfunction.h"
class TanhActivationFunction :
	public ActivationFunction
{
public:
	TanhActivationFunction(void);
	~TanhActivationFunction(void);
	 Mat activationFunction(Mat data);
	 String name();
};

