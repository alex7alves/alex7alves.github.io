#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv){
  Mat image, mask;
  int width, height;
  int nobjects, n1bolha = 0, n2bolha = 0, n3bolha = 0, n4bolha = 0, n5bolha = 0, desconhecidos, sembolhas;
  uchar atual, ultimocinza = 0;
  bool pburaco = false;
  
  CvPoint p;
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  
  if(!image.data){
    std::cout << "imagem nao carregou corretamente\n";
    return(-1);
  }
  width=image.size().width;
  height=image.size().height;

  p.x=0;
  p.y=0;

  floodFill(image,p,1);

  desconhecidos = 0;
  for(int i = 0; i < height; i++){
      if(image.at(i,0) == 255){
		desconhecidos++;
		p.x=0;
		p.y=i;
		floodFill(image,p,1);
      }
      if(image.at(i,width-1) == 255){
		desconhecidos++;
		p.x=width-1;
		p.y=i;
		floodFill(image,p,1);
      }
  }
  for(int j = 0; j < width; j++){
      if(image.at(0,j) == 255){
		desconhecidos++;
		p.x=j;
		p.y=0;
		floodFill(image,p,1);
      }
      if(image.at(height-1,j) == 255){
		desconhecidos++;
		p.x=j;
		p.y=height-1;
		floodFill(image,p,1);
      }
  }
  nobjects = desconhecidos;
  height--;
  width--;
  
  for(int i=1; i(i,j);
      if(atual == 255){
		nobjects++;
		p.x=j;
		p.y=i;
		floodFill(image,p,nobjects);
                pburaco = false;
                ultimocinza = nobjects;
      }else if(atual == 0){
        pburaco = true;
      }else if(atual == 1){
        pburaco = false;
      }else{
        // 101-150-> 1 buraco
        // 151-190-> 2 buracos
        // 191-220-> 3 buracos
        // 221-240-> 4 buracos
        // 241-250-> 5 buracos
        if(ultimocinza == atual && pburaco == true){
          if(atual < 101){
            n1bolha++;
            p.x = j-1;
            p.y = i;
            floodFill(image,p,100+n1bolha);
            p.x = j;
            floodFill(image,p,100+n1bolha);
          }else if(atual < 151){
            n2bolha++;
            p.x = j-1;
            p.y = i;
            floodFill(image,p,150+n2bolha);
            p.x = j;
            floodFill(image,p,150+n2bolha);
          }else if(atual < 191){
            n3bolha++;
            p.x = j-1;
            p.y = i;
            floodFill(image,p,190+n3bolha);
            p.x = j;
            floodFill(image,p,190+n3bolha);
          }else if(atual < 221){
            n4bolha++;
            p.x = j-1;
            p.y = i;
            floodFill(image,p,220+n4bolha);
            p.x = j;
            floodFill(image,p,220+n4bolha);
          }else if(atual < 241){
            n5bolha++;
            p.x = j-1;
            p.y = i;
            floodFill(image,p,240+n5bolha);
            p.x = j;
            floodFill(image,p,240+n5bolha);
          }
        }
        ultimocinza = atual;
        pburaco = false;
      }
    }
  }
  sembolhas = nobjects-desconhecidos-n1bolha;
  n1bolha = n1bolha-n2bolha;
  n2bolha = n2bolha-n3bolha;
  n3bolha = n3bolha-n4bolha;
  n4bolha = n4bolha-n5bolha;
  std::cout << "Numero de total de objetos: " << nobjects << std::endl;
  std::cout << "Numero de objetos nas bordas: " << desconhecidos << std::endl;
  std::cout << "Numero de objetos sem bolhas: " << sembolhas << std::endl;
  std::cout << "Numero de bolhas com 1 furo: " << n1bolha << std::endl;
  std::cout << "Numero de bolhas com 2 furos: " << n2bolha << std::endl;
  std::cout << "Numero de bolhas com 3 furos: " << n3bolha << std::endl;
  std::cout << "Numero de bolhas com 4 furos: " << n4bolha << std::endl;
  std::cout << "Numero de bolhas com 5 furos: " << n5bolha << std::endl;
  imshow("image", image);
  imwrite("labeling2.png", image);
  waitKey();
  return 0;
}