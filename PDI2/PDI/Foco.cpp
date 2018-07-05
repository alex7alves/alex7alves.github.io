
/* Programa para realçar parte de uma imagem
   deixando o resto como plano de fundo

   Autor : Alex Alves
*/   


#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;

void Alocar(int **x, int **x2);
void setValores( int *foco_inicio_slider,int *foco_fim_slider);
void TiltShift(int * x1, int *x2, Mat image,Mat borrado,Mat matriz);
void on_inicio(int, void*);
void on_fim(int, void*);


int main(int argc, char** argv){
    int  *foco_inicio_slider,*foco_fim_slider;
    Mat image,saida,blur,fim,fundo;
    image = imread(argv[1]);
    fundo = imread(argv[2]);
    Mat matriz(image.rows,image.cols,image.type());
    float alfa=atoi(argv[3]);
    float gama = atoi(argv[4]);
    GaussianBlur(image,blur, Size(3,3),5);
    addWeighted( image,alfa, blur, 1-alfa, gama,fim);

    Alocar(&foco_inicio_slider,&foco_fim_slider);

    setValores(foco_inicio_slider,foco_fim_slider);
     
    namedWindow("Foco", 1);

    createTrackbar( "Inicio ", "Foco",foco_inicio_slider,image.rows-30,on_inicio );
    createTrackbar( "Fim", "Foco",foco_fim_slider,image.rows,on_fim );
      
    while(1){

        on_inicio(*foco_inicio_slider, 0 );
        on_fim(*foco_fim_slider, 0 );

        if(*foco_fim_slider>=*foco_inicio_slider ){
          TiltShift(foco_inicio_slider ,foco_fim_slider,fim,fundo,matriz);
        }else{
          TiltShift(foco_fim_slider,foco_inicio_slider,fim,fundo,matriz);
        }
        if(waitKey(20)==27){
          imwrite("Foco.jpg",matriz);
          break;
        }
        imshow("Foco",matriz);
       
    }
    waitKey(0);
    return 0;
}

void Alocar(int **x, int **x2){
    *x = new int;
    *x2 = new int;
}
void setValores( int *foco_inicio_slider,int *foco_fim_slider){
    *foco_inicio_slider=0;
    *foco_fim_slider=200;
}

void TiltShift(int * x1, int *x2, Mat image,Mat borrado,Mat matriz){

    for(int i=0;i<image.rows;i++){
        for(int j=0;j<image.cols;j++){
            if(i>=*x1 && i<=*x2){
                matriz.at<float>(i,j)= image.at<float>(i,j);

            }else {
                matriz.at<float>(i,j)= borrado.at<float>(i,j);

            }    
        }
    }
}
// Funcoes vazias mas servem para direcionar os ponteiros

void on_inicio(int, void*){
}
void on_fim(int, void*){
}