#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;
class KernelGeneration
{
public:
	KernelGeneration(void);
	~KernelGeneration(void);

public:
	static void gaborKernel(int size,int scale,int orientation, float sigma, float kMax, Mat& kernelImag, Mat &kernelReal);
	static void getKernelSawSam90 (Mat &kernel);
	static void	getKernelSawSam270(Mat &kernel);
	static void gaborKernelDerv(int size,double bw,double gamma,double psi,double lambda,double theta, Mat& kernel);

};

