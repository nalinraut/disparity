#include "disparity.h"
#include <omp.h>

DisparityGenerator::DisparityGenerator(Mat& img_1, Mat& img_2, int offset): img1(img_1),
                                                          img2(img_2),
                                                          ndisp(offset)
{
  extractor = ORB::create(500, 1.2f, 8, 31, 0, 2, ORB::HARRIS_SCORE, 31, 20);
  omp_set_num_threads(8);
  // omp_set_nested(1);
}

inline
bool DisparityGenerator::isValidKeyPoint(int i, int j, vector< KeyPoint > &keypoints) 
{
  int n = keypoints.size();
  return (i >= keypoints[0].pt.x && i <= keypoints[n-1].pt.x
          && j >= keypoints[0].pt.y && j <= keypoints[n-1].pt.y);
}

long DisparityGenerator::computeCost(Point &img1_pt, Point img2_pt) 
{

  int w = 0;
  int x0r = keypoints2[0].pt.x;
  int y0r = keypoints2[0].pt.y;
  int ynr = keypoints2[keypoints2.size()-1].pt.y;

  int x0l = keypoints1[0].pt.x;
  int y0l = keypoints1[0].pt.y;
  int ynl = keypoints1[keypoints1.size()-1].pt.y;

  long cost = 0;

  int j_,k_;
  // #pragma omp parallel for private(k_)
  for (int j_ = -w; j_ <= w; j_++) 
  {
    for (int k_ = -w; k_ <= w; k_++) 
    {
      if (!isValidKeyPoint(img1_pt.x+j_, img1_pt.y+k_, keypoints1) || 
          !isValidKeyPoint(img2_pt.x+j_, img2_pt.y+k_, keypoints2))
        continue;

      int idx1 = (img1_pt.x+j_-x0l)*(ynl-y0l+1)+(img1_pt.y+k_-y0l);
      int idx2 = (img2_pt.x+j_-x0r)*(ynr-y0r+1)+(img2_pt.y+k_-y0r);
      
      cost += norm(img1_desc.row(idx1), img2_desc.row(idx2), CV_L1);
    }
  }
  return cost / ((2*w+1)*(2*w+1));
}


inline
int DisparityGenerator::getCorrespondencePoint(Point p, int ndisp) 
{
  long minCost = 1e9;
  int chosen_i = 0;

  int pt_idx;
  // #pragma omp parallel for
  for (pt_idx = p.x - ndisp; pt_idx <= p.x; pt_idx++) 
  {
    long cost = computeCost(p, Point(pt_idx, p.y));
    if (cost < minCost) 
    {
      minCost = cost;
      chosen_i = pt_idx;
    }
  }
  if (minCost == 0)
    return p.x;
  return chosen_i;
 
}


void DisparityGenerator::computeDisparity() 
{
  cout << "Computing Disparity..."<< endl ;

  img_disparity = Mat(img1.rows, img1.cols, CV_8UC1, Scalar(0));

  Scalar color = Scalar(255, 255, 0);
  
  int i,j;

  #pragma omp parallel for private(j)
  for (i = ndisp+1; i < img1.cols; i++) 
  {
    for (j = 0; j < img1.rows; j++) 
    {
      if (!isValidKeyPoint(i,j, keypoints1))
        continue;
      int right_i = getCorrespondencePoint(Point(i,j), ndisp);
      
      img_disparity.at<uchar>(j,i) = static_cast<uchar>(abs(i - right_i));//

#ifdef DEBUG_OUTPUT
      if ((i*j)%10000==0){}
        //cout  << i << ", " << j << " " 
        //      << "DISPARITY: " 
        //      << static_cast<int>(img_disparity.at<uchar>(j,i)) << endl;
#endif

    }
  }

  cout<< "Done."<< endl;
}


inline void DisparityGenerator::detectKeyPoints()
{
  cout << "Detecting Keypoints..." << endl;
  int i,j;
  for (i = 0; i < img1.cols; i++) 
  {
    for (j = 0; j < img1.rows; j++) 
    {
      keypoints1.push_back(KeyPoint(i,j,1));
      keypoints2.push_back(KeyPoint(i,j,1));
    }
  }
  keypoints1.resize(keypoints1.size());
  keypoints2.resize(keypoints2.size());
}

void DisparityGenerator::computeDescriptorORB() 
{

  
  detectKeyPoints();
  
  cout << "Extracting Keypoints..." << endl;
  extractor->compute(img1, keypoints1, img1_desc);
  extractor->compute(img2, keypoints2, img2_desc); 
}

void DisparityGenerator::getDisparityImage(cv::Mat &disparity_img)
{
  disparity_img = img_disparity;
}

