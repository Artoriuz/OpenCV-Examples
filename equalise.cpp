#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	Mat img, img_eql, hist, hist_eql;
	img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	
	equalizeHist(img, img_eql);
	
	bool uniform = true;
	bool acummulate = false;
	
	int nbins = 128;
	float range[] = { 0, 256 };
	const float *histrange = { range };

	int width = img.cols;
	int height = img.rows;

	int histw = nbins, histh = nbins / 4;
	
	Mat hist_img(histh, histw, CV_8UC1, Scalar(0, 0, 0));
	Mat hist_img_eql(histh, histw, CV_8UC1, Scalar(0, 0, 0));

	calcHist(&img, 1, 0, Mat(), hist, 1, &nbins, &histrange, uniform, acummulate);
	calcHist(&img_eql, 1, 0, Mat(), hist_eql, 1, &nbins, &histrange, uniform, acummulate);

	normalize(hist, hist, 0, hist_img.rows, NORM_MINMAX, -1, Mat());
	normalize(hist_eql, hist_eql, 0, hist_img.rows, NORM_MINMAX, -1, Mat());

	hist_img.setTo(Scalar(0));
	hist_img_eql.setTo(Scalar(0));

	for (int i = 0; i<nbins; i++) {
		line(hist_img, Point(i, histh), Point(i, cvRound(hist.at<float>(i))), Scalar(255, 255, 255), 1, 8, 0);
	}
	for (int i = 0; i<nbins; i++) {
		line(hist_img_eql, Point(i, histh), Point(i, cvRound(hist_eql.at<float>(i))), Scalar(255, 255, 255), 1, 8, 0);
	}

	hist_img.copyTo(img(Rect(10, 10, nbins, histh)));
	hist_img_eql.copyTo(img_eql(Rect(10, 10, nbins, histh)));

	namedWindow("Original", WINDOW_AUTOSIZE);
	namedWindow("Equalised", WINDOW_AUTOSIZE);
	
	imshow("Original", img);
	imshow("Equalised", img_eql);
	waitKey();
	
	return 0;
}