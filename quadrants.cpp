#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main(int argc, char** argv) {

	Mat img = imread(argv[1]);
	if (!img.data) {
		cout << "imagem nao carregou corretamente" << endl;
		return(-1);
	}

	Mat img_out = Mat::zeros(img.rows, img.cols, CV_8UC3);

	namedWindow("janela", WINDOW_AUTOSIZE);
	imshow("janela", img);
	waitKey();

	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			if (i < img.rows / 2 && j < img.cols / 2) {
				img_out.at<Vec3b>(i + img.rows / 2, j + img.cols / 2) = img.at<Vec3b>(i, j);
			}
			else if (i < img.rows / 2 && j > img.cols / 2) {
				img_out.at<Vec3b>(i + img.rows / 2, j - img.cols / 2) = img.at<Vec3b>(i, j);
			}
			else if (i > img.rows / 2 && j < img.cols / 2) {
				img_out.at<Vec3b>(i - img.rows / 2, j + img.cols / 2) = img.at<Vec3b>(i, j);
			}
			else if (i > img.rows / 2 && j > img.cols / 2) {
				img_out.at<Vec3b>(i - img.rows / 2, j - img.cols / 2) = img.at<Vec3b>(i, j);
			}
		}
	}

	imshow("janela", img);

	namedWindow("janelaout", WINDOW_AUTOSIZE);
	imshow("janelaout", img_out);

	waitKey();
	return 0;
}