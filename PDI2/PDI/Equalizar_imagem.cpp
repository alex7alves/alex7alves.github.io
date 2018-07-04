
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
   // image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
    image = imread("img2.jpg");
   /* VideoCapture cap;
    bool uniform = true,acummulate = false;
    int nbins = 256;
    float range[] = {0, 256};
    const float *histrange = { range };
*/
    cvtColor(image,cinza,CV_BGR2GRAY);

    // Calculando o histograma para tons de cinzas
   // calcHist(&cinza, 1, 0, Mat(),equalizar, 1,&nbins, &histrange,uniform,acummulate);

    // Equalizando o histograma
    equalizeHist(cinza,equalizar);
    imshow("Cinza",cinza);
    imshow("Equalizar",equalizar);
    imwrite("original.jpg", cinza);
    imwrite("equalizado.jpg",equalizar);
    waitKey();
   
    return 0;
}
