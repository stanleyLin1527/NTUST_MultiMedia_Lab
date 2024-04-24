#include<opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("Lenna.jpg");
	Mat Gray_img = Mat(img.rows, img.cols, CV_8UC1); // To store the converted image

	// Method 1: Using OpenCV's function to change color directly 
	// cvtColor(img, Gray_img, COLOR_BGR2GRAY);

	// Method 2: Traversing all the pixels in the image, and change each pixel into gray
	for (int row = 0; row < img.rows; row++) {
		for (int col = 0; col < img.cols; col++) {
			// Get an array about the values of BGR
			Vec3b target_pixel = img.at<Vec3b>(row, col); 
			uchar red = target_pixel[2];
			uchar green = target_pixel[1];
			uchar blue = target_pixel[0];
			// Calculate the value of gray
			uchar gray = 0.299 * red + 0.587 * green + 0.114 * blue;
			// Change that pixel into gray
			Gray_img.at<uchar>(row, col) = gray;
		}
	}

	imshow("Hello World!", img);		   // Show the original image
	imshow("Hello  World!", Gray_img);	   // Show the result
	imwrite("./Gray_Lenna.jpg", Gray_img); // Output the converted image
	waitKey();
	return 0;
}