
/*
  Programa para executar um filtro homomorfico
  e reduzir efeitos prejudiciais da luminancia 
  em uma imagem

  Autor : Alex Alves

*/


#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace cv;
using namespace std;

void Alocar(int **x, int **x2);
void setValores(int *gl,int *gh, int *c,int *d0);
void on_gamma_l(int, void*);
void on_gamma_h(int, void*);
void on_c(int, void*);
void on_d0(int, void*);
void deslocaDFT(Mat& image );
void Filtro_Homomorfico(Mat temp, int *gl,int *gh,
                       int *c,int *d0,int dft_M,int  dft_N);




int main(int argvc, char** argv){
  Mat imaginaryInput, complexImage, multsp;
  Mat padded, filter, mag;
  Mat image, imagegray, tmp; 
  Mat_<float> realInput, zeros;
  vector<Mat> planos;
  int *gl,*gh,*d0,*c;

  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  Alocar(&gl,&gh);
  Alocar(&c,&d0);
  setValores(gl,gh,c,d0);

  imshow("original", image);

  // valores ideais dos tamanhos da imagem
  // para calculo da DFT
  int dft_M, dft_N;

  // identifica os tamanhos otimos para
  // calculo do FFT
  dft_M = getOptimalDFTSize(image.rows);
  dft_N = getOptimalDFTSize(image.cols);

  // realiza o padding da imagem
  copyMakeBorder(image, padded, 0,
                 dft_M - image.rows, 0,
                 dft_N - image.cols,
                 BORDER_CONSTANT, Scalar::all(0));

  // parte imaginaria da matriz complexa (preenchida com zeros)
  zeros = Mat_<float>::zeros(padded.size());

  // prepara a matriz complexa para ser preenchida
  complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

  // a função de transferência (filtro frequencial) deve ter o
  // mesmo tamanho e tipo da matriz complexa
  filter = complexImage.clone();

  // cria uma matriz temporária para criar as componentes real
  // e imaginaria do filtro ideal
  tmp = Mat(dft_M, dft_N, CV_32F);

  namedWindow("Filtro", 1);

 
  createTrackbar( "c", "Filtro",c,100,on_c );
  createTrackbar( "d0 ","Filtro",d0,100,on_d0 );
  createTrackbar( "gamma_h", "Filtro",gh,100,on_gamma_h );
  createTrackbar( "gamma_l", "Filtro",gl,100,on_gamma_l );
   

  while(1){
    on_c(*c, 0 );
    on_d0(*d0, 0 );
    on_gamma_h(*gh, 0 );
    on_gamma_l(*gl, 0 );

    
    // realiza o padding da imagem
    copyMakeBorder(image, padded, 0,
                    dft_M - image.rows, 0,
                    dft_N - image.cols,
                    BORDER_CONSTANT, Scalar::all(0));

    // limpa o array de matrizes que vao compor a
    // imagem complexa
    planos.clear();
    // cria a compoente real
    realInput = Mat_<float>(padded);
    // insere as duas componentes no array de matrizes
    planos.push_back(realInput);
    planos.push_back(zeros);

    // combina o array de matrizes em uma unica
    // componente complexa
    merge(planos, complexImage);

    // calcula o dft
    dft(complexImage, complexImage);

    // realiza a troca de quadrantes
    deslocaDFT(complexImage);

    Filtro_Homomorfico(tmp,gl,gh,c,d0,dft_M,dft_N);

    // cria a matriz com as componentes do filtro e junta
    // ambas em uma matriz multicanal complexa
    Mat comps[]= {tmp, tmp};
    merge(comps, 2, filter);
    // aplica o filtro frequencial
    mulSpectrums(complexImage,filter,complexImage,0);

    // troca novamente os quadrantes
    deslocaDFT(complexImage);
 
    // calcula a DFT inversa
    idft(complexImage, complexImage);

    // limpa o array de planos
    planos.clear();

    // separa as partes real e imaginaria da
    // imagem filtrada
    split(complexImage, planos);

    // normaliza a parte real para exibicao
    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
    imshow("Filtro",planos[0]);
 
    if(waitKey(10)== 27 ) break; // esc pressed!
    
  }
  return 0;
}

void Alocar(int **x, int **x2){
  *x = new int;
  *x2 = new int;
}
void setValores(int *gl,int *gh, int *c,int *d0){
  *gl=10;
  *gh=20;
  *c=60;
  *d0=70;
}

// Funcoes vazias mas servem para direcionar os ponteiros
void on_gamma_l(int, void*){
}

void on_gamma_h(int, void*){
}
void on_c(int, void*){
}
void on_d0(int, void*){
}
void deslocaDFT(Mat& image )
{
  Mat tmp, A, B, C, D;

  // se a imagem tiver tamanho impar, recorta a regiao para
  // evitar cópias de tamanho desigual
  image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols/2;
  int cy = image.rows/2;
  
  // reorganiza os quadrantes da transformada
  // A B   ->  D C
  // C D       B A
  A = image(Rect(0, 0, cx, cy));
  B = image(Rect(cx, 0, cx, cy));
  C = image(Rect(0, cy, cx, cy));
  D = image(Rect(cx, cy, cx, cy));

  // A <-> D
  A.copyTo(tmp);  D.copyTo(A);  tmp.copyTo(D);

  // C <-> B
  C.copyTo(tmp);  B.copyTo(C);  tmp.copyTo(B);
}



void Filtro_Homomorfico(Mat temp, int *gl,int *gh, 
                        int *c,int *d0,int dft_M,int  dft_N)
{

  float gl_aux,gh_aux,c_aux,d0_aux,t,t2;
  gl_aux= *gl/10;
  gh_aux = *gh/10;
  c_aux = *c/10;                        
  d0_aux = *d0/10;

  for(int i=0; i < temp.rows; i++){
    for(int j=0; j < temp.cols; j++){
      t = (i-dft_M/2)*(i-dft_M/2)+(j-dft_N/2)*(j-dft_N/2);
      t2=(1.0 - (float)exp(-(c_aux*t/(d0_aux*d0_aux))));
      temp.at<float>(i,j) = (gh_aux-gl_aux)*t2 + gl_aux;
    }
  }

}
