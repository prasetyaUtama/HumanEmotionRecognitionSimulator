#include "stdafx.h"
#include "SVMC.h"


SVMC::SVMC(void)
{
}


SVMC::~SVMC(void)
{
}

SVMC::SVMC(Network *network)
{
	SVMC::network = network;
}

void SVMC::train()
{
	CvSVMParams param = CvSVMParams();

	param.svm_type = CvSVM::C_SVC;
	param.kernel_type = CvSVM::RBF; //CvSVM::RBF, CvSVM::LINEAR ...
	param.degree = 0; // for poly
	param.gamma = 35; // for poly/rbf/sigmoid
	param.coef0 = 0; // for poly/sigmoid

	param.C = 32; // for CV_SVM_C_SVC, CV_SVM_EPS_SVR and CV_SVM_NU_SVR
	param.nu = 0.0; // for CV_SVM_NU_SVC, CV_SVM_ONE_CLASS, and CV_SVM_NU_SVR
	param.p = 0.0; // for CV_SVM_EPS_SVR

	param.class_weights = NULL; // for CV_SVM_C_SVC
	param.term_crit.type = CV_TERMCRIT_ITER +CV_TERMCRIT_EPS;
	param.term_crit.max_iter = 6000;
	param.term_crit.epsilon = 1e-6;
	// Train the SVM
	CvSVM SVM;
	Mat a(SVMC::network->getInput().rows,SVMC::network->getInput().cols,CV_32FC1);
	Mat b(SVMC::network->getDesiredOutput().rows,SVMC::network->getDesiredOutput().cols,CV_32FC1);
	SVMC::network->getInput().copyTo(a);
	SVMC::network->getDesiredOutput().copyTo(b);
	a.convertTo(a,CV_32FC1);
	b.convertTo(b,CV_32FC1);

	SVMC::svm.train(a.t(), b.t(), Mat(), Mat(), param);
}

void SVMC::saveSVM(String filename)
{
	SVMC::svm.save(filename.c_str());
}

void SVMC::loadSVM(String filename)
{
	SVMC::svm.load(filename.c_str());
}

void SVMC::test()
{
	Mat a(SVMC::network->getInput().rows,SVMC::network->getInput().cols,CV_32FC1);
	SVMC::network->getInput().copyTo(a);
	a.convertTo(a,CV_32FC1);
	Mat response(1, 1, CV_32FC1);
	response = SVMC::svm.predict(a.col(0).t());
	//cout << SVMC::network->getInput().at<double>(20,0) << "," << a.at<float>(20,0)<<"\n";
	//cout << response << "\n";
	SVMC::network->setOutput(response.t());
}
double SVMC::getError()
{
	Mat errorTrainPow;
	double ePow;
	double eSum;
	Mat response(1, 1, CV_32FC1);
	SVMC::actualOutputTrain =  Mat(SVMC::network->getDesiredOutput().cols, SVMC::network->getDesiredOutput().rows, CV_64F);
	Mat a(SVMC::network->getInput().rows,SVMC::network->getInput().cols,CV_32FC1);
	SVMC::network->getInput().copyTo(a);
	a.convertTo(a,CV_32FC1);
	for(int i = 0; i< SVMC::network->getInput().cols;i++)
	{
		SVMC::svm.predict(a.col(i).t(),response);

		response.copyTo(SVMC::actualOutputTrain.row(i));



	}

	SVMC::errorTrain = SVMC::network->getDesiredOutput() - SVMC::actualOutputTrain.t();
	
	for(int x = 0; x < SVMC::errorTrain.cols;x++)
	{
		eSum = 0;
		for (int y = 0; y < SVMC::errorTrain.rows;y++){
			//cout << ELM::errorTrain.at<double>(y,x);
			ePow = pow(SVMC::errorTrain.at<double>(y,x),2);
			eSum = eSum + ePow;
		}
		SVMC::error = SVMC::error + ( (eSum)/SVMC::errorTrain.rows*7);
	}
	//cout << ELM::error<<"\n\n";
	return sqrt(SVMC::error);
}

int SVMC::supportVectorCount()
{
	return SVMC::svm.get_support_vector_count();
}