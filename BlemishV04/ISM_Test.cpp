
#include <ISM_Test.h>





int gray_Img_Sum(int x, int y, int width, int height, Mat gray) {

	int sum = 0;
	int y_max = y + height;
	int x_max = x + width;
	for (int i = y; i < y_max; i++)
	{
		uchar* inData = gray.ptr<uchar>(i);
		for (int j = x; j < x_max; j++) {
			sum += inData[j];
		}
	}
	return sum;
}


int roi_move(int x_start, int y_start, int Roi_width, int Roi_height, int direction, Mat gray, int size) {

	int sum = 0;
	if (direction == 1) {
		int down = gray_Img_Sum(x_start, y_start + Roi_height - size, Roi_width, size, gray);
		int up = gray_Img_Sum(x_start, y_start - size, Roi_width, size, gray);
		sum = up - down;	
	}
	if (direction == 2) {
		int up = gray_Img_Sum(x_start, y_start, Roi_width, size, gray);
		int down = gray_Img_Sum(x_start, y_start + Roi_height, Roi_width, size, gray);
		sum = down - up ;

	}
	if (direction == 3) {
		int left = gray_Img_Sum(x_start-size, y_start, size, Roi_height, gray);
		int right = gray_Img_Sum(x_start+ Roi_width-size, y_start , size, Roi_height, gray);
		sum = left - right;
	}
	if (direction == 4) {		
		int right = gray_Img_Sum(x_start + Roi_width, y_start, size, Roi_height, gray);
		int left = gray_Img_Sum(x_start , y_start, size, Roi_height, gray);
		sum = right - left ;
	}

	return sum;
}


Point2i OC(Mat gray_img) {

	Point2i center;
	int center_ROI_size = 8;
	int width = gray_img.cols;
	int height = gray_img.rows;
	center.x = width / 2 + 1;
	center.y = height / 2 + 1;
	int rw = (width % center_ROI_size) / 2;
	int rh = (height % center_ROI_size) / 2;
	int x = width / center_ROI_size;
	int y = height / center_ROI_size;

	int part_width = width / center_ROI_size;
	int part_height = height / center_ROI_size;

	int Roi_width = width - part_width *2;
	int Roi_height = height - part_height*2;
	int x_start = part_width;
	int y_start = part_height;
	int init_x = x_start;
	int init_y = y_start;

	int direction = 1;
	int bandSIze = 32;
	int initial_bandSIze = bandSIze;

	int Min_y = bandSIze +1;
	int Max_y = height- bandSIze -1;
	int Min_x = bandSIze +1;
	int Max_x = width - bandSIze -1;


	while (bandSIze > 0){

		int move_up = roi_move(x_start, y_start, Roi_width, Roi_height, 1, gray_img, bandSIze);
		int move_down = roi_move(x_start, y_start, Roi_width, Roi_height, 2, gray_img, bandSIze);
		int move_left = roi_move(x_start, y_start, Roi_width, Roi_height, 3, gray_img, bandSIze);
		int move_right = roi_move(x_start, y_start, Roi_width, Roi_height, 4, gray_img, bandSIze);

		while (move_up > 0 || move_down > 0 || move_left > 0 || move_right > 0) {

			if (move_up > 0 && move_up > move_down) {
				y_start -= bandSIze;
				move_up = roi_move(x_start, y_start, Roi_width, Roi_height, 1, gray_img, bandSIze);
			}
			else if (move_down > 0 && move_down > move_up) {
				y_start += bandSIze;
				move_down = roi_move(x_start, y_start, Roi_width, Roi_height, 2, gray_img, bandSIze);
			}
			if (move_left > 0 && move_left > move_right) {
				x_start -= bandSIze;
				move_left = roi_move(x_start, y_start, Roi_width, Roi_height, 3, gray_img, bandSIze);
			}
			else if (move_right > 0 && move_right > move_left) {
				x_start += bandSIze;
				move_right = roi_move(x_start, y_start, Roi_width, Roi_height, 4, gray_img, bandSIze);
			}

			if (bandSIze != initial_bandSIze|| x_start <= Min_x || x_start + Roi_width >= Max_x || y_start <= Min_y || y_start + Roi_height >= Max_y)
				break;
		}
		bandSIze /= 2;

	}

	center.x += x_start - init_x;
	center.y += y_start - init_y;

	return center;
}


