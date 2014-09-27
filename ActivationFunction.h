#pragma once
#include <opencv2\core\core.hpp>
using namespace cv;
class ActivationFunction
{
public:
	ActivationFunction(void);
	~ActivationFunction(void);
	virtual  Mat activationFunction(Mat data);
	virtual String name();
};

