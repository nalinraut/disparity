
#include "disparity.h"



int main(int argc, char const *argv[])
{

  int ndisp = 256;	
  Mat img_1 = imread(argv[1], 1);
  Mat img_2 = imread(argv[2], 1);
  Mat disparity_img;
  

  cout<< "Image Column: "<< img_1.cols<< endl;
  cout<< "Image Rows: "<< img_1.rows<< endl;

  DisparityGenerator disp(img_1, img_2, ndisp);
  disp.computeDescriptorORB();
  disp.computeDisparity(); 
  disp.getDisparityImage(disparity_img);


  while (1) {
    imshow("IMG-LEFT", img_1);
    imshow("IMG-RIGHT", img_2);
    imshow("IMG-DISP", disparity_img);
    
    if (waitKey(30) > 0) {
      if(argc!=4)
        imwrite("disparity_out.png", disparity_img);
      else
        imwrite(argv[3], disparity_img);
      break;
    }
  }
  return 0;
}