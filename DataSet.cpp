#include "stdafx.h"
#include "DataSet.h"


DataSet::DataSet(Mat input, Mat  desiredOutput)
{
	DataSet::input = input;
	DataSet::desiredOutput = desiredOutput;
}


DataSet::DataSet(Mat input)
{
	DataSet::input = input;
}

Mat DataSet::getInput()
{
	return DataSet::input;
}

Mat DataSet::getDesiredOutput()
{
	return DataSet::desiredOutput;
}
void DataSet::saveDataSet(String filename){

	FileStorage fs;
	fs.open(filename, FileStorage::WRITE);
	fs << "type"<<2;
	fs << "Input"<<DataSet::getInput();
	fs << "Output"<<DataSet::getDesiredOutput();

	fs.release();
}


