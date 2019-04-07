#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include<iostream>     
#include<math.h>

using namespace cv;
using namespace std;

Point2i OC(Mat img);
Point2f Circle_Detect(Mat img, float Threshold);

float Trapezoid_Average(Mat img, Point2i out_left, Point2i out_right, Point2i in_left, Point2i in_right);


unsigned char BV(Mat gray_img);


