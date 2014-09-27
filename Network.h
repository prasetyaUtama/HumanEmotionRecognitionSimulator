#pragma once
#include <opencv2\core\core.hpp>
#include "ActivationFunction.h"
#include "DataSet.h"

using namespace cv;
using namespace std;
class Network
{
public:
	Network(void);
	~Network(void);
	Network(DataSet data);
private:
	Mat input;
	Mat desiredOutput;
	Mat inputWeight;
	Mat hiddenLayer;
	Mat hiddenLayerWeight;
	Mat output;
	Mat bias;
	Mat H;
	ActivationFunction *outputActivation;
	ActivationFunction *hiddenActivation;
	int hiddenLayerCount;

public:


	//get number of input, hiddenLayer and output neuron
	int getInputCount() ;
	int getOutputCount();
	int getHiddenLayerCount();

	Network(DataSet trainSet, int hiddenCount);

	//get value of input, desired, inputweight,hiddenLayer,hiddenLayerWeight, and output
	Mat getInput();
	Mat getDesiredOutput();
	Mat getInputWeight();
	Mat getHiddenLayer();
	Mat getHiddenLayerWeight();
	Mat getOutput();

	//set activation function of input and output layer
	void setOutputActivation(ActivationFunction* outputActivation);
	void setHiddenActivation(ActivationFunction* hiddenActivation);

	//set value of hidden and output layer
	void setHiddenLayer(Mat hiddenLayer);
	void setOutput(Mat output);

	//get and set bias
	Mat getBias();
	void setBias(Mat bias);

	//set value of weight of input and weight of hiddenLayer
	void setInputWeight(Mat inputWeight);
	void setHiddenLayerWeight(Mat hiddenLayerWeight);

	//get and set value of H
	Mat getH();
	void setH(Mat tempH);

	//get activation function in hidden and output layer
	ActivationFunction getOutputActivation();
	ActivationFunction getHiddenActivation();

	//save this network to a xml
	void saveNetwork(String filename);

	//load netwowrk from a xml
	void loadNetwork(String filename);
private:
	//set this network
	void setNetwork(Network network);
	ActivationFunction setActivationFunction(String activationFunctionName);


};

