#ifndef DISPARITY_H
#define DISPARITY_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>

#define DEBUG_OUTPUT

using namespace cv;
using namespace std;

/**
 * @brief Generates disparity image from 2 images using ORB features
 * 
 */
class DisparityGenerator
{

private:
	Mat img1, img2, img_disparity;
	Mat img1_desc, img2_desc;

	vector< KeyPoint > keypoints1, keypoints2;
	
	Ptr< FeatureDetector > extractor;
	
	int ndisp;


public:

	/**
	 * @brief Construct a new Disparity Generator object ~ parametrized constructor
	 * 
	 * @param img_1 
	 * @param img_2 
	 * @param offset 
	 */
	DisparityGenerator(Mat& img_1, Mat& img_2, int offset);

	/**
	 * @brief check validaity of keypoint
	 * 
	 * @param i 
	 * @param j 
	 * @param keypoints 
	 * @return true 
	 * @return false 
	 */
	inline bool isValidKeyPoint(int i, int j, std::vector< KeyPoint> &keypoints);

	/**
	 * @brief computes cost between given 2 points
	 * 
	 * @param img1_pt 
	 * @param img2_pt 
	 * @return long 
	 */
	long computeCost(Point &img1_pt, Point img2_pt);

	/**
	 * @brief Get the Correspondence Point 
	 * 
	 * @param p 
	 * @param ndisp 
	 * @return int 
	 */
	inline int getCorrespondencePoint(Point p, int ndisp);

	/**
	 * @brief compute disparity between left and right image
	 * 
	 */
	void computeDisparity();

	/**
	 * @brief detects HARRIS keypoints
	 * 
	 */
	inline void detectKeyPoints();

	/**
	 * @brief computes ORB descriptors
	 * 
	 */
	void computeDescriptorORB();

	/**
	 * @brief Get the Disparity Image 
	 * 
	 * @param disparity_img 
	 */
	void getDisparityImage(Mat & disparity_img);

	

}; // class DisparityGenerator



#endif // CONVERTER_H