#include "stdafx.h"
#include "MainWindow.h"
#include "Features.h"
#include "DataSet.h"
#include "Network.h"
#include "ELM.h"
#include "SigmoidActivationFunction.h"
#include "TanhActivationFunction.h"
#include "opencv2/opencv.hpp"
#include <time.h>

using namespace cv;




int numDesired = 7;
int numInput = 132;
int numFeature = 132;


void getLabel(int label, Mat& dst);
String setLabel(Mat output);
int getLabel(Mat output);
MainWindow::MainWindow(int argc,char *argv[])
{
	GtkBuilder *builder; 
	GtkWidget *window;
	MWidgets mWidgets; 


	gtk_init (&argc, &argv);
	builder = gtk_builder_new ();

	gtk_builder_add_from_file (builder, "layout/HER.xml", NULL);
	window = GTK_WIDGET (gtk_builder_get_object (builder, "mainWindow"));
	mWidgets.about = GTK_WIDGET( gtk_builder_get_object( builder, "about" ) );
	mWidgets.aboutDialog = GTK_WIDGET( gtk_builder_get_object( builder, "aboutdialog" ) );
	mWidgets.fileTestingDialog = GTK_WIDGET(gtk_builder_get_object(builder,"testingFileChoose"));
	mWidgets.fileTrainingDialog =  GTK_WIDGET(gtk_builder_get_object(builder,"trainingFileChoose"));
	mWidgets.mainPanel = GTK_WIDGET(gtk_builder_get_object(builder,"fixed"));
	mWidgets.image = GTK_WIDGET(gtk_builder_get_object(builder,"faceImage"));
	mWidgets.emotionLabel = GTK_WIDGET(gtk_builder_get_object(builder,"emotionLabel"));	
	gtk_builder_connect_signals (builder, &mWidgets);

	//destroy builder
	g_object_unref (G_OBJECT (builder));

	gtk_widget_show_all (window); 

	gtk_main ();
}

MainWindow::MainWindow(void)
{
}
MainWindow::~MainWindow(void)
{
}

//handlers
extern "C" G_MODULE_EXPORT void on_about_activate(GtkImageMenuItem *about, MWidgets* widgets)
{
	gtk_dialog_run(GTK_DIALOG( widgets->aboutDialog ));
	gtk_widget_hide( widgets->aboutDialog );

}
extern "C" G_MODULE_EXPORT void on_exit_activate(GtkImageMenuItem *exit, MWidgets* widgets)
{
	gtk_main_quit();
}
extern "C" G_MODULE_EXPORT void on_trainingNetwork_activate(GtkImageMenuItem *exit, MWidgets* widgets)
{

	if(gtk_dialog_run(GTK_DIALOG( widgets->fileTrainingDialog )) == -4)
	{
		char *filename;
		String imgSrc;
		int label;

		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (widgets->fileTrainingDialog));

		FileStorage fs(filename, FileStorage::READ);
		int numData = (int)fs["numData"];
		int type = (int)fs["type"];
		Mat inputNetwork(numInput,numData,CV_64F);
		Mat feature(numData,1764,CV_64F);

		Mat desiredOutputNetwork(numDesired,numData,CV_64F);

		double c;
		double hidden;



		if(type ==  1){

			FileNode dataTrain = fs["dataTrain"];
			FileNodeIterator it = dataTrain.begin(), it_end = dataTrain.end();
			int idx = 0;
			Features features = Features();
			Mat featuresTrain (1,1764,CV_64F);

			Mat desiredOutput(numDesired,1,CV_64F);


			for( ; it != it_end; ++it, idx++ )
			{

				imgSrc = (String) (*it)["image"];
				label = (int) (*it)["label"];
				cout << imgSrc << "\n";

				if (features.getFeatures(imgSrc, featuresTrain) ==  true)
				{

					//	cout << featuresTrain.cols << " " << feature.cols;
					featuresTrain.row(0).copyTo(feature.row(idx));


					//DESIRED OUTPUT
					getLabel(label,desiredOutput);
					desiredOutput.col(0).copyTo(desiredOutputNetwork.col(idx));



				}
				else
				{
					feature.row(20).copyTo(feature.row(idx));
					//cout << imgSrc << "\n";
				}
			}

			PCA extractFeature(feature,Mat(), CV_PCA_DATA_AS_ROW, numFeature);

			Mat featureR;

			for(int i = 0; i < numData;i++)
			{
				extractFeature.project(feature.row(i),featureR);
				featureR = featureR.t();			
				featureR.col(0).copyTo(inputNetwork.col(i));


			}


			SubSpace::savePCA("data/PCA.xml",extractFeature.eigenvectors, extractFeature.mean);
		}
		else
		{

			fs["Input"] >> inputNetwork;
			fs["Output"] >> desiredOutputNetwork;
		}





		DataSet dataSet = DataSet(inputNetwork,desiredOutputNetwork);



		dataSet.saveDataSet("data/dataSet.yml");
		ActivationFunction* sig;
		ActivationFunction* tanh;
		SigmoidActivationFunction sigAc = SigmoidActivationFunction();		
		TanhActivationFunction tanhAc =  TanhActivationFunction();
		sig = &sigAc;
		tanh = &tanhAc;


		cout << "input hidden:";
		cin  >>hidden;

		cout << "input c:";
		cin  >>c;

		Network network = Network(dataSet,hidden);



		network.setHiddenActivation(sig);
		//network.setOutputActivation(tanh);
		ELM elm = ELM(&network);


		clock_t tStart = clock();

		elm.train(pow(2,c));
		cout <<"error = "<<elm.getError()<<"\n\n";
		//cout << dataSet.getDesiredOutput().col(550) << dataSet.getDesiredOutput().cols<<" " << dataSet.getDesiredOutput().rows;

		double timeExecution = (double)(clock() - tStart)/CLOCKS_PER_SEC;
		cout << "training time = "<< timeExecution<<"\n\n";
		stringstream file;
		file << "data/Train"<<c<<"_"<<hidden<<".xml";
		network.saveNetwork(file.str());







	}

	gtk_widget_hide (widgets->fileTrainingDialog);

}

