#pragma once
#include "opencv2\core\core.hpp"
using namespace cv;
class DataSet
{
public:
	DataSet(Mat input, Mat  desiredOutput);
	DataSet(Mat input);
	Mat getInput();
	Mat getDesiredOutput();
	void saveDataSet(String filename);
private:
	Mat input;
	Mat desiredOutput;
};

