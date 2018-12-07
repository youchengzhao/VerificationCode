#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include <string>
#include "Danny.h"

using namespace cv;
using namespace std;
using namespace danny;

Mat danny_func(Mat img) {
	Danny danny;
	Mat img_gray;
	cvtColor(img, img_gray, CV_RGB2GRAY); //灰度处理
	Mat dst = danny.danny_morphologyEx(img_gray, MORPH_GRADIENT, MORPH_RECT, 1);
	dst = danny.danny_threshold(dst, 200, 0); //阀值处理--变黑白
	return dst;
}

int danny_func_id(Mat img) {
	unsigned char *p;
	const int channels = img.channels(); //检测一下通道数，灰度图为1，彩色图为3
	int rows, cols;
	int tmp[400][2];
	int count = 0;
	rows = img.rows;
	cols = img.cols * channels;
	cout << img.rows << " -- " << img.cols << std::endl;
	for (int i = 0; i < rows; i++) {
		const unsigned char* prs = img.ptr(i);
		int count_x = 0;
		int j = 0;
		for (j = 0; j < cols; j++) {
			if (prs[j] > 200) {
				if (count_x == 0) {
					tmp[count][0] = j;
				}
				count_x++;
			} else {
				if (count_x > 10) {
					tmp[count][1] = j;
				}
			}
		}
		if (count_x > 20) {
			tmp[count][1] = j;
			cout << "end " << tmp[count][0] << "-" << tmp[count][1] << "  "
					<< count_x << std::endl;
			count++;
		}
	}
}

void test() {
	Mat original = imread("dd.jpg");
	namedWindow("My original");
	imshow("My original", original);
	Mat gray = original;
	cv::cvtColor(gray, gray, CV_RGB2GRAY); //灰度化

	int thresh_size = (100 / 4) * 2 + 1; //自适应二值化阈值
	adaptiveThreshold(gray, gray, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C,
			CV_THRESH_BINARY_INV, thresh_size, thresh_size / 3);
	morphologyEx(gray, gray, MORPH_OPEN, Mat()); //形态学开运算去噪点

	vector<vector<Point> > contours;
	cv::findContours(gray, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //找轮廓
	vector<vector<Point> > contours1;
	for (int i = 0; i < contours.size(); ++i) {
		if (contours[i].size() > 200) //将比较小的轮廓剔除掉
				{
			contours1.push_back(contours[i]);
		}
	}
	/*可以加入以下功能*/
	//vector<vector<Point>> contours2;
	//vector<Point> approx_poly;//存放顶点
	//for (int i = 0; i < contours1.size(); ++i)
	//{
	//double eps = contours1[i].size()*0.08;//精度
	//approxPolyDP(contours1[i], approx_poly, eps, true);//用另一条顶点较少的曲线来逼近一条曲线或者一个多边形
	//if ((approx_poly.size() != 4) || (!isContourConvex(approx_poly)))//提取只有4个顶点的轮廓和凸多边形
	//{
	//	continue;
	//}
	//else {
	//contours2.push_back(contours1[i]);
	//}
	//}
	Mat hole(gray.size(), CV_8U, Scalar(0)); //遮罩图层
	cv::drawContours(hole, contours1, -1, Scalar(255), CV_FILLED); //在遮罩图层上，用白色像素填充轮廓
	namedWindow("My hole");
	imshow("My hole", hole);
	Mat crop(original.rows, original.cols, CV_8UC3);
	original.copyTo(crop, hole); //将原图像拷贝进遮罩图层
	namedWindow("My warpPerspective");
	imshow("My warpPerspective", crop);
	waitKey(0);
}

int test_main() {
	//String imageName("cc.jpg"); // by default
	//String imageName("haha.png"); // by default
	String url = "";
//	String imageName("ee.jpg"); // by default
	String imageName("tmp.png"); // by default
	Mat image;
	Mat img_gray;
	Danny danny;
	image = imread(imageName, IMREAD_COLOR); // Read the file
	if (image.empty())                      // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
	//imshow( "Display window", image );                // Show our image inside it.
	//waitKey(0); // Wait for a keystroke in the window

	//Mat dst1 = danny.danny_erosion(image, MORPH_RECT,1);
	//Mat dst = danny.danny_dilation(image, MORPH_RECT,1);
	//Mat dst = danny.danny_morphologyEx(image,MORPH_GRADIENT, MORPH_RECT,1);
	//Mat dst = danny.danny_morphologyEx(image,MORPH_TOPHAT, MORPH_CROSS,5);
	//Mat dst = danny.danny_morphologyEx(image,MORPH_BLACKHAT, MORPH_CROSS,5);

	//高斯平滑
	//GaussianBlur(image, image, Size(3,3), 0, 0, BORDER_DEFAULT);

	Mat dst = danny_func(image);
	vector<vector<Point> > contours;
	cv::findContours(dst, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //找轮廓
	cout << "--- " << contours.size() << std::endl;
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());

	for (int i = 0; i < contours.size(); ++i) {
		if (contours[i].size() < 100)
			continue;
		cout << i << " size " << contours[i].size() << std::endl;
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true); //多边形拟合处理
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
		cout << contours[i].data() << std::endl;
		cout << boundRect[i].width << "," << boundRect[i].height << "坐标"
				<< boundRect[i].x << " - " << boundRect[i].y << std::endl;
	}

	//danny_func_id(dst);
	imshow("Display window", dst);              // Show our image inside it.
	waitKey(0); // Wait for a keystroke in the window
	return 0;
}

