#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

#define STEP 5
#define JITTER 3
#define RAIO 3

int top_slider = 10;
int top_slider_max = 200;

char TrackbarName[50];

Mat image, border, frame, points, tmp, result;

void on_trackbar_canny(int, void *){
    Canny(image, border, top_slider, 3 * top_slider);
    imshow("canny", border);
}

int main(int argc, char **argv){
    vector<int> yrange;
    vector<int> xrange;

    int width, height;
    int x, y;
    int r, g, b;

    image = imread(argv[1]);

    srand(time(0));

    if (!image.data){
        cout << "nao abriu" << argv[1] << endl;
        cout << argv[0] << " imagem.jpg";
        exit(0);
    }

    width = image.size().width;
    height = image.size().height;

    xrange.resize(height / STEP);
    yrange.resize(width / STEP);

    iota(xrange.begin(), xrange.end(), 0);
    iota(yrange.begin(), yrange.end(), 0);

    for (uint i = 0; i < xrange.size(); i++){
        xrange[i] = xrange[i] * STEP + STEP / 2;
    }

    for (uint i = 0; i < yrange.size(); i++){
        yrange[i] = yrange[i] * STEP + STEP / 2;
    }

    random_shuffle(xrange.begin(), xrange.end());
    points = Mat(height, width, CV_8UC3, Scalar(CV_RGB(255, 255, 255)));
    for (auto i : xrange){
        random_shuffle(yrange.begin(), yrange.end());
        for (auto j : yrange){
            x = i + rand() % (2 * JITTER) - JITTER + 1;
            y = j + rand() % (2 * JITTER) - JITTER + 1;
            r = image.at<Vec3b>(x, y)[2];
            g = image.at<Vec3b>(x, y)[1];
            b = image.at<Vec3b>(x, y)[0];
            circle(points, cv::Point(y, x), RAIO, CV_RGB(r, g, b), -1, CV_AA);
        }
    }
    namedWindow("pontos");
    imshow("pontos", points);

    for (int round = 0; round < 2; round++){
        sprintf(TrackbarName, "Threshold inferior", top_slider_max);

        namedWindow("canny");
        createTrackbar(TrackbarName, "canny",
                       &top_slider,
                       top_slider_max,
                       on_trackbar_canny);

        on_trackbar_canny(top_slider, 0);
        waitKey();
        Mat aux = Mat(height, width, CV_8U, Scalar(0));
        for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){
                if (border.at<uchar>(i, j) > 0 && aux.at<uchar>(i, j) == 0){
                    r = image.at<cv::Vec3b>(i, j)[2];
                    g = image.at<cv::Vec3b>(i, j)[1];
                    b = image.at<cv::Vec3b>(i, j)[0];
                    circle(points, cv::Point(j, i), RAIO - round, CV_RGB(r, g, b), -1, CV_AA);
                    circle(aux, cv::Point(j, i), 2 * (RAIO - round), CV_RGB(255, 255, 255), -1, CV_AA);
                }
            }
        }

        namedWindow("pontos");
        imshow("pontos", points);
        waitKey();
    }
    return 0;
}