extern "C"	G_MODULE_EXPORT void on_chooseTestingFile_activate(GtkMenuItem *exit, MWidgets* widgets)
{
	if(gtk_dialog_run(GTK_DIALOG( widgets->fileTestingDialog )) == GTK_RESPONSE_ACCEPT)
	{





		ActivationFunction* sig;
		ActivationFunction* tanh;
		SigmoidActivationFunction sigAc = SigmoidActivationFunction();		
		TanhActivationFunction tanhAc =  TanhActivationFunction();
		sig = &sigAc;
		tanh = &tanhAc;

		char *filename;
		Mat inputNetwork(numInput,1,CV_64F);
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (widgets->fileTestingDialog));
		gtk_image_set_from_file (GTK_IMAGE (widgets->image), filename);
		Features features = Features();

		//load PCA subspace
		Mat eigv, eigm;
		Mat featuresTrain (1,1764,CV_64F);
		Mat featureR (numFeature,1,CV_64F);


		SubSpace::loadPCA("data/PCA.xml",eigv, eigm);
		//end of load
		features.getFeatures(filename,featuresTrain);
		//project to PCA subspace
		featuresTrain.convertTo(featuresTrain,CV_64F);

		featureR = eigv * (featuresTrain.row(0).t() -  eigm.row(0).t());

		featureR.col(0).copyTo(inputNetwork.col(0));
		

		DataSet testData = DataSet(inputNetwork);
		Network network=Network(testData);
		network.loadNetwork("data/Train.xml");
		network.setHiddenActivation(sig);
		//network.setOutputActivation(sig);
		ELM elm = ELM(&network);
		elm.test();
		cout << network.getOutput()<<"\n";
		gtk_label_set_text(GTK_LABEL(widgets->emotionLabel),setLabel(network.getOutput()).c_str());

	}

	gtk_widget_hide (widgets->fileTestingDialog);
}



