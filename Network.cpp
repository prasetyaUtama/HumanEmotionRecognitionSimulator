#include "stdafx.h"
#include "Network.h"
#include "SigmoidActivationFunction.h"
#include "TanhActivationFunction.h"


Network::Network(void)
{
}


Network::~Network(void)
{
}

Network::Network(DataSet data){
	Network::input = data.getInput();

}
int Network::getInputCount(){
	return Network::input.rows;
}
int Network::getOutputCount(){
	return Network::output.rows;
}
int Network::getHiddenLayerCount(){
	return Network::hiddenLayerCount;
}
Network::Network(DataSet trainSet, int hiddenCount){
	Network::input = trainSet.getInput();
	Network::desiredOutput = trainSet.getDesiredOutput();
	Network::inputWeight = (Mat_<double>(Network::input.rows,hiddenCount));
	Network::hiddenLayer =  (Mat_<double>(hiddenCount,1));
	Network::bias =  (Mat_<double>(hiddenCount,1));
	Network::hiddenLayerWeight = (Mat_<double>(hiddenCount,Network::desiredOutput.rows));
	Network::output = (Mat_<double>(Network::desiredOutput.rows,Network::desiredOutput.cols));
	Network::hiddenLayerCount = hiddenCount;
	
}
Mat Network::getInput(){
	return Network::input;
}
Mat Network::getDesiredOutput(){
	return Network::desiredOutput;
}
Mat Network::getInputWeight(){
	return Network::inputWeight;
}
Mat Network::getHiddenLayer(){
	return Network::hiddenLayer;
}
Mat Network::getHiddenLayerWeight(){
	return Network::hiddenLayerWeight;
}
Mat Network::getOutput(){
	return Network::output;
}

void Network::setOutputActivation(ActivationFunction* outputActivation){
	Network::outputActivation = outputActivation;

}
void Network::setHiddenActivation(ActivationFunction* hiddenActivation){

	Network::hiddenActivation = hiddenActivation;


}
void  Network::setHiddenLayer(Mat hiddenLayer){

	Network::hiddenLayer = Network::hiddenActivation->activationFunction(hiddenLayer);

}
void  Network::setOutput(Mat output){
	Network::output = output;
}
Mat Network::getBias(){
	return bias;
}
void Network::setBias(Mat bias){
	Network::bias = bias;
}
void Network::setInputWeight(Mat inputWeight){
	Network::inputWeight =  inputWeight ;
}
void Network::setHiddenLayerWeight(Mat hiddenLayerWeight){
	Network::hiddenLayerWeight = hiddenLayerWeight;
}
Mat Network::getH(){
	return Network::H;
}
void Network::setH(Mat tempH){
	Network::H = Network::hiddenActivation->activationFunction(tempH);
}
ActivationFunction Network::getOutputActivation(){
	return *Network::outputActivation;
}
ActivationFunction Network::getHiddenActivation(){
	return *Network::hiddenActivation;
}
void Network::saveNetwork(String filename){

	FileStorage fs;
	fs.open(filename, FileStorage::WRITE);
	fs << "InputWeight"<<Network::getInputWeight();
	fs << "Bias"<<Network::getBias();
	fs << "HiddenLayerWeight"<<Network::getHiddenLayerWeight();
	fs << "HiddenActivationFunction" << Network::hiddenActivation->name();
	fs << "OutputActivationFunction" << "ActivationFunction";
	fs.release();
}
void Network::setNetwork(Network network){
	/*ActivationFunction* sig;
	ActivationFunction* tanh;
	SigmoidActivationFunction sigAc = SigmoidActivationFunction();		
	TanhActivationFunction tanhAc =  TanhActivationFunction();
	sig = &sigAc;
	tanh = &tanhAc;*/
	Network::setInputWeight(network.getInputWeight());
	Network::setHiddenLayerWeight(network.getHiddenLayerWeight());
	Network::setBias(network.getBias());
	/*Network::setHiddenActivation(&ActivationFunction());
	Network::setOutputActivation(&ActivationFunction());*/
}
void Network::loadNetwork(String filename){
	ActivationFunction* sig;
	ActivationFunction* tanh;
	SigmoidActivationFunction sigAc = SigmoidActivationFunction();		
	TanhActivationFunction tanhAc =  TanhActivationFunction();
	sig = &sigAc;
	tanh = &tanhAc;
	FileStorage fs;
	fs.open(filename, FileStorage::READ);

	Mat inputWeight;
	Mat bias;
	Mat hiddenLayerWeight;
	String hiddenActivationName;
	String outputActivationName;

	fs["InputWeight"] >> inputWeight;
	fs["Bias"] >> bias;
	fs["HiddenLayerWeight"] >> hiddenLayerWeight;
	fs["HiddenActivationFunction"] >> hiddenActivationName;
	fs["OutputActivationFunction"] >> outputActivationName;

	Network::setInputWeight(inputWeight);
	Network::setHiddenLayerWeight(hiddenLayerWeight);
	Network::setBias(bias);	
	/*Network::setHiddenActivation(sig);
	Network::setOutputActivation(tanh);*/
	Network::hiddenActivation = &sigAc;
	Network::outputActivation = &tanhAc;
}

ActivationFunction Network::setActivationFunction(String activationFunctionName){
	if(activationFunctionName.compare("TanhActivationFunction"))
	{
		return TanhActivationFunction();
	}
	else if(activationFunctionName.compare("SigmoidActivationFunction"))
	{
		return SigmoidActivationFunction();
	}
}