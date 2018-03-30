#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	
	Mat image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	if (!image.data) {
		cout << "imagem nao carregou corretamente" << endl;
		return(-1);
	}

	long int obj_total = 0;
	long int obj_with_holes = 0;
	long int obj_found_colour = 2;
	long int obj_colour_aux = 0;

	CvPoint p;

	namedWindow("Labelling", CV_WINDOW_KEEPRATIO);
	imshow("Labelling", image);
	waitKey();

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			if (i == 0 || j == 0 || i == image.rows - 1 || j == image.cols - 1) {
				image.at<uchar>(i, j) = 255;
			}
		}
	}
	p.x = 0; p.y = 0;
	floodFill(image, p, 0); //Retirada das bolhas que tocam as bordas

	imshow("Labelling", image);
	waitKey();

	floodFill(image, p, 1); //Mudança do valor da cor de fundo

	imshow("Labelling", image);
	waitKey();

	for (int i = 0; i < image.rows; i++) { //Marca todos os objetos como sem bolha
		for (int j = 0; j < image.cols; j++) {
			if (image.at<uchar>(i, j) == 255) {
				obj_total++; //Achou algo
				p.x = j;
				p.y = i;
				floodFill(image, p, obj_found_colour);
				obj_found_colour++;
			}
		}
	}

	imshow("Labelling", image);
	waitKey();

	for (int x = 0; x < image.rows; x++) { //Fazer uma busca dos objetos com buracos
		for (int y = 0; y < image.cols; y++) {
			if (image.at<uchar>(x, y) == 0 && image.at<uchar>(x, y - 1) != obj_colour_aux) {
				obj_colour_aux = image.at<uchar>(x, y - 1);
				p.x = y;
				p.y = x;
				floodFill(image, p, obj_colour_aux);
				obj_with_holes++;
			}
			else if (image.at<uchar>(x, y) == 0 && image.at<uchar>(x, y - 1) == obj_colour_aux) {
				p.x = y;
				p.y = x;
				floodFill(image, p, obj_colour_aux);
			}
		}
	}

	imshow("Labelling", image);
	waitKey();

	imwrite("labeling.png", image);

	cout << "Number of elements without holes: " << obj_total - obj_with_holes << endl;
	cout << "Number of elements with holes: " << obj_with_holes << endl;

	waitKey();
	return 0;
}

