/*
 * Danny.cpp
 *
 *  Created on: 2018年12月4日
 *      Author: Administrator
 */

#include "Danny.h"

namespace danny {

Danny::Danny() {
	// TODO Auto-generated constructor stub

}

Danny::~Danny() {
	// TODO Auto-generated destructor stub
}

Mat Danny::danny_blur_blur(Mat img, int weight) {
	Mat dst = img;//.clone();
	blur(img, dst, Size(weight, weight), Point(-1, -1));
	return dst;
}

Mat Danny::danny_blur_caussian(Mat img, int weight) {
	Mat dst = img;//.clone();
	GaussianBlur(img, dst, Size(weight, weight), 0, 0);
	return dst;
}


Mat Danny::danny_threshold(Mat img, int threshold_value,int opt) {
	Mat dst = img;//.clone();
	threshold(img, dst,threshold_value , 255, opt);
	return dst;
}

Mat Danny::danny_erosion(Mat img, int opt_k,int erosion_size) {
	Mat dst = img;//.clone();
	Mat element = getStructuringElement(opt_k,
			Size(2 * erosion_size + 1, 2 * erosion_size + 1),	//内核大小
			Point(erosion_size, erosion_size));	//瞄点，中心位置
	erode(img, dst, element);
	return dst;
}

Mat Danny::danny_dilation(Mat img, int opt_k, int dilatoin_size) {
	Mat dst = img;//.clone();
	Mat element = getStructuringElement(opt_k,
			Size(2 * dilatoin_size + 1, 2 * dilatoin_size + 1),	//内核大小
			Point(dilatoin_size, dilatoin_size));	//瞄点，中心位置
	dilate(img, dst, element);
	return dst;
}

Mat Danny::danny_morphologyEx(Mat img, int opt, int opt_k, int element_size) {
	Mat dst = img;//.clone();
	Mat element = getStructuringElement(opt_k,
			Size(2 * element_size+ 1, 2 * element_size+ 1),	//内核大小
			Point(element_size, element_size));	//瞄点，中心位置
	//运行指定形态学操作
	morphologyEx( img, dst, opt, element);
	return dst;
}

Rect Danny::danny_find_max_rect(Mat img) {
	Mat image = img.clone();
//	danny_threshold(image, 240, 3); //阀处理
//	danny_threshold(image, 240, 3); //阀处理
//	cv::cvtColor(image, image, CV_RGB2GRAY); //灰度化

	vector<vector<Point> > contours;
	cv::findContours(image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //找轮廓

	vector<vector<Point> > contours_poly( contours.size() );
	vector<Rect> boundRect( contours.size() );

	for (int i = 0; i < contours.size(); ++i) {
		approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true ); //多边形拟合处理
		boundRect[i] = boundingRect( Mat(contours_poly[i]) );
	}

	int max_index = 0;
	int max_next_index= 0;
	int max_value = 0;
//	cout << "-------------------- " << std::endl;
	for (int i = 0; i < boundRect.size(); i++) {
		int tmp = boundRect[i].height * boundRect[i].width;
		if (tmp > max_value) {
			max_value = tmp;
			max_next_index = max_index;
			max_index = i;
		}
//		cout <<i<<" " << boundRect[i].width << " " << boundRect[i].height <<" == " << boundRect[i].x << "-"<< boundRect[i].y<< std::endl;
	}
//	cout << "rect size = " << boundRect.size() << std::endl;
	//cout << "max1 ret " << boundRect[max_index].width << " " << boundRect[max_index].height <<" == " << boundRect[max_index].x << "-"<< boundRect[max_index].y<< std::endl;
	//cout << "max2 ret " << boundRect[max_next_index].width << " " << boundRect[max_next_index].height <<\
	//		" == " << boundRect[max_next_index].x << "-"<< boundRect[max_next_index].y<< std::endl;
	//Mat image_roi = src_img(boundRect[max_index]);
	if (boundRect.size() > 0) {
		return boundRect[max_index];
	} else {
		return Rect(0,0,0,0);
	}
}

Rect Danny::danny_find_ok_rect(Mat img) {
	Mat image = img.clone();
	vector<vector<Point> > contours;
	cv::findContours(image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //找轮廓
	vector<vector<Point> > contours_poly( contours.size() );
	vector<Rect> boundRect( contours.size() );
	for (int i = 0; i < contours.size(); ++i) {
		approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true ); //多边形拟合处理
		boundRect[i] = boundingRect( Mat(contours_poly[i]) );
	}

	int max_index = 0;
	int max_next_index= 0;
	int max_value = 0;
//	cout << "-------------------- " << std::endl;
	for (int i = 0; i < boundRect.size(); i++) {
		if ((boundRect[i].width >= 45) && \
			(boundRect[i].width <=54 ) && \
		    (boundRect[i].height>= 45) && \
			(boundRect[i].height<=54 )) {
//			cout <<i<<" " << boundRect[i].width << " " << boundRect[i].height <<" == " << boundRect[i].x << "-"<< boundRect[i].y<< std::endl;
			return boundRect[i];
		}
	}
//	cout << "rect size = " << boundRect.size() << std::endl;
	//cout << "max1 ret " << boundRect[max_index].width << " " << boundRect[max_index].height <<" == " << boundRect[max_index].x << "-"<< boundRect[max_index].y<< std::endl;
	//cout << "max2 ret " << boundRect[max_next_index].width << " " << boundRect[max_next_index].height <<\
	//		" == " << boundRect[max_next_index].x << "-"<< boundRect[max_next_index].y<< std::endl;
	//Mat image_roi = src_img(boundRect[max_index]);

	Rect rst(0,0,1,1);
	return rst;
}

} /* namespace danny */
