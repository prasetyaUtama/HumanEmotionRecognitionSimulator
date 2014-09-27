#pragma once
#include "opencv2\core\core.hpp"
using namespace cv;
using namespace std;
class SubSpace
{
public:
	SubSpace(void);
	~SubSpace(void);
	static void savePCA(String fileName, Mat eigenVector, Mat mean);
	static void loadPCA(String fileName, Mat &eigenVector, Mat &mean);
};

