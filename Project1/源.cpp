#include <iostream>

#include <opencv2/opencv.hpp>



using namespace cv;

using namespace std;



float *HOG(Mat refMat) {

	int blockSize = 16;

	int nX = refMat.cols / blockSize;

	int nY = refMat.rows / blockSize;

	int num = nX * nY;

	int bins = num * 8;

	Mat gx, gy;

	Mat mag, angle;

	Sobel(refMat, gx, CV_32F, 1, 0, 1);

	Sobel(refMat, gy, CV_32F, 0, 1, 1);

	cartToPolar(gx, gy, mag, angle, true);

	//定义存储数组

	float* ref_hist = new float[bins];

	memset(ref_hist, 0, sizeof(float) * bins);

	int counts = 0;
	for (int j = 0; j < nY; j++) {
		for (int i = 0; i < nX; i++) {
			for (int a = 0; a < 16; a++) {
				for (int b = 0; b < 16; b++) {
					int angle1 = 0;
					angle1 = angle.at<float>(a, b) / 45;
					float mag1 = mag.at<float>(a, b);
					ref_hist[angle1 + counts * 8] = ref_hist[angle1 + counts * 8] + mag1;
				}
			}
			counts++;
		}
	}
	return ref_hist;
	delete[] ref_hist;

}



int main() {

	Mat src = imread("template.png", 0);

	Mat src1 = imread("img.png", 0);

	int blockSize = 16;

	int nX = src.cols / blockSize;

	int nY = src.rows / blockSize;

	int num = nX * nY;

	int bins = num * 8;
	int h = 0;
	int w = 0;

	float *hog1;
    float *hog2;

	Mat tempMat;

	Mat refMat;



	int min = 100000;

	//代入函数	

	hog1 = HOG(src);

	Rect r2;

	for (h = 0; h < src1.rows - src.rows; h++) {

		for (w = 0; w < src1.cols - src.cols; w++)

		{

			Rect r(w,h, src.cols, src.rows);

			tempMat = src1(r);

			tempMat.copyTo(refMat);

			int res1 = 0;

			hog2 = HOG(refMat);

			//计算直方图距离



			for (int i= 0; i < bins;i++)

			{

				res1 += (hog1[i] - hog2[i])*(hog1[i] - hog2[i]);

			}

			res1 = sqrt(res1);
			if (res1 < min)

			{

				min = res1;

				r2 = r;

			}

		}

	}



	rectangle(src1, r2, CV_RGB(255, 255, 255), 1, 8, 0);
	imshow("result", src1);
	waitKey(0);

	return 0;

}