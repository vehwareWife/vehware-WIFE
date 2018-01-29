#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <vector>
#include <stack>
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <math.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>
# define M_PI           3.14159265358979323846  /* pi */
# define SIZE           14  /* pi */



using namespace std;
using namespace cv;

int lbl;
void swap(double *xp, double *yp);
const int MAX =3;
int searchNearest(int anArray[], int key);
void bubbleSort(double arr[], int n);
void search4CompNeighbors(Mat1i& image, int label, int row, int column);
int componentFinder(Mat1i& LB);
double labelArray[SIZE];
double labelArray2[SIZE];
int labelMatrix[10000][10000];
double getDistance(int a, int b, int x, int y);
void dotDetecter(int offset1, int offset2, Mat original_image);
void labeler(int offset1, int offset2, Mat gray, Mat1i imgw);


int main(int argc, char **argv) {

	

			const char *filename = "../../../TestingImages3/6.png";
			//the exact results are given by const char *filename = "../../../TestingImages3/2.bmp"; , const char *filename = "../../../TestingImages3/4.png"; , const char *filename = "../../../TestingImages3/6.png";

			int array_x[10000];
			int array_y[10000];
			int x_ctr = 0;
			int y_ctr = 0;

			int sum1_x = 0;
			int sum1_y = 0;

			int sum2_x = 0;
			int sum2_y = 0;

			int distances[3] = {160,365,576 };// distances are true for comparing 2.bmp, 4.png and 6.png because they are captured by same camera height and angle

			//Load two copies of the image. One to leave as the original, and one to be modified.
			//Done for display purposes only
			//Use CV_LOAD_IMAGE_GRAYSCALE for greyscale images
			Mat original_image = imread(filename);
			Mat modified_image = imread(filename);
			//Mat hsi_image = imread(filename2);

			//Create a pointer so that we can quickly toggle between which image is being displayed
			Mat *image = &original_image;

			//Check that the images loaded
			if (!original_image.data || !modified_image.data) {
				cout << "ERROR: Could not load image data." << endl;
				return -1;
			}

			//Create the display window
			namedWindow("Unix Sample Skeleton");
			//cout << "make selection";
			//Replace center third of the image with white
			//This can be replaced with whatever filtering you need to do.
			int offset1 = image->rows;
			int offset2 = image->cols;
			imshow("Original Image", *image);
			
		//	modified_image=modified_image.grayScale

			dotDetecter(offset1, offset2, original_image);

			Mat gray;

			// convert RGB image to gray
			cvtColor(original_image, gray, CV_BGR2GRAY);
			Mat1i imgw = Mat1i(gray.rows, gray.cols, 0);
			labeler(offset1, offset2, gray, imgw);

			componentFinder(imgw);

			for (size_t i = 0; i < SIZE; i++)
			{
				labelArray2[i] = labelArray[i];
			}

			sort(labelArray, labelArray + SIZE);
			cout << "---------";
			
			int firstDot = labelArray[SIZE-1];
			int secondDot = labelArray[SIZE-2];

			int lbl_values[2];
			
			int lbl_vl_ctr = 0;
			for (size_t i = 0; i < SIZE; i++)
			{
				if (labelArray2[i] == firstDot || labelArray2[i] == secondDot) {

					lbl_values[lbl_vl_ctr] = i;
					lbl_vl_ctr++;
				}

			}

			/*
			for (int s = 0; s < lbl; s++) {
			cout << "Pixel Sizes of the pigs before Operations: "<<labelArray[s] << endl;
				
			}

			cout << "------------"<<endl;
			cout << "There are " << lbl << " pigs in the image" << endl;
			
			*/

			


			

			
			for (int row = 0; row < offset1; ++row) {
				for (int column = 0; column < offset2; ++column) {
					if (labelMatrix[row][column] == lbl_values[0]) {
						//cout << "a";
						sum1_x += row;
						sum1_y += column;
						x_ctr++;

					}
					if (labelMatrix[row][column] == lbl_values[1]){
						
						sum2_x += row;
						sum2_y += column;
						y_ctr++;

					}

				}

			}




			double sum1x_avg = sum1_x / x_ctr;
			double sum1y_avg = sum1_y / x_ctr;

			double sum2x_avg = sum2_x / y_ctr;
			double sum2y_avg = sum2_y / y_ctr;
			
			//cout << sum1x_avg << endl;
			//cout << sum1y_avg << endl;

			double distance2 = getDistance(sum1x_avg, sum2x_avg, sum1y_avg, sum2y_avg);

			cout << "pixel distance is= " << distance2<<endl;

			
			original_image.at<Vec3b>(sum1x_avg, sum1y_avg)[0] = 0;
			original_image.at<Vec3b>(sum1x_avg, sum1y_avg+1)[1] = 255;
			original_image.at<Vec3b>(sum1x_avg, sum1y_avg+2)[2] = 0;
			original_image.at<Vec3b>(sum1x_avg+1, sum1y_avg)[0] = 0;
			original_image.at<Vec3b>(sum1x_avg+1, sum1y_avg+1)[1] = 255;
			original_image.at<Vec3b>(sum1x_avg+1, sum1y_avg+2)[2] = 0;

			original_image.at<Vec3b>(sum1x_avg+2, sum1y_avg)[0] = 0;
			original_image.at<Vec3b>(sum1x_avg+2, sum1y_avg + 1)[1] = 255;
			original_image.at<Vec3b>(sum1x_avg+2, sum1y_avg + 2)[2] = 0;
			original_image.at<Vec3b>(sum1x_avg + 3, sum1y_avg)[0] = 0;
			original_image.at<Vec3b>(sum1x_avg + 3, sum1y_avg + 1)[1] = 255;
			original_image.at<Vec3b>(sum1x_avg + 3, sum1y_avg + 2)[2] = 0;

			original_image.at<Vec3b>(sum2x_avg, sum2y_avg)[0] = 51;
			original_image.at<Vec3b>(sum2x_avg, sum2y_avg + 1)[1] = 255;
			original_image.at<Vec3b>(sum2x_avg, sum2y_avg + 2)[2] = 255;
			original_image.at<Vec3b>(sum2x_avg + 1, sum2y_avg)[0] = 51;
			original_image.at<Vec3b>(sum2x_avg + 1, sum2y_avg + 1)[1] = 255;
			original_image.at<Vec3b>(sum2x_avg + 1, sum2y_avg + 2)[2] = 255;

			original_image.at<Vec3b>(sum2x_avg+2, sum2y_avg)[0] = 51;
			original_image.at<Vec3b>(sum2x_avg+2, sum2y_avg + 1)[1] = 255;
			original_image.at<Vec3b>(sum2x_avg+2, sum2y_avg + 2)[2] = 255;
			original_image.at<Vec3b>(sum2x_avg + 3, sum2y_avg)[0] = 51;
			original_image.at<Vec3b>(sum2x_avg + 3, sum2y_avg + 1)[1] = 255;
			original_image.at<Vec3b>(sum2x_avg + 3, sum2y_avg + 2)[2] = 255;

			imshow("Centers", original_image);


			for (int i = 0; i < 3; i++) {

			}
			
			int nearest = searchNearest(distances, distance2);
			cout << "The distance is: " << (nearest+1)*2 <<" feet"<< endl;

			//Display loop
			bool loop = true;
			while (loop) {
				imshow("Unix Sample Skeleton", *image);

				switch (cvWaitKey(15)) {
				case 27:  //Exit display loop if ESC is pressed
					loop = false;
					break;
				case 32:  //Swap image pointer if space is pressed
					if (image == &original_image) image = &modified_image;
					//else if (image == &modified_image) image = &hsi_image;
					else image = &original_image;
					break;
				default:
					break;
				}
			}

			
	

	
			
		}
		

		double getDistance(int a, int b, int x, int y) {

			int calculate = (a - x)*(a - x) + (b - y)*(b - y);

			return sqrt(calculate);

		}
		void labeler(int offset1, int offset2, Mat gray, Mat1i imgw) {
			


			for (int row = 0; row < offset1; ++row) {
				for (int column = 0; column < offset2; ++column) {
					if (gray.at<uchar>(row, column) > 40) {

						imgw(row, column) = 50;

					}

				}

			}
		}
		void search4CompNeighbors(Mat1i& image, int label, int row, int column)
		{

			labelArray[label]++;

			labelMatrix[row][column] = label;
			image(row, column) = label;

			

			if ((row - 1 > 0) && image(row - 1, column) == 50) {
				search4CompNeighbors(image, label, row - 1, column);
			}
			if ((row + 1 < image.rows) && image(row + 1, column) == 50) {
				search4CompNeighbors(image, label, row + 1, column);
			}
			if ((column - 1 > 0) && image(row, column - 1) == 50) {
				search4CompNeighbors(image, label, row, column - 1);
			}
			if ((column + 1 < image.cols) && image(row, column + 1) == 50) {
				search4CompNeighbors(image, label, row, column + 1);
			}
			


		}
		int componentFinder(Mat1i& image)
		{
			int label = 0;
			int var = 50;

			for (int row = 0; row < image.rows; ++row) {
				for (int column = 0; column < image.cols; ++column) {
					if (image(row, column) == 50) {
						label++;
						search4CompNeighbors(image, label, row, column);
						
					//	cout << "label ="<<label << endl;
				
						var++;
					}

				}
			}
			lbl = label;
			return label;
		}
		void dotDetecter(int offset1, int offset2, Mat original_image) {

			double b;
			double g;
			double r;

			for (int a = 0; a < offset1; a++) {

				for (int j = 0; j < offset2; j++) {



					b = original_image.at<Vec3b>(a, j)[0];
					g = original_image.at<Vec3b>(a, j)[1];
					r = original_image.at<Vec3b>(a, j)[2];


					if (r> 200 && b<=190 && g<=170) {
							
					//	cout << original_image.at<Vec3b>(a, j)<<endl;
						//cout << "---------------------------";

							original_image.at<Vec3b>(a, j)[0] = 255;
							original_image.at<Vec3b>(a, j)[1] = 255;
							original_image.at<Vec3b>(a, j)[2] = 255;
						
						

					}
					else {
						original_image.at<Vec3b>(a, j)[0] = 0;
						original_image.at<Vec3b>(a, j)[1] = 0;
						original_image.at<Vec3b>(a, j)[2] = 0;
					}




				}

			}

		}
		void swap(double *xp, double *yp)
		{
			double temp = *xp;
			*xp = *yp;
			*yp = temp;
		}

		void bubbleSort(double arr[], int n)
		{
			int i, j;
			for (i = 0; i < n - 1; i++)
				for (j = 0; j < n - i - 1; j++)
					if (arr[j] > arr[j + 1])
						swap(&arr[j], &arr[j + 1]);

		}


	

		int searchNearest(int anArray[], int key)
		{
			int value = abs(key - anArray[0]);
			int num = 0;

			for (int x = 0; x < MAX; x++)
			{
				if (value > abs(key - anArray[x]))
				{
					value = abs(key - anArray[x]);
					num = x;
				}
			}
			


			return num;

		}
