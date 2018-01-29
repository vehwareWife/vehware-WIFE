#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	const char* filename = argc >= 2 ? argv[1] : "door6.jpg";
	bool showSteps = true;
	int xStart_first, xEnd_first, xStart_second, xEnd_second, yStart, yEnd;
	int yUpStart, yUpEnd;

	Mat src = imread(filename, 0);
	if (src.empty())
	{
		cout << "can not open " << filename << endl;
		return -1;
	}

	Mat blured, dst, cdst, distance;
	blured = src.clone();
	distance = src.clone();
	for (int i = 1; i < 9; i = i + 2)
	{
		GaussianBlur(src, blured, Size(i, i), 0, 0);
		
	}
	Canny(blured, dst, 50, 200, 3);
	cvtColor(dst, cdst, CV_GRAY2BGR);

#if 0
	vector<Vec2f> lines;
	HoughLines(dst, lines, 1, CV_PI / 180, 100, 0, 0);

	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(cdst, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
	}
#else
	vector<Vec4i> lines;
	HoughLinesP(dst, lines, 1, CV_PI / 180, 50, 50, 10);
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		if (i == 0) {
			xStart_first = l[0];
			xEnd_first = l[2];
			xStart_second = l[0];
			xEnd_second = l[2];
			yStart = l[3];
			yEnd = l[1];
		}
		else {
			if (l[0] < xStart_first) {
				xStart_first = l[0];
				xEnd_first = l[2];
				yStart = l[3];
			}
			if (l[2] > xEnd_second) {
				xStart_second = l[0];
				xEnd_second = l[2];
				yEnd = l[1];
			}
		}
		if (showSteps)
		cout << "start x: " << l[0] << " start y: " << l[1] << endl << " end x: " << l[2] << " end y: " << l[3] << endl << "##############" << endl;
		
		line(cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);
	}
#endif

	if (yStart >= yEnd) {
		int temp = yStart - yEnd;

		if (temp >= (yStart*0.08)) {
			yEnd = yStart;
		}
	}
	else {
		int temp = yEnd - yStart;

		if (temp >= (yEnd*0.08)) {
			yStart = yEnd;
		}
	}

	for (size_t i = 0; i < lines.size(); i++) {
		Vec4i l = lines[i];
		if (i == 0) {
			yUpStart = l[1];
			yUpEnd = l[3];
		}
		else {
			if (l[0] > xEnd_first && l[0] < xStart_second && l[2] > xEnd_first && l[2] < xStart_second) {
				if (l[1] < yUpStart && l[3] < yUpEnd) {
					yUpStart = l[1];
					yUpEnd = l[3];
				}
			}
		}
	}

	if (yUpStart >= yUpEnd) {
		int temp = yUpStart - yUpEnd;

		if (temp >= (yUpStart*0.07)) {
			yUpEnd = yUpStart;
		}
	}
	else {
		int temp = yUpEnd - yUpStart;

		if (temp >= (yUpEnd*0.07)) {
			yUpStart = yUpEnd;
		}
	}

	cvtColor(src, distance, CV_GRAY2BGR);

	if (showSteps) {
		cout << "start x: " << xStart_first << " end x: " << xEnd_first << endl << " second x: " << xStart_second << " second end x: " << xEnd_second << endl << "##############" << endl;
		cout << "yStart: " << yStart << " yEnd: " << yEnd;
		//line(src, Point(xEnd_first, yStart), Point(xStart_second, yEnd), Scalar(0, 0, 255), 3, CV_AA);
	}

	line(distance, Point(xEnd_first, yStart), Point(xStart_second, yEnd), Scalar(0, 0, 255), 3, CV_AA);
	line(distance, Point(xEnd_first, yUpStart), Point(xStart_second, yUpEnd), Scalar(0, 0, 255), 3, CV_AA);
	line(distance, Point(xEnd_first, yStart), Point(xEnd_first, yUpEnd), Scalar(0, 0, 255), 3, CV_AA);
	line(distance, Point(xStart_second, yEnd), Point(xStart_second, yUpEnd), Scalar(0, 0, 255), 3, CV_AA);
	namedWindow("source", CV_WINDOW_AUTOSIZE);
	imshow("source", src);

	if (showSteps) {
		namedWindow("blured", CV_WINDOW_AUTOSIZE);
		imshow("blured", blured);
		namedWindow("CannyEdge", CV_WINDOW_AUTOSIZE);
		imshow("CannyEdge", dst);
		namedWindow("detected lines", CV_WINDOW_AUTOSIZE);
		imshow("detected lines", cdst);
	}

	namedWindow("door lines", CV_WINDOW_AUTOSIZE);
	imshow("door lines", distance);
	


	waitKey();

	return 0;
}