int main(int argc, char** argv) {

	if (argc < 2) {
		cout << "exe  *.png" << std::endl;
		return 0;
	}

	//String imageName("tmp.png"); // by default
	String imageName = argv[1];
	Mat image;
	image = imread(imageName, IMREAD_COLOR); // Read the file
//	namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.

	Danny danny;

	Mat img_tmp0 = image.clone();
	danny.danny_threshold(image, 240, 3);
	cv::cvtColor(image, image, CV_RGB2GRAY); //灰度化
	Mat img_tmp1 = img_tmp0(danny.danny_find_max_rect(image));

	Mat img_tmp2 = img_tmp1.clone();

	danny.danny_threshold(img_tmp1, 250, 1);
	cv::cvtColor(img_tmp1, img_tmp1, CV_RGB2GRAY); //灰度化
	Mat img_tmp3 = img_tmp2(danny.danny_find_max_rect(img_tmp1));
	Mat img_tmpx = img_tmp3.clone();

	//cvtColor(img_tmp3, img_tmp3, CV_RGB2GRAY); //灰度处理
	int flag = 0;
	int  lap = 250;
	Rect r1,r2;
	cv::cvtColor(img_tmp3, img_tmp3, CV_RGB2GRAY); //灰度化
	while (1) {
		Mat img_tmp_i = img_tmp3.clone();
		img_tmp_i = danny.danny_threshold(img_tmp_i, lap, 1); //阀值处理--变黑白
		img_tmp_i = danny.danny_morphologyEx(img_tmp_i, MORPH_GRADIENT,
				MORPH_RECT, 1);
		Rect res = danny.danny_find_ok_rect(img_tmp_i);
		if (res.height != 1){
			//cout << "{\"index\": 1, \"width\": "<<res.width<<", \"height\": "<<res.height<<", \"x\": "<<res.x<<", \"y\":" <<res.y<< "}";
			r1 = res;
			flag ++;
			break;
		}
		lap -=10;
		if (lap <=20)
			break;
	}

	img_tmp3 = img_tmpx.clone();

	lap = 20;
	cv::cvtColor(img_tmp3, img_tmp3, CV_RGB2GRAY); //灰度化
	while (1) {
		Mat img_tmp_i = img_tmp3.clone();
		img_tmp_i = danny.danny_threshold(img_tmp_i, lap, 1); //阀值处理--变黑白
		img_tmp_i = danny.danny_morphologyEx(img_tmp_i, MORPH_GRADIENT,
				MORPH_RECT, 1);
		Rect res = danny.danny_find_ok_rect(img_tmp_i);
		if (res.height != 1){
			//cout << "{\"index\": 2, \"width\": "<<res.width<<", \"height\": "<<res.height<<", \"x\": "<<res.x<<", \"y\":" <<res.y<< "}";
			r2 = res;
			flag ++;
			break;
		}
		lap +=10;
		if (lap >=250)
			break;
	}

	if (flag >= 2) {
		cout << "{\"state\": 1, \"data\": [" ;
		cout << "{\"index\": 1, \"width\": "<<r1.width<<", \"height\": "<<r1.height<<", \"x\": "<<r1.x<<", \"y\":" <<r1.y<< "},";
		cout << "{\"index\": 2, \"width\": "<<r2.width<<", \"height\": "<<r2.height<<", \"x\": "<<r2.x<<", \"y\":" <<r2.y<< "}";
		cout << "]}" ;
	} else {
		cout << "{\"state\": 0, \"data\": null}" ;
	}

	return 0;
}

