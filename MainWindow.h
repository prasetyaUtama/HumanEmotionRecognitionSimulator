#pragma once
#include <gtk/gtk.h>
#include "cairo.h"
#include "SubSpace.h"
#include <iostream>
#include "kNNC.h"
//MWidgets = Main Window Widgets
typedef struct  _Widgets
{
	GtkWidget *about;
	GtkWidget *aboutDialog;
	GtkWidget *fileTestingDialog;
	GtkWidget *fileTrainingDialog;
	GtkWidget *window;
	GtkWidget *image;
	GtkWidget *mainPanel;
	GtkWidget *emotionLabel;
} MWidgets;
class MainWindow
{

public:
	MainWindow::MainWindow(int argc,char *argv[]);

	MainWindow(void);
	~MainWindow(void);



};

