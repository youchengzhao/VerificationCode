/*
 * Danny.h
 *
 *  Created on: 2018年12月4日
 *      Author: Administrator
 */

#ifndef SRC_DANNY_H_
#define SRC_DANNY_H_

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

using namespace cv;
using namespace std;
namespace danny {

class Danny {
public:
	Danny();
	virtual ~Danny();

//============模糊处理================//
	//归一化块滤波器:最简单的滤波器， 输出像素值是核窗口内像素值的 均值 ( 所有像素加权系数相等)
	// img 输入， weight 权重
	Mat danny_blur_blur(Mat img, int weight) ;

	//高斯滤波器 :高斯滤波是将输入数组的每一个像素点与 高斯内核 卷积将卷积和当作输出像素值，速度较慢
	// img 输入， weight 权重
	Mat danny_blur_caussian(Mat img, int weight) ;

//============阀值处理================//
	//归一化块滤波器:最简单的滤波器， 输出像素值是核窗口内像素值的 均值 ( 所有像素加权系数相等)
	// img 输入， threshold_value 阀值
	// opt 0: 二进制阈值; 1: 反二进制阈值; 2: 截断阈值; 3: 0阈值; 4: 反0阈值
	Mat danny_threshold(Mat img, int threshold_value,int opt) ;


//============膨胀，腐蚀处理================//
	//腐蚀：它提取的是内核覆盖下的相素最小值
	//opt_k 内核形状： MORPH_RECT-矩形，MORPH_CROSS-交叉形， MORPH_ELLIPSE-椭圆形
	//img 输入， erosion_size 内核大小
	Mat danny_erosion(Mat img,int opt_k, int erosion_size) ;

	//膨胀：它提取的是内核覆盖下的相素最大值
	//opt_k 内核形状： MORPH_RECT-矩形，MORPH_CROSS-交叉形， MORPH_ELLIPSE-椭圆形
	//img 输入， erosion_size 内核大小
	Mat danny_dilation(Mat img, int opt_k,int dilation_size) ;


//============形态学变换处理================//
	//opt 运算模式 :MORPH_OPEN-开运算，MORPH_CLOSE-闭运算，MORPH_GRADIENT-形态梯度，
	//MORPH_TOPHAT-顶帽，MORPH_BLACKHAT-黑帽
	//opt_k 内核形状： MORPH_RECT-矩形，MORPH_CROSS-交叉形， MORPH_ELLIPSE-椭圆形
	//element_size -- 内核大小
	Mat danny_morphologyEx(Mat img, int opt,int opt_k,int element_size) ;


//============Canny算子找边界换处理================//
	//opt 运算模式 :MORPH_OPEN-开运算，MORPH_CLOSE-闭运算，MORPH_GRADIENT-形态梯度，
	//element_size -- 内核大小
	//Mat danny_Canny(Mat img, int opt,int opt_k,int element_size) ;

//============找到最大的矩形================//
	Rect danny_find_max_rect(Mat img) ;
	Rect danny_find_ok_rect(Mat img) ;


};

} /* namespace danny */

#endif /* SRC_DANNY_H_ */
