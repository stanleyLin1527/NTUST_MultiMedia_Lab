#include<opencv2/opencv.hpp>
#include <fstream>
#include<iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat src1 = imread("src1.png", cv::IMREAD_GRAYSCALE);	// watermark
	Mat result1 = Mat::zeros(src1.rows, src1.cols, CV_8UC1);
	
	//Filling your code here
	// Traverse all the pixels
	for (int row = 0; row < src1.rows; row++) {
		for (int col = 0; col < src1.cols; col++) {
			double LSB_flag = src1.at<uchar>(row, col) % 2; // Get the last significant bit

			// Because a QR code only has two colors: black(255) and white(0)
			// So we need to change that pixel to black or white regarding the LSB
			if (LSB_flag) 
				result1.at<uchar>(row, col) = 255;
			else 
				result1.at<uchar>(row, col) = 0;
		}
	}
	uint scale = 2;
	Mat src2 =  imread("src2.jpg", cv::IMREAD_GRAYSCALE);	// linear interpolation
	Mat result2 = Mat::zeros(src2.rows * scale, src2.cols * scale, CV_8UC1);
	// Filling your code here
	// Traverse all the pixels in the big picture
	// MUST BE -2, or will get error
	for (int row = 0; row < result2.rows - 2; row++) {
		for (int col = 0; col < result2.cols - 2; col++) {
			// Get the position of original picture
			int prow = floor(row / 2), pcol = floor(col / 2);
			
			// Get the weight of original picture
			int frow = row / 2 - prow, fcol = col / 2 - pcol;

			// Do bilinear interpolation
			result2.at<uchar>(row, col) = \
				( 1 - frow ) * ( 1 - fcol ) * src2.at<uchar>(prow, pcol) + \
				frow * ( 1 - fcol ) * src2.at<uchar>(prow + 1, pcol) + \
				( 1 - frow ) * fcol * src2.at<uchar>(prow, pcol + 1) + \
				frow * fcol * src2.at<uchar>(prow + 1, pcol + 1);
		}
	}

	imshow("src1", src1);
	imshow("src2", src2);
	imshow("result1", result1);
	imshow("result2", result2);
	imwrite("result1.png",result1);
	imwrite("result2.png",result2);
	/*imwrite("result.png", dst);*/
	waitKey();
}