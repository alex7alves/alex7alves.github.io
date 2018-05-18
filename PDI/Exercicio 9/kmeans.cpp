#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <string>
#include <iostream>
using namespace std;
using namespace cv;

int main( int argc, char** argv ){
  int nClusters = 6;
  Mat rotulos;
  int nRodadas = 1;
  Mat centros;
  int cont=1;

  
  Mat img = imread( argv[1], CV_LOAD_IMAGE_COLOR);
  Mat samples(img.rows * img.cols, 3, CV_32F);
  while(cont <=10){
    for( int y = 0; y < img.rows; y++ ){
      for( int x = 0; x < img.cols; x++ ){
        for( int z = 0; z < 3; z++){
          samples.at<float>(y + x*img.rows, z) = img.at<Vec3b>(y,x)[z];
        }
      }
    }
    
    kmeans(samples,
       nClusters,
       rotulos,
       TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001),
       nRodadas,
       KMEANS_RANDOM_CENTERS,
       centros );


    Mat rotulada( img.size(), img.type() );
    for( int y = 0; y < img.rows; y++ ){
      for( int x = 0; x < img.cols; x++ ){ 
        int indice = rotulos.at<int>(y + x*img.rows,0);
        rotulada.at<Vec3b>(y,x)[0] = (uchar) centros.at<float>(indice, 0);
        rotulada.at<Vec3b>(y,x)[1] = (uchar) centros.at<float>(indice, 1);
        rotulada.at<Vec3b>(y,x)[2] = (uchar) centros.at<float>(indice, 2);
      }
    }
    stringstream ss;
    ss << "saida" << cont << ".jpg";
    string s = ss.str();
    cont++;

    imshow( "clustered image", rotulada );
    imwrite(s, rotulada);
    waitKey( 1 );
  }
  return 0;
}