float Trapezoid_Average(Mat img, Point3i Top, Point2i Left, Point3i Down, Point3i Right) {

	int sum = 0,n=0;

	float left_slope = (float)(Top.y - Left.y) / (Top.x - Left.x);
	float right_slope = (float)(Top.y - Right.y) / (Top.x - Right.x);
	float slope2 = (float)(Top.y - Left.y) / (Top.x - Left.x);
	float slope3 = (float)(Top.y - Right.y) / (Top.x - Right.x);



	return (float)sum/n;

}


unsigned char BV(Mat gray_img) {

	Point2i center;
	int center_ROI_size = 16;
	int width = gray_img.cols;
	int height = gray_img.rows;
	center.x = width / 2 + 1;
	center.y = height / 2 + 1;

	int part_width = width / center_ROI_size;
	int part_height = height / center_ROI_size;

	unsigned int sum = gray_Img_Sum(center.x - part_width, center.y - part_height, part_width, part_height, gray_img);

	unsigned char bv = sum / (part_width*part_height);
	return bv;
}


Point2f Circle_Detect(Mat gray_image, float Threshold) {

	unsigned char bv = BV(gray_image);
	Mat img2, img3;

	img2 = gray_image.clone();
	GaussianBlur(img2, img2, Size(9, 9), 2, 2);
	threshold(img2, img3, bv*Threshold, 220, THRESH_BINARY);  //图像二值化，

	namedWindow("detecte circles1", CV_NORMAL);
	imshow("detecte circles1", img3);

	Canny(img3, img3, 50, 100);//边缘检测
							 
	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;
	findContours(img3, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//查找出所有的圆边界

	vector<Point> Circle_edge;
	for (int i = 0; i < contours.size(); i++) {
		vector<Point> ::iterator start = contours[i].begin();
		while (start != contours[i].end()) {
			Circle_edge.push_back(*start);
			++start;
		}
	}

	//标准圆在图片上一般是椭圆，所以采用OpenCV中拟合椭圆的方法求解中心
	Mat pointsf;
	Mat(Circle_edge).convertTo(pointsf, CV_32F);
	RotatedRect box = fitEllipse(pointsf);

	return box.center;

}




/*

Point2i OC(Mat gray_img) {

Point2i center;
int center_ROI_size = 4;
int width = gray_img.cols;
int height = gray_img.rows;
center.x = width / 2 + 1;
center.y = height / 2 + 1;
int rw = (width % center_ROI_size) / 2;
int rh = (height % center_ROI_size) / 2;
int x = width / center_ROI_size;
int y = height / center_ROI_size;
int Roi_width = 2 * x;
int Roi_height = 2 * y;
int x_start = rw + x*(center_ROI_size / 2 - 1);
int y_start = rh + y*(center_ROI_size / 2 - 1);
int Min_y = y_start - y / 2;
int Max_y = y_start + y / 2;
int Min_x = x_start - x / 2;
int Max_x = x_start + x / 2;

int direction = 1;
int bandSIze = 32;
int initial_bandSIze = bandSIze;
while (bandSIze > 0){

int move_up = roi_move(x_start, y_start, Roi_width, Roi_height, 1, gray_img, bandSIze);
int move_down = roi_move(x_start, y_start, Roi_width, Roi_height, 2, gray_img, bandSIze);
int move_left = roi_move(x_start, y_start, Roi_width, Roi_height, 3, gray_img, bandSIze);
int move_right = roi_move(x_start, y_start, Roi_width, Roi_height, 4, gray_img, bandSIze);

while (move_up > 0 || move_down > 0 || move_left > 0 || move_right > 0) {

if (move_up > 0 && move_up > move_down) {
y_start -= bandSIze;
move_up = roi_move(x_start, y_start, Roi_width, Roi_height, 1, gray_img, bandSIze);
}
if (move_down > 0 && move_down > move_up) {
y_start += bandSIze;
move_down = roi_move(x_start, y_start, Roi_width, Roi_height, 2, gray_img, bandSIze);
}
if (move_left > 0 && move_left > move_right) {
x_start -= bandSIze;
move_left = roi_move(x_start, y_start, Roi_width, Roi_height, 3, gray_img, bandSIze);
}
if (move_right > 0 && move_right > move_left) {
x_start += bandSIze;
move_right = roi_move(x_start, y_start, Roi_width, Roi_height, 4, gray_img, bandSIze);
}

if (bandSIze != initial_bandSIze||x_start <= Min_x || x_start >= Max_x || y_start <= Min_y || y_start >= Max_y)
break;
}
bandSIze /= 2;

}

//move_up = roi_move(x_start, y_start, Roi_width, Roi_height, 1, gray_img, 2);
//move_down = roi_move(x_start, y_start, Roi_width, Roi_height, 2, gray_img, 2);
//move_left = roi_move(x_start, y_start, Roi_width, Roi_height, 3, gray_img, 2);
//move_right = roi_move(x_start, y_start, Roi_width, Roi_height, 4, gray_img, 2);

//if (move_up > 0 && move_up>move_down) {
//	y_start -= 2;
//}
//if (move_down > 0 && move_down>move_up) {
//	y_start += 2;
//}
//if (move_left > 0 && move_left>move_right) {
//	x_start -= 2;
//}
//if (move_right > 0 && move_right>move_left) {
//	x_start += 2;
//}

//move_up = roi_move(x_start, y_start, Roi_width, Roi_height, 1, gray_img, 1);
//move_down = roi_move(x_start, y_start, Roi_width, Roi_height, 2, gray_img, 1);
//move_left = roi_move(x_start, y_start, Roi_width, Roi_height, 3, gray_img, 1);
//move_right = roi_move(x_start, y_start, Roi_width, Roi_height, 4, gray_img, 1);

//if (move_up > 0 && move_up>move_down) {
//	y_start -= 1;
//}
//if (move_down > 0 && move_down>move_up) {
//	y_start += 1;
//}
//if (move_left > 0 && move_left>move_right) {
//	x_start -= 1;
//}
//if (move_right > 0 && move_right>move_left) {
//	x_start += 1;
//}

center.x += x_start - (rw + x * (center_ROI_size / 2 - 1));
center.y += y_start - (rh + y * (center_ROI_size / 2 - 1));

return center;
}



*/




























/*

int move_up = roi_move(x_start, y_start, Roi_width, Roi_height, 1, gray_img,2);
int move_down = roi_move(x_start, y_start, Roi_width, Roi_height, 2, gray_img, 2);
int move_left = roi_move(x_start, y_start, Roi_width, Roi_height, 3, gray_img, 2);
int move_right = roi_move(x_start, y_start, Roi_width, Roi_height, 4, gray_img, 2);

while (move_up > 0 || move_down > 0|| move_left> 0 || move_right> 0) {

if (move_up > 0&& move_up>move_down) {
y_start-=2;
move_up = roi_move(x_start, y_start, Roi_width, Roi_height, 1, gray_img, 2);
}
if (move_down > 0&& move_down>move_up) {
y_start += 2;
move_down = roi_move(x_start, y_start, Roi_width, Roi_height, 2, gray_img, 2);
}
if (move_left > 0&& move_left>move_right) {
x_start -= 2;
move_left = roi_move(x_start, y_start, Roi_width, Roi_height, 3, gray_img, 2);
}
if (move_right > 0&& move_right>move_left) {
x_start += 2;
move_right = roi_move(x_start, y_start, Roi_width, Roi_height, 4, gray_img, 2);
}

if (x_start <= Min_x || x_start >= Max_x || y_start <= Min_y || y_start >= Max_y)
break;
}

if (roi_move(x_start, y_start, Roi_width, Roi_height, 1, gray_img, 1) > 0)
y_start--;
else if (roi_move(x_start, y_start, Roi_width, Roi_height, 2, gray_img, 1) > 0)
y_start++;
if (roi_move(x_start, y_start, Roi_width, Roi_height, 3, gray_img, 1) > 0)
x_start--;
else if (roi_move(x_start, y_start, Roi_width, Roi_height, 4, gray_img, 1) > 0)
x_start++;

center.x += x_start - (rw + x * (center_ROI_size / 2 - 1));
center.y += y_start - (rh + y * (center_ROI_size / 2 - 1));

*/