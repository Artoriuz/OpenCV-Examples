#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	long frame_counter = 0, difference_total_value = 0;
	Mat frame, frame_past, difference;
	VideoCapture cap(argv[1]);
	
	int width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	
	difference = Mat::zeros(height, width, CV_8UC1);

	while (1) {
		cap >> frame;
		cvtColor(frame, frame, CV_BGR2GRAY);
		
		difference_total_value = 0;
		for (int i = 0; i < frame.rows; i++) {
			for (int j = 0; j < frame.cols; j++) {
				if (frame_counter == 0) {
					difference.at<uchar>(i, j) = 0;
				} else {
					difference.at<uchar>(i, j) = abs(frame.at<uchar>(i, j) - frame_past.at<uchar>(i, j));
				}
				difference_total_value += difference.at<uchar>(i, j);
			}
		}

		frame_past = frame;
		frame_counter++;

		cout << "frame: " << frame_counter << " difference = " << difference_total_value << endl;

		if (difference_total_value > 150000) {
			cout << "THERE'S MOVEMENT!" << endl;
		}

		namedWindow("Difference", WINDOW_AUTOSIZE);
		imshow("Difference", difference);
		if (waitKey(30) >= 0) break;
	}
	return 0;
}