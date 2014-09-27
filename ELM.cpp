#include "stdafx.h"
#include "ELM.h"


ELM::ELM(void)
{
}


ELM::~ELM(void)
{
}
ELM::ELM(Network *network){
	ELM::network = network;
}
void ELM::train(double C){
	Mat identity;
	Mat hiddenLayerTemp;
	Mat hiddenLayerTempInv;
	Mat tempH;
	Mat biasMatrix;
	Mat H;
	Mat hiddenLayerWeight;
	Mat pinvH;
	ELM::network->setInputWeight(ELM::generateInputWeight());
	ELM::network->setBias(ELM::generateBias());
	tempH = ELM::network->getInputWeight() * ELM::network->getInput();

	biasMatrix = ELM::biasMatrix();

	tempH = tempH + biasMatrix;
	//cout << tempH.at<double>(0,0);
	ELM::network->setHiddenLayer(tempH);
	//cout << ELM::network->getHiddenLayer().at<double>(0,0);
	H = ELM::network->getHiddenLayer();

	//invert(H.t(), pinvH, cv::DECOMP_SVD);
	//hiddenLayerWeight = pinvH * ELM::network->getDesiredOutput().t();

	identity = Mat::eye(H.rows,H.rows,CV_64F);
	
	hiddenLayerTemp = (identity / C )+ (H * H.t());
	//invert(hiddenLayerTemp,hiddenLayerTempInv, DECOMP_SVD);
	hiddenLayerTempInv = hiddenLayerTemp.inv(DECOMP_SVD);
	clock_t tStart = clock();
	hiddenLayerWeight = hiddenLayerTempInv * H * ELM::network->getDesiredOutput().t();
	double timeExecution = (double)(clock() - tStart)/CLOCKS_PER_SEC;
	cout << "training time = "<< timeExecution<<"\n\n";


	//cout << hiddenLayerWeight.at<double>(0,0);
	ELM::network->setHiddenLayerWeight(hiddenLayerWeight);
	//cout << ELM::network->getHiddenLayerWeight().at<double>(0,0);


}
Mat ELM::biasMatrix(){
	Mat matrixBias(ELM::network->getBias().rows,ELM::network->getInput().cols, CV_64F);

	for (int y = 0; y < ELM::network->getBias().rows; y++) {
		for (int x = 0; x < ELM::network->getInput().cols; x++) {
			matrixBias.at<double>(y,x) = ELM::network->getBias().at<double>(y,0);

		}
	}
	return matrixBias;
}

Mat ELM::generateInputWeight(){
	/*Mat weight = (Mat_<double>(3,3)<<-0.2247, -0.2128, 0.900, 0.7915, 0.3509, 0.2494, 0.7755, -0.4956, -0.5861);*/
	Mat weight(ELM::network->getHiddenLayerCount(),ELM::network->getInputCount(), CV_64F);
	//srand((unsigned)time(0));
	//RAND_MAX << 3;
	//for (int y = 0; y < ELM::network->getHiddenLayerCount(); y++) {
	//	for(int x = 0; x < ELM::network->getInputCount();x++)
	//	{
	//		weight.at<double>(y,x) =  (double)rand()/(double)RAND_MAX;
	//		if(((rand()%2)-1)==-1){
	//			weight.at<double>(y,x) =  (double)rand()/(double)RAND_MAX * ((rand()%2)-1);
	//		}

	//	}

	//}

	randu(weight, Scalar::all(-0.9), Scalar::all(0.9));
	return weight;
}
Mat ELM::generateBias(){
	/*Mat bias = (Mat_<double>(3,1)<<0.1097, 0.5658,0.2743);*/

	Mat bias (ELM::network->getHiddenLayerCount(),1,CV_64F);
	//srand((unsigned)time(0));
	//RAND_MAX << 3;
	//for (int y = 0; y < ELM::network->getHiddenLayerCount(); y++) {

	//	bias.at<double>(y,0) =  (double)rand()/(double)RAND_MAX;
	//	if(((rand()%2)-1)==-1){
	//		bias.at<double>(y,0) =  (double)rand()/(double)RAND_MAX * ((rand()%2)-1);
	//	}


	//}

	randu(bias, Scalar::all(-0.9), Scalar::all(0.9));
	return bias ;
}

void ELM::test()
{
	Mat tempH;
	Mat H;


	//cout << ELM::network->getHiddenLayerWeight().at<double>(59,0)<<"\n\n";
	//cout <<ELM::network->getBias()<<"\n\n";
	//ELM::network->setBias(ELM::biasMatrix());
	//cout << ELM::network->getInput();
	tempH = ELM::network->getInputWeight()* ELM::network->getInput().col(0);
	tempH = tempH + ELM::network->getBias();
	//cout << tempH<<"\n\n";
	//cout << tempH.at<double>(0,0);
	ELM::network->setH(tempH);
	H = ELM::network->getH();
	//cout << H.at<double>(0,0);
	//cout << H.at<double>(0,0)<<"\n\n";
	//cout << H.size() << "\n\n";
	//cout << H << "\n\n";
	//cout << ELM::network->getHiddenLayerWeight()<<"\n\n";

	//cout << H.t()*ELM::network->getHiddenLayerWeight() << "\n\n";
	//cout << ELM::network->getHiddenLayerWeight().at<double>(0,0);
	ELM::network->setOutput(H.t()*ELM::network->getHiddenLayerWeight());

}

double ELM::getError()
{
	Mat H;
	Mat errorTrainPow;
	double ePow;
	double eSum;
	ELM::error = 0;
	H = ELM::network->getHiddenLayer();
	ELM::network->setOutput(H.t()*ELM::network->getHiddenLayerWeight());
	ELM::actualOutputTrain = ELM::network->getOutput().t();
	ELM::errorTrain = ELM::network->getDesiredOutput() - ELM::actualOutputTrain;

	for(int x = 0; x < ELM::errorTrain.cols;x++)
	{
		eSum = 0;
		for (int y = 0; y < ELM::errorTrain.rows;y++){
			//cout << ELM::errorTrain.at<double>(y,x);
			ePow = pow(ELM::errorTrain.at<double>(y,x),2);
			eSum = eSum + ePow;
		}
		ELM::error = ELM::error + eSum;
	}
	ELM::error = ELM::error/(ELM::errorTrain.cols*ELM::errorTrain.rows);
	//cout << ELM::error<<"\n\n";
	return sqrt(ELM::error);
}