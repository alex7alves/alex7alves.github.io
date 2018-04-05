
/* Programa para equalizar o histograma
   de uma imagem em tons de cinza

   Autor : Alex Alves
*/   


#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){

    Mat image,cinza,equalizar;
    int width, height;
    VideoCapture cap;
    bool uniform = true,acummulate = false;
    int nbins = 256;
    float range[] = {0, 256};
    const float *histrange = { range };
    cap.open(0);
    
    if(!cap.isOpened()){
        printf("cameras indisponiveis\n");
        return -1;
    }
    width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    cout << "largura = " << width << endl;
    cout << "altura  = " << height << endl;
    while(1){
        cap >> image;
        cvtColor(image,cinza,CV_BGR2GRAY);

        // Calculando o histograma para tons de cinzas
        calcHist(&cinza, 1, 0, Mat(),equalizar, 1,&nbins, &histrange,uniform,acummulate);

        // Equalizando o histograma
        equalizeHist(cinza,equalizar);
        imshow("Cinza",cinza);
        imshow("Equalizar",equalizar);
        waitKey(30);
    }
    return 0;
}
