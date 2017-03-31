#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Mat source = imread("/home/issam/TP VPN/Process TP/Process_TP_1/index.jpeg", 1);
    if(source.empty())
        cout << "impossible d ouvrir l image "<<endl;
    else
        imshow("image dest", source);
}

MainWindow::~MainWindow()
{
    delete ui;
}
