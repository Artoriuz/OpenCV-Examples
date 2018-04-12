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

	int pos_xi, pos_yi, pos_xf, pos_yf;

	cout << "digite o pixel x inicial:" << endl;
	cin >> pos_xi;
	cout << "digite o pixel y inicial:" << endl;
	cin >> pos_yi;

	cout << "digite o pixel x final:" << endl;
	cin >> pos_xf;
	cout << "digite o pixel y final:" << endl;
	cin >> pos_yf;

	namedWindow("janela", WINDOW_AUTOSIZE);
	imshow("janela", img);
	waitKey();


	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			if (i > pos_xi && i < pos_xf && j > pos_yi && j < pos_yf) {
				bitwise_not(img.at<Vec3b>(i, j), img_out.at<Vec3b>(i, j));
			}
			else {
				img_out.at<Vec3b>(i, j) = img.at<Vec3b>(i, j);
			}
		}
	}
	imshow("janela", img);

	namedWindow("janelaout", WINDOW_AUTOSIZE);
	imshow("janelaout", img_out);

	waitKey();
	return 0;
}
