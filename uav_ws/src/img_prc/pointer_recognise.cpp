#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

double pointer_recognise(cv::Mat meterImage)
{
	Mat srcImage;
	Mat grayImage;

	//读取图片
	srcImage = meterImage;
	if (!srcImage.data)
	{
		printf("读取图片错误，请确定目录下是否有imread函数指定的图片存在！ \n");
		return -1;
	}


	//图像二值化
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	blur(grayImage, grayImage, Size(3, 3));
	threshold(grayImage, grayImage, 60, 255, THRESH_BINARY);
	//waitKey();

	
	dilate(grayImage, grayImage, getStructuringElement(MORPH_RECT, Size(3, 3)), Point(-1, -1), 4);
	erode(grayImage, grayImage, getStructuringElement(MORPH_RECT, Size(3, 3)), Point(-1, -1), 4);
	imshow("P_腐蚀膨胀后二值图像", grayImage);
	//waitKey();

	//canny边缘检测
	Mat cannyMat_output1;
	Canny(grayImage, cannyMat_output1, 80, 80 * 2, 3);
	imshow("P_canny图像1", cannyMat_output1);
	//waitKey();

	//记录canny边缘上的所有点
	int a = cannyMat_output1.rows;;
	int b = cannyMat_output1.cols;
	vector<Point>points;
	int N_point = 0;
	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j <b; j++)
		{
			int m = cannyMat_output1.at<uchar>(i, j);
			//printf("%d行%d列像素值为%d\n", i, j, m);
			if (m==255)
			{
				N_point = N_point + 1;
				Point point;
				point.x = j;
				point.y = i;
				points.push_back(point);
			}
		}		
	}
	Mat image_point=Mat::zeros(cannyMat_output1.size(), CV_8UC3);
	for (int i = 0; i < N_point; i++)
	{
		circle(image_point, points[i], 3, cv::Scalar(255, 0, 0), 2);
	}
	imshow("轮廓上的所有点", image_point);
	//waitKey();
	//找到包围这些点的最小面积旋转矩形
	cv::RotatedRect maxRect0 = cv::minAreaRect(points);
	Point2f vertex[4];
	maxRect0.points(vertex);
	for (int i = 0; i < 4; i++)
	{
		line(cannyMat_output1, vertex[i], vertex[(i + 1) % 4], Scalar(100, 200, 211), 2, 8);
	}
	cv::imshow("00电表最小面积外包矩形", cannyMat_output1);
	//找到最小外接矩形保留电表区域
	cv::Rect Rect0 = boundingRect(points);
	Mat meter_Image = Mat(srcImage, Rect0);//根据这个区域，保留原图像
	imshow("电表区域", meter_Image);
	//waitKey();

	//透视变换
	vector<Point2f> srcTri, dstTri(4), point_1234(4);
	int x0, y0;
	x0 = meter_Image.rows / 2;
	y0 = meter_Image.cols / 2;
	//判断四个点顺序，并依照正确顺序附给另一个向量
	for (int i = 0; i < 4; i++)
	{
		if ((vertex[i].x<x0) && (vertex[i].y<y0))
		{
			point_1234[0] = vertex[i];
		}
		if ((vertex[i].x>x0) && (vertex[i].y<y0))
		{
			point_1234[1] = vertex[i];
		}
		if ((vertex[i].x<x0) && (vertex[i].y>y0))
		{
			point_1234[2] = vertex[i];
		}
		if ((vertex[i].x>x0) && (vertex[i].y>y0))
		{
			point_1234[3] = vertex[i];
		}
	}
	//定义透视变换矩阵
	int l = point_1234[1].x - point_1234[0].x;
	dstTri[0] = point_1234[0];
	dstTri[1] = Point2i(point_1234[0].x + l, point_1234[0].y);
	dstTri[2] = Point2i(point_1234[0].x, point_1234[0].y + l);
	dstTri[3] = Point2i(point_1234[0].x + l, point_1234[0].y + l);
	Mat transform = Mat::zeros(3, 3, CV_32FC1); //透视变换矩阵 
	transform = getPerspectiveTransform(point_1234, dstTri);  //获取透视变换矩阵 ，要求为point2i类型，此处和上方的多边形近似输入值类型不同，容易出错
	//透视变换 
	Mat imageWarp;
	warpPerspective(meter_Image, imageWarp, transform, imageWarp.size());
	imshow("After WarpPerspecttive", imageWarp);
	//waitKey();

	Mat imageWarp_gray;
	cvtColor(imageWarp, imageWarp_gray, COLOR_BGR2GRAY);
	//blur(imageWarp_gray, imageWarp_gray, Size(3, 3));
	threshold(imageWarp_gray, imageWarp_gray, 100, 255, THRESH_BINARY);
	imshow("二值图像2", imageWarp_gray);
	//waitKey();

	//截取更小部分
	Rect Rect2;
	Rect2.x = imageWarp_gray.cols /7;
	Rect2.width = imageWarp_gray.cols*0.65;
	Rect2.y = imageWarp_gray.rows /7;
	Rect2.height = imageWarp_gray.rows*0.55;
	Mat ROIimage = Mat(imageWarp_gray, Rect2);//根据这个区域，保留原图像
	imshow("ROI结果", ROIimage);
	//waitKey();


	cv::Mat InROIimage = 255 - ROIimage;
	imshow("ROI反色", InROIimage);
	//waitKey();

	erode(InROIimage, InROIimage, getStructuringElement(MORPH_RECT, Size(3, 3)), Point(-1, -1), 2);
	dilate(InROIimage, InROIimage, getStructuringElement(MORPH_RECT, Size(3, 3)), Point(-1, -1), 2);
	imshow("P_腐蚀膨胀后ROI区域", InROIimage);
	//waitKey();

	vector< vector<Point> > vContours2;
	//vector<Vec4i> vHierarchy2;
	cv::findContours(InROIimage, vContours2, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	// 寻找最大连通域  
	double maxArea = 0;
	vector<cv::Point> maxContour;
	for (size_t i = 0; i < vContours2.size(); i++)
	{
		double area = cv::contourArea(vContours2[i]);
		if (area > maxArea)
		{
			maxArea = area;
			maxContour = vContours2[i];
		}
	}

	// 将轮廓转为矩形框  
	cv::Rect RectPointer = cv::boundingRect(maxContour);

	// 显示连通域  
	cv::Mat result1, result2;

	InROIimage.copyTo(result1);
	InROIimage.copyTo(result2);

	for (size_t i = 0; i < vContours2.size(); i++)
	{
		cv::Rect r = cv::boundingRect(vContours2[i]);
		cv::rectangle(result1, r, cv::Scalar(255));
	}
	cv::imshow("P_all regions", result1);
	//cv::waitKey();

	cv::rectangle(result2, RectPointer, cv::Scalar(255));
	cv::imshow("P_largest region", result2);
	//cv::waitKey();

	

	Point point1;
	Point point2;
	Mat result3 = Mat::zeros(InROIimage.size(), CV_8UC3);
	//InROIimage.copyTo(result3);
	point1.x = RectPointer.x;
	point1.y = RectPointer.y;
	point2.x = RectPointer.x + RectPointer.width;
	point2.y = RectPointer.y + RectPointer.height;
	//printf("point1.x %d", point1.x);
	//printf("point1.y %d", point1.y);
	//printf("point2.x %d", point2.x);
	//printf("point2.y %d", point2.y);
	//画出对角点
	circle(result3, point1, 3, CV_RGB(255, 0, 0), 2);
	circle(result3, point2, 3, CV_RGB(0, 255, 0), 2);

	imshow("最大矩形的对角点", result3);
	//waitKey;


	float gradient;
	double angle;
	double ratio;

	gradient = float(point2.y - point1.y) / float(point2.x - point1.x);
	angle = atan(gradient) * 180 / 3.1415927;
	ratio = angle / 90;
	printf("斜率是%.2lf\n", gradient);   //这个斜率目前没有什么作用，因为是在截取图像中的斜率，无法用这个判断数值
	printf("角度是%.2lf\n", angle);
	printf("比例是%.2lf\n", ratio);



	//waitKey();
	//return ratio;
	return angle;
}
