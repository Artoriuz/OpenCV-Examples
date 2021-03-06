#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void printmask(Mat &m) {
	for (int i = 0; i<m.rows; i++) {
		for (int j = 0; j<m.cols; j++) {
			cout << m.at<float>(i, j) << ",";
		}
		cout << endl;
	}
}

void menu() {
	cout << "\npressione a tecla para ativar o filtro: \n"
		"a - calcular modulo\n"
		"m - media\n"
		"g - gauss\n"
		"v - vertical\n"
		"h - horizontal\n"
		"l - laplaciano\n"
		"esc - sair\n";
}

int main(int argvc, char** argv) {
	VideoCapture video(argv[1]); //use this for videos
	//cap = imread(argv[1]); //use this for images
	
	float media[] = { 1,1,1,1,1,1,1,1,1 };
	float gauss[] = { 1,2,1,2,4,2,1,2,1 };
	float horizontal[] = { -1,0,1,-2,0,2,-1,0,1 };
	float vertical[] = { -1,-2,-1,0,0,0,1,2,1 };
	float laplacian[] = { 0,-1,0,-1,4,-1,0,-1,0 };

	Mat cap, frame, frame32f, frameFiltered, frameFiltered2;
	Mat mask(3, 3, CV_32F), mask_gauss(3, 3, CV_32F), mask_lap(3, 3, CV_32F);
	Mat result, result1, result_lapgauss;
	double width, height, min, max;
	bool lap_gauss_aux = false, absolut = true;
	char key;

	width = video.get(CV_CAP_PROP_FRAME_WIDTH); //use this for videos
	height = video.get(CV_CAP_PROP_FRAME_HEIGHT); //use this for videos
	//width = cap.cols; //use this for images
	//height = cap.rows; //use this for images

	std::cout << "width = " << width << "\n";;
	std::cout << "height = " << height << "\n";;

	namedWindow("spacialfilter", 1);

	mask_gauss = Mat(3, 3, CV_32F, gauss);
	scaleAdd(mask_gauss, 1 / 16.0, Mat::zeros(3, 3, CV_32F), mask_gauss);
	mask_lap = Mat(3, 3, CV_32F, laplacian);

	mask = Mat(3, 3, CV_32F, media);
	scaleAdd(mask, 1 / 9.0, Mat::zeros(3, 3, CV_32F), mask);
	swap(mask, mask);

	menu();
	
	while (true) {
		video >> cap; //comment this when using images as input
		cvtColor(cap, frame, CV_BGR2GRAY);
		imshow("original", frame);
		frame.convertTo(frame32f, CV_32F);
		
		key = (char)waitKey(10);

		if (lap_gauss_aux) {
			filter2D(frame32f, frameFiltered, frame32f.depth(), mask_gauss, Point(1, 1), 0);
			filter2D(frameFiltered, frameFiltered2, frameFiltered.depth(), mask_lap, Point(1, 1), 0);

			frameFiltered2 = abs(frameFiltered2);
			frameFiltered2.convertTo(result_lapgauss, CV_8U);

			imshow("spacialfilter", result_lapgauss);

		} else {
			filter2D(frame32f, frameFiltered, frame32f.depth(), mask, Point(1, 1), 0);
			if (absolut) {
				frameFiltered = abs(frameFiltered);
			}
			frameFiltered.convertTo(result, CV_8U);
			imshow("spacialfilter", result);
		}
			
		if (key == 27) break; // esc pressed!

		switch (key) {
		case 'a':
			menu();
			absolut = !absolut;
			lap_gauss_aux = false;
			break;
		case 'm':
			menu();
			mask = Mat(3, 3, CV_32F, media);
			scaleAdd(mask, 1 / 9.0, Mat::zeros(3, 3, CV_32F), mask);
			printmask(mask);
			lap_gauss_aux = false;
			break;
		case 'g':
			menu();
			mask = mask_gauss;
			printmask(mask_gauss);
			lap_gauss_aux = false;
			break;
		case 'h':
			menu();
			mask = Mat(3, 3, CV_32F, horizontal);
			printmask(mask);
			lap_gauss_aux = false;
			break;
		case 'v':
			menu();
			mask = Mat(3, 3, CV_32F, vertical);
			printmask(mask);
			lap_gauss_aux = false;
			break;
		case 'l':
			menu();
			mask = Mat(3, 3, CV_32F, laplacian);
			printmask(mask);
			lap_gauss_aux = false;
			break;
		case 'q':
			lap_gauss_aux = true;
			break;
		default:
			break;
		}
	}
	return 0;
}
