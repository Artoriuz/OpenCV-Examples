#include <iostream>
#include <opencv2/opencv.hpp>

#define RADIUS 20

using namespace cv;
using namespace std;

void deslocaDFT(Mat &image){
	Mat tmp, A, B, C, D;

	image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
	int cx = image.cols / 2;
	int cy = image.rows / 2;

	A = image(Rect(0, 0, cx, cy));
	B = image(Rect(cx, 0, cx, cy));
	C = image(Rect(0, cy, cx, cy));
	D = image(Rect(cx, cy, cx, cy));

	A.copyTo(tmp);
	D.copyTo(A);
	tmp.copyTo(D);

	C.copyTo(tmp);
	B.copyTo(C);
	tmp.copyTo(B);
}

int main(int argc, char **argv){
	Mat imaginaryInput, complexImage, multsp;
	Mat padded, filter, mag;
	Mat image, imagegray, tmp;
	Mat_<float> realInput, zeros;
	vector<Mat> planos;

	image = imread(argv[1]);

	float mean;

	int dft_M, dft_N;

	dft_M = getOptimalDFTSize(image.rows);
	dft_N = getOptimalDFTSize(image.cols);

	copyMakeBorder(image, padded, 0,
				   dft_M - image.rows, 0,
				   dft_N - image.cols,
				   BORDER_CONSTANT, Scalar::all(0));


	zeros = Mat_<float>::zeros(padded.size());


	complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

	filter = complexImage.clone();

	tmp = Mat(dft_M, dft_N, CV_32F);

	float gama_h = 100;
	float gama_l = 510;
	float c = 10;
	float D_0 = 10;

	for (int i = 0; i < dft_M; i++){
		for (int j = 0; j < dft_N; j++){
			tmp.at<float>(i, j) = (gama_h - gama_l) * 
			(1.0 - exp(-1.0 * (float)c * ((((float)i - dft_M / 2.0) * 
			((float)i - dft_M / 2.0) + ((float)j - dft_N / 2.0) * 
			((float)j - dft_N / 2.0)) / (D_0 * D_0)))) + gama_l;
		}
	}

	Mat comps[] = {tmp, tmp};
	merge(comps, 2, filter);

	cvtColor(image, imagegray, CV_BGR2GRAY);
	imshow("original", imagegray);

	copyMakeBorder(imagegray, padded, 0,
				   dft_M - image.rows, 0,
				   dft_N - image.cols,
				   BORDER_CONSTANT, Scalar::all(0));

	planos.clear();

	realInput = Mat_<float>(padded);

	planos.push_back(realInput);
	planos.push_back(zeros);

	merge(planos, complexImage);

	dft(complexImage, complexImage);

	deslocaDFT(complexImage);

	mulSpectrums(complexImage, filter, complexImage, 0);

	planos.clear();
	split(complexImage, planos);

	mean = abs(planos[0].at<float>(dft_M / 2, dft_N / 2));

	merge(planos, complexImage);

	deslocaDFT(complexImage);

	cout << complexImage.size().height << endl;

	idft(complexImage, complexImage);

	planos.clear();

	split(complexImage, planos);

	normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
	imshow("filtrada", planos[0]);
	waitKey();

	return 0;
}