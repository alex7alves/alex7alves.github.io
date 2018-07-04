
/*
    Programa para fazer o negativo entre dois pontos
    em uma imagem

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

void AlocarPonto(int **x, int **y);
void Ponto(int *x,int *y, int n);
void Verifica(int *x1,int *x2);
void isDentro(int *x1,int *x2, Mat image);

int main(int argc, char** argv)
{
    Mat image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);

    int *x1,*y1,*x2,*y2;

    AlocarPonto(&x1,&y1);
    AlocarPonto(&x2,&y2);
    Ponto(x1,y1,1);
    isDentro(x1,y1,image);
    Ponto(x2,y2,2);
    isDentro(x1,y1,image);

    Verifica(x1,x2);
    Verifica(y1,y2);
    imshow(" Original ",image);

    for(int i=*y1; i<*y2 ;i++)
    {
        for(int j=*x1; j<*x2 ; j++)
        {
          image.at<uchar>(i,j) = 255 -image.at<uchar>(i,j) ;
        }
    }

    imshow("Modificada",image);
    imwrite("negativo.png", image);
    waitKey();
    return 0;
}

void AlocarPonto(int **x, int **y)
{
    *x = new int;
    *y = new int;
}

void Ponto(int *x,int *y, int n)
{
    printf("Digite o ponto %d \n",n);
    cin >> *x >> *y;
}
void Verifica(int *x1,int *x2)
{
    int aux;
    if(*x2 < *x1){
        aux = *x1;
        *x1= *x2;
        *x2 = aux;
    }
}
void isDentro(int *x1,int *x2, Mat image){
    if(*x1<0 || *x1> image.rows){
        printf("Cordenada da linha invalida\n");
        exit(-1);
    }
    if(*x2<0 || *x2> image.cols){
        printf("Cordenada da coluna invalida\n");
        exit(-1);
    }
}
