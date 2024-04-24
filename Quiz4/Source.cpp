#include<opencv2/opencv.hpp>
#include <fstream>
#include<iostream>

using namespace cv;
using namespace std;

void myErode(Mat& input, int mask_size);

int main()
{
	Mat src = imread("image.jpg", cv::IMREAD_GRAYSCALE);
	Mat result = Mat::zeros(src.rows, src.cols, CV_8UC1);
	Mat labels;
	resize(src, src, Size(700, (int)(700.0*src.rows/src.cols)));

	// fill your code here

	// Binarization (¤G­È¤Æ)
	threshold(src, result, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

	// Do closing(dilate then erode) to fill blank
	morphologyEx(result, result, MORPH_CLOSE, Mat(), Point(-1, -1), 1);

	// Do erode 20 times	
	for (int i = 0; i < 20; i++) myErode(result, 3);

	// Find the number of chess using connected components
	int numComponents = connectedComponents(result, labels);

	// show the result, -1 for the background
	cout << "Number of connected components: " << numComponents - 1 << endl;

	imshow("src", src);
	imshow("result", result);
	imwrite("result.png",result);
	waitKey();
}

void myErode(Mat &input, int mask_size) {
	Mat newInput = Mat::zeros(input.rows, input.cols, CV_8UC1);
	int abs_offset = floor(mask_size / 2);

	for (int row = abs_offset; row < input.rows - abs_offset; row++) {
		for (int col = abs_offset; col < input.cols - abs_offset; col++) {
			if (input.at<uchar>(row, col) != 0) {
				bool valid = true;

				for (int i = -abs_offset; i <= abs_offset; i++) {
					for (int j = -abs_offset; j <= abs_offset; j++) {
						if (input.at<uchar>(row + i, col + j) == 0) {
							valid = false;
							break;
						}
					}
					if (!valid) break;
				}

				if (valid) newInput.at<uchar>(row, col) = input.at<uchar>(row, col);
			}
		}
	}

	input = newInput;
}