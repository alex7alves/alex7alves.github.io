
/* 
    Programa para detectar movimento 

    Autor : Alex Alves
*/
#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){

    Mat image;
    int width, height;
    VideoCapture cap;
    vector<Mat> planes;
    Mat histG, histG2;
    int nbins = 64;
    float range[] = {0, 256};
    const float *histrange = { range };
    bool uniform = true;
    bool acummulate = false;
    cap.open(0);
      
    if(!cap.isOpened()){
        cout << "cameras indisponiveis";
        return -1;
    }
      
    width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    cout << "largura = " << width << endl;
    cout << "altura  = " << height << endl;

    int histw = nbins, histh = nbins/2;
    Mat histImgG(histh, histw, CV_8UC3, Scalar(0,0,0));

    while(1){
        cap >> image;
        split (image, planes);
     
        calcHist(&planes[1], 1, 0, Mat(), histG, 1,
                 &nbins, &histrange,
                 uniform, acummulate);

        normalize(histG, histG, 0, histImgG.rows, NORM_MINMAX, -1, Mat());

        if( histG2.data && compareHist(histG,histG2,CV_COMP_CHISQR)>20){
            // Se houve movimento escreve mensagem na imagem
            putText(image,"Ocorreu um movimento !!!",cvPoint(50,height-20),FONT_HERSHEY_SCRIPT_COMPLEX,
                    1,cvScalar(255,0,20),1,8);
        }
           
        for(int i=0; i<nbins; i++){

          line(histImgG,
               Point(i, histh),
               Point(i, histh-cvRound(histG.at<float>(i))),
               Scalar(0, 255, 0), 1, 8, 0);
        }
        histG2 =histG.clone();
        imshow("image", image);
        waitKey(30);
    }
    return 0;
}
