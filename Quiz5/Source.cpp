#include<opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <map>

using namespace cv;
using namespace std;

Mat getHist(const Mat& src,bool cdf = false) {
	vector<Mat> bgr_planes;
	split(src, bgr_planes);
	int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange[] = { range };
	bool uniform = true, accumulate = false;
	Mat b_hist, g_hist, r_hist;
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate);
	int hist_w = 512, hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	if (cdf) {
		for (int i = 0; i < histSize; i++) {
			if (i == 0) {
				b_hist.at<float>(i) /= (src.rows * src.cols);
				g_hist.at<float>(i) /= (src.rows * src.cols);
				r_hist.at<float>(i) /= (src.rows * src.cols);
			}
			else {
				b_hist.at<float>(i) = b_hist.at<float>(i - 1) + b_hist.at<float>(i) / (src.rows * src.cols);
				g_hist.at<float>(i) = g_hist.at<float>(i - 1) + g_hist.at<float>(i) / (src.rows * src.cols);
				r_hist.at<float>(i) = r_hist.at<float>(i - 1) + r_hist.at<float>(i) / (src.rows * src.cols);
			}
			
		}
	}
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}
	return histImage;
}


int main()
{	
	// Must do
	Mat src1 = imread("target/src1.jpg", cv::IMREAD_COLOR);
	Mat src2 = imread("target/src2.jpg", cv::IMREAD_COLOR);
	Mat gray_src1 = imread("target/src1.jpg", cv::IMREAD_GRAYSCALE);
	Mat gray_src2 = imread("target/src2.jpg", cv::IMREAD_GRAYSCALE);
	Mat result1 = Mat::zeros(gray_src1.rows, gray_src1.cols, CV_8UC1);
	Mat result2 = Mat::zeros(gray_src2.rows, gray_src2.cols, CV_8UC1);

	float pdf1[256] = { 0 }, cdf1[256] = { 0 };
	float pdf2[256] = { 0 }, cdf2[256] = { 0 };
	float gray_src1_pixels = gray_src1.rows * gray_src1.cols, gray_src2_pixels = gray_src2.rows * gray_src2.cols;
	// Counting
	for (int i = 0; i < gray_src1.rows; i++) {
		for (int j = 0; j < gray_src1.cols; j++) {
			pdf1[gray_src1.at<uchar>(i, j)]++;
		}
	}
	for (int i = 0; i < gray_src2.rows; i++) {
		for (int j = 0; j < gray_src2.cols; j++) {
			pdf2[gray_src2.at<uchar>(i, j)]++;
		}
	}

	// Get probability(PDF)
	for (int i = 0; i < 256; i++) {
		pdf1[i] /= gray_src1_pixels;
		pdf2[i] /= gray_src2_pixels;
	}

	// Get CDF
	cdf1[0] = pdf1[0], cdf2[0] = pdf2[0];
	for (int i = 1; i < 256; i++) {
		cdf1[i] = pdf1[i] + cdf1[i - 1];
		cdf2[i] = pdf2[i] + cdf2[i - 1];
	}

	// Do Equalization
	for (int i = 0; i < gray_src1.rows; i++) {
		for (int j = 0; j < gray_src1.cols; j++) {
			result1.at<uchar>(i, j) = round(0 + cdf1[gray_src1.at<uchar>(i, j)] * (255 - 0));
		}
	}
	for (int i = 0; i < gray_src2.rows; i++) {
		for (int j = 0; j < gray_src2.cols; j++) {
			result2.at<uchar>(i, j) = round(0 + cdf2[gray_src2.at<uchar>(i, j)] * (255 - 0));
		}
	}

	imshow("src1", src1);
	imshow("src2", src2);
	imshow("result1", result1);
	imshow("result2", result2);
	imwrite("result1.png", result1);
	imwrite("result2.png", result2);

	// Bonus
	Mat src = imread("target/src3.jpg", cv::IMREAD_COLOR);
	Mat tgt = imread("target/target.jpg", cv::IMREAD_COLOR);
	Mat result = Mat::zeros(src.rows, src.cols, CV_8UC3);

	//imshow("src", src);
	//imshow("tgt", tgt);
	//imshow("result", result);
	//imshow("src hist", getHist(src,false));
	//imshow("tgt hist", getHist(tgt,false));
	//imshow("result hist", getHist(result,false));
	//imwrite("result.png",result);
	waitKey();
}