#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

double alfa;
int alfa_slider = 0;
int alfa_slider_max = 100;

int h,h_slider;
int p,p_slider;
int top_slider_max = 100;




float tiltshift(int x, double a, double b, double d) {
    if(d>0)
        return (double)0.5*(tanh((x-a)/d)-tanh((x-b)/d));
    else
        return 1;
}

void on_trackbar_blend(int, void*){
 alfa = (double) alfa_slider/alfa_slider_max ;
}


int main(int argvc, char** argv){
  Mat image, borrado;
 // image = imread("blend1.jpg");
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  float alpha;
   Mat blended = Mat::zeros(image.size(), CV_8UC3);
  // Passando o filtro gaussiano
  GaussianBlur(borrado, image, Size(5,5),7,0);
  
  namedWindow("addweighted", 1);
  
 
  createTrackbar( "Alpha", "addweighted",
				  &alfa_slider,
				  alfa_slider_max,
				  on_trackbar_blend );
  on_trackbar_blend(alfa_slider, 0 );
  
  h= image.rows;
  createTrackbar( "Altura", "addweighted",
				  &h_slider,
				  h,
				  on_trackbar_blend );
  on_trackbar_blend(h_slider, 0 );
  p= image.rows;
  createTrackbar( "Posição vertical", "addweighted",
          &p_slider,
          p,
          on_trackbar_blend );
  on_trackbar_blend(p_slider, 0 );


  for(int i=0;i<image.rows;i++){
    alpha = tiltshift(i,h,p,alfa_slider);
    addWeighted(image.row(i),alpha,borrado.row(i), 1-alpha, 0.0, blended.row(i));
  }
  

   
  // 
  waitKey(0);
  return 0;
}
