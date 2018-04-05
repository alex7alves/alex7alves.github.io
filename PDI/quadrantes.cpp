
/*
    Programa para mudar os quadrantes de uma imagem

    Autor : Alex Alves
*/

#include <iostream>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    Mat image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
    Mat matriz(image.rows,image.cols,image.type());


    int ql =image.rows/2,qc=image.cols/2;
    for(int i=0; i<ql ;i++)
    {
        for(int j=0; j<qc ; j++)
        {
            matriz.at<uchar>(i,j)=image.at<uchar>(i+ql,j+qc);
            matriz.at<uchar>(i+ql,j+qc)=image.at<uchar>(i,j);
            matriz.at<uchar>(i,j+qc)=image.at<uchar>(i+ql,j);
            matriz.at<uchar>(i+ql,j)=image.at<uchar>(i,j+qc);
        }
    }
    imshow(" Original ",image);
    imshow("Modificada",matriz);
    imwrite("quadrantes.png",matriz);
    waitKey();
    return 0;
}
