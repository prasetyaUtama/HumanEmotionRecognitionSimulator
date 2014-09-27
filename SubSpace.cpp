#include "stdafx.h"
#include "SubSpace.h"


SubSpace::SubSpace(void)
{
}


SubSpace::~SubSpace(void)
{
}

 void SubSpace::savePCA(String fileName, Mat eigenVector, Mat mean){
	FileStorage fs;
	fs.open(fileName, FileStorage::WRITE);
	fs << "eigenVector"<<eigenVector;
	fs << "mean"<<mean;
	fs.release();

}
 void SubSpace::loadPCA(String fileName, Mat &eigenVector, Mat &mean){
	FileStorage fs;
	fs.open(fileName, FileStorage::READ);
	
	fs["eigenVector"] >> eigenVector;
	fs["mean"] >> mean;
}
