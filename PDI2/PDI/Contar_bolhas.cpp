
/*
  Programa para contar obbjetos e os numero de buracos dentro deles

  Autor : Alex Alves
*/

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
CvPoint ponto(int i, int j);
void Bordas( Mat bolha, int passo_linha,int passo_coluna);

int main(int argc, char** argv){

    Mat image;

    int nobjects=0,anterior,soma=0,cont=0;
    int vetorburacos[10];
    CvPoint p;
    image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
    Mat bolha(image.rows,image.cols,image.type());
    if(!image.data){
        std::cout << "imagem nao carregou corretamente\n";
        return(-1);
    }
    for(int j=0; j<10; j++){
        vetorburacos[j]=0; 
    }
    // copiando imagem original
    for(int i=0; i<image.rows; i++){
        for(int j=0; j<image.cols; j++){
            bolha.at<uchar>(i,j) = image.at<uchar>(i,j);  
        }
    }
    // Retirando bolhas que tocam as bordas nas linhas
    Bordas(bolha,bolha.rows-1,1);
    // Retirando bolhas que tocam as bordas nas colunas
    Bordas(bolha,1,bolha.cols-1);
    imshow("Sem borda",bolha);
     
    // busca objetos  presentes
    for(int i=0; i<bolha.rows; i++){
        for(int j=0; j<bolha.cols; j++){
            if(bolha.at<uchar>(i,j)==255){
                nobjects++;
                p=ponto(i,j);
                floodFill(bolha,p,1);
            }
        }
    }
    p=ponto(0,0);
    floodFill(bolha,p,255);
    // busca objetos com buracos presentes
    for(int i=0; i<bolha.rows; i++){
        for(int j=0; j<bolha.cols; j++){
            if(bolha.at<uchar>(i,j)==0){
                anterior = bolha.at<uchar>(i,j-1);
                if(anterior!=0 && anterior !=255 ){
                    vetorburacos[anterior] =vetorburacos[anterior] +1;
                    // se encontrou um buraco pinta com n+1 para 
                    // proxima vez se tiver outro buraco
                    p=ponto(i,j);
                    anterior++;
                    floodFill(bolha,p,anterior);
                    // pinta o anterior tambem
                    p=ponto(i,j-1);
                    floodFill(bolha,p,anterior);

                }

            }
        }
    }
    // Ajustando pois todo objeto que tem 3 buracos 
    // ja foi contabilizado no 1 e no 2 também ai tem que tirar essa diferenca
    for(int k=8; k>=1; k--){
        soma = soma + vetorburacos[k+1];
        vetorburacos[k] = vetorburacos[k] -soma;
        cont = cont+vetorburacos[k];
    }
    vetorburacos[0]= nobjects -cont;
    printf("Foram encontrados %d objetos dos quais :\n",nobjects);
    for(int k=0; k<10; k++){
        printf(" %d objetos com %d buracos \n",vetorburacos[k],k);
    }   
    imshow("image", image);
    imshow("Modificado",bolha);
    //imwrite("labeling.png", image);
    waitKey();
    return 0;
}

CvPoint ponto(int i, int j)
{
    CvPoint p;
    p.x=j;
    p.y=i;
    return p;
}

void Bordas( Mat bolha, int passo_linha,int passo_coluna)
{
    CvPoint p;
    for(int i=0; i<bolha.rows; i = i+passo_linha){
        for(int j=0; j<bolha.cols; j= j+passo_coluna){
            if(bolha.at<uchar>(i,j)==255){
                p=ponto(i,j);
                floodFill(bolha,p,0);
            }
        }
    }
}
