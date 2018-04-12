
/* Programa para equalizar o histograma
   de uma imagem colorida

   Autor : Alex Alves
*/   


#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;



int main(int argc, char** argv){

    Mat image,saida;
    image = imread(argv[1]);
    vector<Mat> planes;
    split (image, planes);
    equalizeHist(planes[0],planes[0]);
    equalizeHist(planes[1],planes[1]);
    equalizeHist(planes[2],planes[2]);
    merge(planes,saida);
    
    imshow("Saida",image);
    imshow("Equalizar",saida);
    imwrite("equalizado2.jpg",saida);
    waitKey(0);
   
    return 0;
}
