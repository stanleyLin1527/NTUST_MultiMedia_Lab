#include<opencv2/opencv.hpp>
#include <fstream>
#include<iostream>

using namespace cv;
using namespace std;

class ImageNamePair
{
public:
	const Mat* img;
	string name;
	ImageNamePair(const Mat& img, string name) {
		this->img = &img;
		this->name = name;
	}
	~ImageNamePair() {

	}
};

void compare(vector<ImageNamePair> name_image_pairs, string path = "result.txt") {
	ofstream myfile;
	myfile.open(path, fstream::out);
	myfile << "name,values" << endl;
	for (ImageNamePair pair : name_image_pairs) {
		myfile << pair.name << ",";
		for (auto it = pair.img->begin<uchar>(), end = pair.img->end<uchar>(); it != end; ++it)
		{
			myfile << (int)*it << " ";
		}
		myfile << "====================================================" << endl;
	}
	myfile.close();

}

int main()
{
	Mat src1 = imread("lenna.jpg", cv::IMREAD_GRAYSCALE);	// ¦Ç¶¥¹Ï
	Mat result1 = Mat::zeros(src1.rows, src1.cols, CV_8UC1);
	// Filling your code here
	double ratio = 25; // 5 * 5 mask, so there's 25 pixels

	// Traverse all the pixels
	for (int row = 0; row < src1.rows; row++) {
		for (int col = 0; col < src1.cols; col++) {
			// Valid position to do Smoothing method
			if (!(row < 2 || row > src1.rows - 3 || col < 2 || col > src1.cols - 3)) {
				double sum = 0; // The value of that pixel's color 

				for (int i = -2; i <= 2; i++) {
					for (int j = -2; j <= 2; j++) {
						sum += src1.at<uchar>(row + i, col + j);
					}
				}

				result1.at<uchar>(row, col) = (uchar)(sum / ratio);
			}
		}
	}

	Mat src2 = imread("lenna_noise.jpg", cv::IMREAD_GRAYSCALE);	// ¦Ç¶¥¹Ï
	Mat result2 = Mat::zeros(src2.rows, src2.cols, CV_8UC1);
	// Filling your code here


	imshow("src1", src1);
	// imshow("src2", src2);
	imshow("result1", result1);
	// imshow("result2", result2);
	// ImageNamePair lenna = ImageNamePair(src1, "lenna");
	// ImageNamePair smooth = ImageNamePair(result1, "smooth");
	// ImageNamePair lenna_noise = ImageNamePair(src2, "lenna_noise");
	// ImageNamePair mid_filter = ImageNamePair(result2, "mid_filter");
	// vector<ImageNamePair> lenna_and_smooth = vector<ImageNamePair> { lenna, smooth };
	// compare(lenna_and_smooth, "result.txt");
	imwrite("result1.png", result1);
	// imwrite("result2.png", result2);
	/*imwrite("result.png", dst);*/
	waitKey();
}