extern "C"	G_MODULE_EXPORT void on_chooseTestingFile_raw_activate(GtkMenuItem *exit, MWidgets* widgets)
{

	String imgSrc;
	char *filename;
	int label;

	if(gtk_dialog_run(GTK_DIALOG( widgets->fileTrainingDialog )) == -4)
	{

		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (widgets->fileTrainingDialog));
		FileStorage fs(filename, FileStorage::READ);

		ActivationFunction* sig;
		ActivationFunction* tanh;
		SigmoidActivationFunction sigAc = SigmoidActivationFunction();		
		TanhActivationFunction tanhAc =  TanhActivationFunction();
		sig = &sigAc;
		tanh = &tanhAc;

		char *filename;
		Mat inputNetwork(numInput,1,CV_64F);
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (widgets->fileTestingDialog));
		Features features = Features();
		FileNode dataTrain = fs["dataTest"];
		int numData = (int)fs["numData"];
		FileNodeIterator it = dataTrain.begin(), it_end = dataTrain.end();
		int idx = 0;
		int result;
		int tempLabel = 1;
		int numE = 1;

		//Test rate
		double avgReslt = 0.0;
		double emReslt = 0.0;

		//Miss Classification
		double angry = 0.0;
		double fear = 0.0;
		double disgust = 0.0;
		double happy = 0.0;
		double neutral = 0.0;
		double sad = 0.0;
		double surprise = 0.0;
		double sumMiss = 0.0;


		//RMSE
		Mat desiredOutput(7,1,CV_64F);
		Mat ePow = Mat::zeros(7,1,CV_64F);
		double eSum = 0.0;
		double rmse;


		//DEV
		double dev = 0.0;

		//Load PCA Subspace
		Mat eigv, eigm;
		Mat featuresTrain (1,1764,CV_64F);
		Mat featureR (numFeature,1,CV_64F);


		SubSpace::loadPCA("data/PCA.xml",eigv, eigm);


		double c[7] = {25.0};
		int hidden[7] = {1000};

		for(int k = 0; k < 1;k++)
		{
			for (int l = 0; l < 1;l++){
				stringstream networkTrain;
				networkTrain  << "data/Train"<<c[l]<<"_"<<hidden[k]<<".xml";
				//cout << networkTrain.str() << "\n";
				it = dataTrain.begin(), it_end = dataTrain.end();
				for( ; it != it_end; ++it, idx++ )
				{

					imgSrc = (String) (*it)["image"];
					label = (int) (*it)["label"];
					features.getFeatures(imgSrc,featuresTrain);

					//project to PCA subspace
					featuresTrain.convertTo(featuresTrain,CV_64F);

					featureR = eigv * (featuresTrain.row(0).t() -  eigm.row(0).t());

					featureR.col(0).copyTo(inputNetwork.col(0));


					DataSet testData = DataSet(inputNetwork);
					Network network=Network(testData);					
					network.loadNetwork(networkTrain.str());
					network.setHiddenActivation(sig);
					network.setOutputActivation(&ActivationFunction());

					ELM elm = ELM(&network);
					elm.test();


					//get Result
					result = getLabel(network.getOutput());


					//get Desired
					getLabel (label,desiredOutput);

					//cout << imgSrc<< " " << setLabel(network.getOutput())<<"\n";


					ePow = desiredOutput - network.getOutput().t();


					eSum = 0.0;

					for(int i = 0; i <= 7;i++)
					{
						eSum += pow(ePow.at<double>(i,0),2);

					}




					if(label == result)
					{
						avgReslt += 1.0;
						emReslt += 1.0;
					}
					else
					{
						if(result == 1){
							angry +=1.0;
						}
						if(result == 2){
							disgust +=1.0;
						}
						if(result == 3){
							fear +=1.0;
						}
						if(result == 4){
							happy +=1.0;
						}
						if(result == 5){
							neutral +=1.0;
						}
						if(result == 6){
							sad +=1.0;
						}
						if(result == 7){
							surprise +=1.0;
						}
					}
					if(tempLabel != label)
					{

						sumMiss += (angry + fear + disgust+happy+neutral+sad+surprise)/numE;
						cout << "\n\n";
						cout << "angryMiss : "<< angry/numE<<"\n";				
						cout << "disgustMiss : "<< disgust/numE<<"\n";
						cout << "fearMiss : "<< fear/numE<<"\n";
						cout << "happyMiss : "<< happy/numE<<"\n";
						cout << "neutralMiss : "<< neutral/numE<<"\n";
						cout << "sadMiss : "<< sad/numE<<"\n";
						cout << "surpriseMiss : "<< surprise/numE<<"\n";
						cout << "sumMiss : "<< sumMiss<<"\n";
						cout << "recognition rate: "<<emReslt/numE<<"\n";
						cout << "----------------------------------------------";

						angry = 0.0;
						fear = 0.0;
						disgust = 0.0;
						happy = 0.0;
						neutral = 0.0;
						sad = 0.0;
						surprise = 0.0;
						numE = 0;
						emReslt = 0.0;

					}
					numE++;
					tempLabel = label;

					rmse += eSum;
				}
				cout << "\n\n";
				cout << "angryMiss : "<< angry/(numE-1)<<"\n";
				cout << "fearMiss : "<< fear/(numE-1)<<"\n";
				cout << "disgustMiss : "<< disgust/(numE-1)<<"\n";
				cout << "happyMiss : "<< happy/(numE-1)<<"\n";
				cout << "neutralMiss : "<< neutral/(numE-1)<<"\n";
				cout << "sadMiss : "<< sad/(numE-1)<<"\n";
				cout << "surpriseMiss : "<< surprise/(numE-1)<<"\n";
				cout << "recognition rate: "<<emReslt/(numE-1)<<"\n";
				cout << "nume" << numE <<" " << " " <<label << " " << tempLabel;

				cout << "--------------------------------------------------------";
				cout << "\n"<<"Test Rate = "<<avgReslt/numData;
				cout << "\n"<<"RMSE= "<<sqrt(rmse/(numData*7));
				cout << "\n"<<"C = "<<c[l];
				cout << "\n"<<"L = "<<hidden[k];
				cout << "\n";
				cout << "--------------------------------------------------------";
				avgReslt = 0.0;
				rmse = 0.0;

			}}


	


	}



	gtk_widget_hide (widgets->fileTrainingDialog);

}
extern "C"	G_MODULE_EXPORT void on_mainWindow_destroy(MWidgets* widgets)
{
	gtk_main_quit();
}


