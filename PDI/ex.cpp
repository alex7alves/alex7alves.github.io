#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;


void Colorir( Mat image, int p1,int p2)
{
	for(int i=p1; i<image.rows;i++){
    	for(int j=p2; j<image.cols; j++){
    		image.at<uchar>(i,j)=0; 	
		  }
  	}
}

int main(int argc, char** argv){
  Mat imagem;


  imagem = imread("img.png",CV_LOAD_IMAGE_GRAYSCALE);
  imshow("image", imagem);
  Colorir(imagem,imagem.rows/2,imagem.cols/2);
  imshow("New",imagem);
  waitKey();
  return 0;
}
