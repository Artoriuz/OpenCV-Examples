#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>
using namespace cv;
using namespace std;

int main(int argvc, char** argv) {
	VideoCapture video(argv[1]);
	Mat img_in, img_discard;
	int cols = video.get(CV_CAP_PROP_FRAME_WIDTH);
	int rows = video.get(CV_CAP_PROP_FRAME_HEIGHT);

	Mat img_blurred = Mat::zeros(rows, cols, CV_32FC3);
	Mat img_out = Mat::zeros(rows, cols, CV_32FC3);
	Mat img_focused = Mat::zeros(rows, cols, CV_32FC3);
	Mat img_unfocused = Mat::zeros(rows, cols, CV_32FC3);
	Mat mask_focus = Mat(rows, cols, CV_32FC3, Scalar(0, 0, 0));
	Mat mask_unfocus = Mat(rows, cols, CV_32FC3, Scalar(1, 1, 1));

	int frame_skip = 12; //Sets how many frames to skip between frames
	int pass_frame = 0;

	float unfocus_aux = 0.35; //Edit this to change how much of the original image will stay focused
	int pos_i = unfocus_aux*rows;
	int pos_f = rows - unfocus_aux*rows;
	int decay = 60; //Edit this to change how fast it'll decay

	Vec3f mask_function_value;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			mask_function_value[0] = (tanh((float(i - pos_i) / decay)) - tanh((float(i - pos_f) / decay))) / 2;
			mask_function_value[1] = (tanh((float(i - pos_i) / decay)) - tanh((float(i - pos_f) / decay))) / 2;
			mask_function_value[2] = (tanh((float(i - pos_i) / decay)) - tanh((float(i - pos_f) / decay))) / 2;
			mask_focus.at<Vec3f>(i, j) = mask_function_value;
		}
	}
	mask_unfocus = mask_unfocus - mask_focus;

	//This section shows the masks
	imshow("focus mask", mask_focus);
	imshow("unfocus mask", mask_unfocus);
	
	while (true) {
		if (pass_frame == 0) {
			video >> img_in;
		} else {
			video >> img_discard;
		}
		
		img_in.convertTo(img_in, CV_32FC3);
		GaussianBlur(img_in, img_blurred, Size(7, 7), 0, 0);

		img_focused = img_in.mul(mask_focus);
		img_unfocused = img_blurred.mul(mask_unfocus);

		//This section only shows the blurred image
		img_blurred.convertTo(img_blurred, CV_8UC3);
		imshow("img_blurred", img_blurred);
		//waitKey();

		img_out = img_focused + img_unfocused;
		img_out.convertTo(img_out, CV_8UC3);

		//This section only shows the components of the final image
		img_focused.convertTo(img_focused, CV_8UC3);
		img_unfocused.convertTo(img_unfocused, CV_8UC3);
		imshow("img_focused", img_focused);
		imshow("img_unfocused", img_unfocused);
		//waitKey();

		if (pass_frame == 0) {
			pass_frame = frame_skip;
		}
		else {
			pass_frame -= 1;
		}

		imshow("output", img_out);
		if (waitKey(30) >= 0) break;
	}
	return 0;
}