String setLabel(Mat output)
{
	//Max Min value
	double minVal; 
	double maxVal; 
	Point minLoc; 
	Point maxLoc;
	minMaxLoc( output, &minVal, &maxVal, &minLoc, &maxLoc );
	String emotions[] = {"Angry","Disgust","Fear","Happy","Neutral","Sad","Surprise"};

	return emotions[maxLoc.x];
}

int getLabel(Mat output)
{
	//Max Min value
	double minVal; 
	double maxVal; 
	Point minLoc; 
	Point maxLoc;
	minMaxLoc( output, &minVal, &maxVal, &minLoc, &maxLoc );
	String emotions[] = {"Angry","Disgust","Fear","Happy","Neutral","Sad","Surprise"};

	return maxLoc.x + 1;
}
void getLabel(int label, Mat& dst)
{
	/* Description 
	1 = angry;
	2 = disgust;
	3 = fear;
	4 = happy;
	5 = neutral;
	6 = sad;
	7 = surprise;*/


	Mat desiredResponses;
	if(label == 1){
		desiredResponses = (Mat_<double>(7,1)<<1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0);
	}
	if(label == 2){
		desiredResponses = (Mat_<double>(7,1)<<-1.0,1.0,-1.0,-1.0,-1.0,-1.0,-1.0);
	}

	if(label == 3){
		desiredResponses = (Mat_<double>(7,1)<<-1.0,-1.0,1.0,-1.0,-1.0,-1.0,-1.0);
	}

	if(label == 4){
		desiredResponses = (Mat_<double>(7,1)<<-1.0,-1.0,-1.0,1.0,-1.0,-1.0,-1.0);
	}

	if(label == 5){
		desiredResponses = (Mat_<double>(7,1)<<-1.0,-1.0,-1.0,-1.0,1.0,-1.0,-1.0);
	}

	if(label == 6){
		desiredResponses = (Mat_<double>(7,1)<<-1.0,-1.0,-1.0,-1.0,-1.0,1.0,-1.0);
	}

	if(label == 7){
		desiredResponses = (Mat_<double>(7,1)<<-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,1.0);
	}
	dst = desiredResponses.clone();

}
