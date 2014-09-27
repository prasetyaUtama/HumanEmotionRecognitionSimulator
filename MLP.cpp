#include "stdafx.h"
#include "MLP.h"


MLP::MLP(void)
{
}


MLP::~MLP(void)
{
}

MLP::MLP(Network *network)
{
	MLP::network = network;
}
void MLP::train(Scalar hiddenCount)
{
	Mat layers = Mat(4, 1, CV_32SC1);
	layers.row(0) = Scalar(132); //input layer accepts x and y
	cout << hiddenCount.val[0]<<"\n";
	layers.row(1) = Scalar(hiddenCount.val[0]);//hidden layer
	layers.row(2) = Scalar(hiddenCount.val[0]);//hidden layer

	layers.row(3) = Scalar(7); //output layer returns 1 or -1


	//ANN criteria for termination
	CvTermCriteria criter;
	criter.max_iter = 1300;
	criter.epsilon = 0.0000001;
	criter.type = CV_TERMCRIT_ITER | CV_TERMCRIT_EPS;
	


	//ANN parameters
	CvANN_MLP_TrainParams params;
	params.train_method = CvANN_MLP_TrainParams::RPROP;
	params.bp_dw_scale = 0.001;
	params.bp_moment_scale = 0.0000001;
	params.term_crit = criter; //termination criteria
	params.rp_dw0 = 0.1; params.rp_dw_plus = 1.2; params.rp_dw_minus = 0.5;
	params.rp_dw_min = FLT_EPSILON; params.rp_dw_max = 50.;
	MLP::ann.create(layers,CvANN_MLP::SIGMOID_SYM,0.6,1);
	//cout << MLP::network->getDesiredOutput().t();
	//MLP::network->getInput().convertTo(MLP::network->getInput(),CV_32FC1);
	//MLP::network->getDesiredOutput().convertTo(MLP::network->getDesiredOutput(),CV_32FC1);
	ann.train(MLP::network->getInput().t(), MLP::network->getDesiredOutput().t(), cv::Mat(), cv::Mat(), params);


}
void MLP::saveMLP(String filename)
{
	MLP::ann.save(filename.c_str());
}
void MLP::loadMLP(String filename)
{
	MLP::ann.load(filename.c_str());
}

void MLP::test()
{
	Mat response(1, 7, CV_32FC1);
	Mat predicted(MLP::network->getInput().cols, 7, CV_32F);
	//cout << MLP::network->getInput().cols;
	//MLP::network->getInput().convertTo(MLP::network->getInput(),CV_32FC1);
	MLP::ann.predict(MLP::network->getInput().col(0).t(),response);

	MLP::network->setOutput(response.t());
	


}

double MLP::getError()
{
	Mat errorTrainPow;
	double ePow;
	double eSum;
	Mat response(1, 7, CV_32FC1);
	MLP::actualOutputTrain =  Mat(MLP::network->getDesiredOutput().cols, MLP::network->getDesiredOutput().rows, CV_64F);
	for(int i = 0; i< MLP::network->getInput().cols;i++)
	{
		MLP::ann.predict(MLP::network->getInput().col(i).t(),response);

		response.copyTo(MLP::actualOutputTrain.row(i));



	}

	MLP::errorTrain = MLP::network->getDesiredOutput() - MLP::actualOutputTrain.t();
	//cout << MLP::network->getDesiredOutput()<<"\n";
	//cout << MLP::actualOutputTrain.t()<<"\n";
	//cout << MLP::errorTrain<<"\n";
	for(int x = 0; x < MLP::errorTrain.cols;x++)
	{
		eSum = 0;
		for (int y = 0; y < MLP::errorTrain.rows;y++){
			//cout << ELM::errorTrain.at<double>(y,x);
			ePow = pow(MLP::errorTrain.at<double>(y,x),2);
			eSum = eSum + ePow;
		}
		MLP::error = MLP::error + ( (eSum)/MLP::errorTrain.rows*7);
	}
	//cout << ELM::error<<"\n\n";
	return sqrt(MLP::error);
}