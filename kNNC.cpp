#include "stdafx.h"
#include "kNNC.h"


kNNC::kNNC(void)
{
}


kNNC::~kNNC(void)
{
}

kNNC::kNNC(Network *network)
{
	kNNC::network = network;
}

void kNNC::train()
{
	Mat a(kNNC::network->getInput().rows,kNNC::network->getInput().cols,CV_32FC1);
	Mat b(kNNC::network->getDesiredOutput().rows,kNNC::network->getDesiredOutput().cols,CV_32FC1);
	kNNC::network->getInput().copyTo(a);
	kNNC::network->getDesiredOutput().copyTo(b);
	a.convertTo(a,CV_32FC1);
	b.convertTo(b,CV_32FC1);
	kNNC::knn = new CvKNearest(a.t(),b.t());
}
void kNNC::setNetwork(Network *network)
{
	kNNC::network = network;
}
void kNNC::test()
{
	Mat a(kNNC::network->getInput().rows,kNNC::network->getInput().cols,CV_32FC1);
	kNNC::network->getInput().copyTo(a);
	a.convertTo(a,CV_32FC1);
	Mat response(1, 1, CV_32FC1);
	//cout << a.col(0);
	int output = kNNC::knn->find_nearest(a.col(0).t(),7,0,0);
	response.at<float>(0,0) = output;
	kNNC::network->setOutput(response.t());
}
void kNNC::saveKNN(String filename)
{
	kNNC::knn->save(filename.c_str());
}

void kNNC::loadKNN(String filename, int numInput, int numDesired)
{
	FileStorage fs(filename, FileStorage::READ);
	FileNode dataTrain = fs["dataTrain"];
	int numData = (int)fs["numData"];
	int type = (int)fs["type"];
	Mat inputNetwork(numInput,numData,CV_64F);
	Mat desiredOutputNetwork(1,numData,CV_64F);
	fs["Input"] >> inputNetwork;
	fs["Output"] >> desiredOutputNetwork;
	DataSet dataSet = DataSet(inputNetwork,desiredOutputNetwork);
	Network network1 = Network(dataSet,100);
	kNNC::network = &network1;
	kNNC::train();
}