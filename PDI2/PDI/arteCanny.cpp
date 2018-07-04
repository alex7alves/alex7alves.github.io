
/* 
	Programa para fazer arte usando 
	algoritmo de Canny junto com pontilhismo


*/


#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;


vector<int> AmostrarCentros(vector<int> y, int passo);
void Circulo(Mat im, Mat art, Vec3b cor, int raio,int x,int y );


int main(int argc, char** argv){
  vector<int> yrange;
  vector<int> xrange;

  Mat image, frame, points,cinza,borrar,bordas;
  int fator =60;

  int x, y;
  int STEP=5,JITTER=3,RAIO=3; 
  image = imread(argv[1]);
  Mat  arte(image.rows,image.cols,image.type());
  Vec3b cor;

  srand(time(0));
  
  if(!image.data){
	  cout << "nao abriu" << argv[1] << endl;
    cout << argv[0] << " imagem.jpg";
    exit(0);
  }

  xrange.resize(image.rows/STEP);
  yrange.resize(image.cols/STEP);
  
  iota(xrange.begin(), xrange.end(), 0); 
  iota(yrange.begin(), yrange.end(), 0);


  xrange=AmostrarCentros(xrange,STEP);
  yrange=AmostrarCentros(yrange,STEP);
 
  random_shuffle(xrange.begin(), xrange.end());
  
  for(auto i : xrange){
    random_shuffle(yrange.begin(), yrange.end());
    for(auto j : yrange){
      x = i+rand()%(2*JITTER)-JITTER+1;
      y = j+rand()%(2*JITTER)-JITTER+1;
      Circulo(image,arte,cor,RAIO,x,y);
    }
  }
  imwrite("pontos.jpg",arte);

  cvtColor(arte, cinza, CV_BGR2GRAY);
  GaussianBlur(cinza, borrar, Size(5,5), 25, 25);
  Canny(borrar, bordas,fator,3*fator);
  imwrite("canny.jpg",bordas);

  for(int i=0;i<arte.rows;i++){
    for(int j=0;j<arte.cols;j++){
      if(bordas.at<uchar>(i,j)==255){
        Circulo(arte,arte,cor,2,i,j);
      }
    }
  }

  imwrite("arte.jpg",arte);
  imshow("arte",arte);
  waitKey();
  return 0;
    
}

vector<int> AmostrarCentros(vector<int> y, int passo)
{
  for(uint i=0; i<y.size(); i++){
    y[i]= y[i]*passo+passo/2;
  }
  return y;
}

void Circulo(Mat im, Mat art, Vec3b cor, int raio,int x,int y )
{
  cor = im.at<Vec3b>(x,y);
  circle(art,cv::Point(y,x),raio,CV_RGB(cor[2],cor[1],cor[0]),-1, CV_AA);
}
