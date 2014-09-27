#include "stdafx.h"
#include "KernelGeneration.h"
#include <complex>
#include <iostream>


KernelGeneration::KernelGeneration(void)
{
}


KernelGeneration::~KernelGeneration(void)
{
}

void KernelGeneration::gaborKernel(int size,int scale,int orientation, float sigma, float kMax, Mat& kernelImag, Mat &kernelReal)
{
	//orientation
	int u = orientation;
	//scale
	int v = scale;

	//psi
	complex<double> psi;
	//delta2
	double delta2 = pow(sigma,2);

	//phiu
	double phiu = u * 3.1416 / 8.0;

	//freq
	double f = sqrt(2.0);

	//i
	complex<double> i (0,1);

	complex<double> k = ( kMax / pow(f,v) ) * exp(i*phiu);


	complex<double> kn2 = pow(sqrt(pow(real(k),2)+pow(imag(k),2)),2);



	int y = 0;
	int x = 0;

	for(double m = (-size/2.0)+1;m<=size/2.0;m+=1.0){
		x = 0;
		for (double n = (-size/2.0)+1;n<=size/2.0;n+=1.0)
		{
			psi =  ( kn2 / delta2 ) * exp( -0.5 * kn2 * ( pow(m,2) + pow(n,2) ) / delta2) 
				* ( exp( i * ( real( k ) * m + imag ( k ) * n ) ) - exp ( -0.5 * delta2 ) );
			//kernel.at<float>(y,x) = sqrt(pow(real(psi),2)+pow(imag(psi),2));
			kernelReal.at<float>(y,x) = real(psi);
			kernelImag.at<float>(y,x) = imag(psi);
			x++;


		}
		y++;

	}

}

void KernelGeneration::gaborKernelDerv(int size,double bw,double gamma,double psi,double lambda,double theta, Mat& kernel){
	double sigma = lambda/3.1416*sqrt(log(2.0)/2.0)*(pow(2.0,bw)+1)/(pow(2.0,bw-1));
	double sigma_x = sigma;
	double sigma_y = sigma / gamma;

	int yI = size / 2;
	int xI = -size/2;
	float cell;
	for (int y = 0; y < size; y++)
	{
		xI = -size/2;
		for(int x = 0; x < size; x++)
		{

			double x_theta=xI*cos(theta)+yI*sin(theta);
			double y_theta=-xI*sin(theta)+yI*cos(theta);

			cell = exp(-0.5*((pow(x_theta,2.0)/pow(sigma_x,2.0))+(pow(y_theta,2.0)/pow(sigma_y,2.0)))) * cos (2.0*3.1416/lambda*x_theta+psi);

			kernel.at<float>(y,x) =cell;		
			//exp(-0.5*(x_theta.^2/sigma_x^2+y_theta.^2/sigma_y^2)).*cos(2*pi/lambda*x_theta+psi)
			xI++;
		}
		yI--;
	}

}
void KernelGeneration::getKernelSawSam90 (Mat &kernel){
	kernel= (Mat_<float>(3,3) << -3.0, -3.0, -3.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0);
}
void KernelGeneration::getKernelSawSam270(Mat &kernel){
	kernel= (Mat_<float>(3,3) << 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, -3.0, -3.0, -3.0);
}
