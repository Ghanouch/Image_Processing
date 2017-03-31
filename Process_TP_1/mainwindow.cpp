#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"

using namespace cv;
using namespace std;


vector<Mat> calculeHistograme(Mat source) {
    int const histTaille = 256;

    // Creation des Matrices
    Mat histogrammeVert (histTaille, 1, CV_32FC1, Scalar(0));
    Mat histogrammeBleu (histTaille, 1, CV_32FC1, Scalar(0));
    Mat histogrammeRouge(histTaille, 1, CV_32FC1, Scalar(0));

    Vec3b pixel;
    // Parcours de la Matrice
    for (int i = 0; i < source.rows; i++) {
        for (int j = 0; j < source.cols; j++) {
            // pixel contient la valeur de chaque couleur
            pixel = source.at<Vec3b>(i, j);
            int valeurBleue = (uchar)pixel.val[0];
            int valeurVerte = (uchar)pixel.val[1];
            int valeurRouge = (uchar)pixel.val[2];

            // Ajouter 1 pour la meme correspondance
            histogrammeBleu.at<float> (valeurBleue, 0) += 1;
            histogrammeVert.at<float> (valeurVerte, 0) += 1;
            histogrammeRouge.at<float>(valeurRouge, 0) += 1;
        }
    }
    // Ajouter les 3 Matrice à un tableau des matrices
    vector<Mat> listeHistogrammes;
    listeHistogrammes.push_back(histogrammeRouge);
    listeHistogrammes.push_back(histogrammeVert);
    listeHistogrammes.push_back(histogrammeBleu);

    return listeHistogrammes;
}

void dessineHistograme(vector<Mat> histogramme , Mat source) {

    split(source, histogramme);
    int histTaille = 256;
    float range[] = {0, 256};
    const float* histRange = {range};
    Mat histoRouge, histoVert, histoBleu;
    calcHist(&histogramme[0], 1, 0, Mat(), histoBleu, 1, &histTaille, &histRange, true, false);
    calcHist(&histogramme[1], 1, 0, Mat(), histoVert, 1, &histTaille, &histRange, true, false);
    calcHist(&histogramme[2], 1, 0, Mat(), histoRouge, 1, &histTaille, &histRange, true, false);


    //Compare Hito
    //compare2Histo(histoBleu, histoRouge);


    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histTaille );
    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    normalize(histoBleu, histoBleu, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(histoVert, histoVert, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(histoRouge, histoRouge, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    for( int i = 1; i < histTaille; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(histoBleu.at<float>(i-1)) ) ,
              Point( bin_w*(i), hist_h - cvRound(histoBleu.at<float>(i)) ),
              Scalar( 255, 0, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(histoVert.at<float>(i-1)) ) ,
              Point( bin_w*(i), hist_h - cvRound(histoVert.at<float>(i)) ),
              Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(histoRouge.at<float>(i-1)) ) ,
              Point( bin_w*(i), hist_h - cvRound(histoRouge.at<float>(i)) ),
              Scalar( 0, 0, 255), 2, 8, 0  );
    }
    namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
    imshow("calcHist Demo", histImage );
}


void moyenne(Mat source)
{
    int Somme[3] = { 0 , 0 , 0} ;
    int k;
    Vec3b pixel ;
    for(int i=0 ; i<source.rows ; i++)
    {
        for(int j=0 ; j<source.cols ; j++)
        {
            pixel = source.at<Vec3b>(i,j);
            k = 0 ;
            while(k<3)
            {
             Somme[k] += pixel.val[k];
             k++;
            }

        }
    }

    int Dimension = source.cols * source.rows;

    cout << " ___________ Affichage des moyennes ____________" << endl ;
    cout << "Moyenne du bleu "   << Somme[0]/Dimension  << endl ;
    cout << "Moyenne du rouge "  << Somme[1]/Dimension  << endl ;
    cout << "Moyenne du vert "   << Somme[2]/Dimension  << endl ;


}


Mat ImgToGris(Mat source)
{
    Mat dest(source.rows , source.cols , CV_8UC1 ,Scalar(0));
    Vec3b pixel ;
    for(int i=0 ; i<source.rows ; i++)
    {
        for(int j=0 ; j<source.cols ; j++)
        {
            pixel = source.at<Vec3b>(i,j);
            dest.at<uchar>(i,j) =(uchar) (pixel.val[0] + pixel.val[1] + pixel.val[2]) / 3 ;
        }
    }

    return dest ;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Mat source = imread("/home/issam/TP VPN/Process TP/Process_TP_1/index.jpeg", 1);
    if(source.empty())
        cout << "impossible d ouvrir l image "<<endl;
    else
    {
        imshow("image dest", source);
        cout << " Nombre de colonne :" << source.cols << endl ;
        cout << " Nombre de ligne :" << source.rows <<endl;

        // Calcul de la moyenne de chaque couleur
        moyenne(source);

        // Récuperer les Histo de chaque couleur
        vector<Mat> Hist = calculeHistograme(source);

        // Dessiner les Histo de chaque couleurs
        dessineHistograme(Hist,source);


        // Afficher la'image  en Gris
        imshow("Image en Gris " , ImgToGris(source));


    }


}

MainWindow::~MainWindow()
{
    delete ui;
}
