#ifndef _IARC_OBJ_DEC_H_
#define _IARC_OBJ_DEC_H_

#include "Para.h"

extern void SVM_Load(CvSVM *svm, char *filename);
//时间：2016.8.2
//功能：加载SVM
//参数：文件目录
//说明：暂无
extern void Hog_Init(void);
//时间：2016.8.2
//功能：初始HOG类模板
//参数：暂无，均由函数内部设定
//说明：暂无
extern void Multi_Scale_Obj_Dec(int height, Mat src, HOGDescriptor myHOG, Point start, Size winsize, Size step, float scale, int S, int hflag, int fflag);
//时间：2016.8.2
//功能：多尺度目标检测
//参数：原图，HOG类，SVM类，检测开始点，检测窗口的大小，每一次移动的大小，图片的压缩尺度，图片缩放尺度，高度限制标志位，结果滤波标志位
//说明：暂无
extern void Multi_Scale_Obj_Dec2(int height, Mat src, HOGDescriptor myHOG, Point start, Size winsize, Size step, float scale, int S, int hflag, int fflag);
//时间：2016.8.3
//功能：多尺度目标检测
//参数：原图，HOG类，SVM类，检测开始点，检测窗口的大小，每一次移动的大小，窗口的扩展尺度，图片缩放尺度，高度限制标志位，结果滤波标志位
//说明：暂无
extern void Obj_MaxMinSize_Cal(float height, int Min_Fit_Met, int Max_Fit_Met, int minoff, int maxoff, int *sizemin, int *sizemax);
//时间：2016.8.6
//功能：在某一高度下拟合目标大小上下限
//参数：飞行器高度，最小值拟合方式，最大值拟合方式，最小值裕量，最大值裕量，输出最小值，输出最大值
//说明：暂无
extern int Obj_MinSize_Cal(float height, int Min_Fit_Met, int minoff);
//时间：2016.8.6
//功能：在某一高度下拟合目标大小下限
//参数：飞行器高度，最小值拟合方式，最小值裕量，输出最小值
//说明：暂无
extern int Obj_MaxSize_Cal(float height, int Max_Fit_Met, int maxoff);
//时间：2016.8.6
//功能：在某一高度下拟合目标大小上限
//参数：飞行器高度，最大值拟合方式，最大值裕量，
//说明：暂无
extern void Full_Search_Img(Mat src, Point start, Size winsize, Size stepsize, HOGDescriptor myHOG, vector<Rect>&found, int time, int S, float scale);
//时间：2016.8.8
//功能：对一幅图进行一次全搜索
//参数：图像，起始点，窗口大小，步长，HOG，结果向量，已经循环的次数，尺度因子，缩放尺度
//说明：暂无
extern void Full_Search_Img2(Mat src, Point start, Size winsize, Size stepsize, HOGDescriptor myHOG, vector<Rect>&found, int time, int S, float scale);
//时间：2016.8.8
//功能：对一幅图进行一次全搜索
//参数：图像，起始点，窗口大小，步长，HOG，结果向量，已经循环的次数，尺度因子，缩放尺度
//说明：暂无
extern void Vec_Init(void);
//时间：2016.8.12
//功能：初始化向量
//参数：暂无
//说明：暂无
extern void Obj_Filter(Mat src, int lmax, int lmin, vector<Rect>found, vector<Rect>&Obj_Out);
//时间：2016.8.12
//功能：对机器学习得到的目标进行选择
//参数：图像，目标尺度最大值，目标尺度最小值，原结果向量，输出向量
//说明：暂无
extern void Obj_H_Filter(int lmax, int lmin, vector<Rect>&found);
//时间：2016.8.12
//功能：用高度限制对目标进行选择
//参数：目标尺度最大值，目标尺度最小值，结果向量
//说明：暂无
extern void Obj_Classify_Filter(vector<Rect>&found, float threshold);
//时间：2016.8.12
//功能：用分类目标进行选择
//参数：结果向量，分类阈值
//说明：暂无
extern float Re_Coin_Degree(Rect r1, Rect r2, int *num);
//时间：2016.8.12
//功能：计算两个矩形的重合部分占面积小者的比例
//参数：矩形1，矩形2，小的标号
//tips: 返回值是重合度和面积小的矩形标号（前者返回1，后者返回2）
extern void Obj_Filter2(Mat src, int lmax, int lmin, vector<Rect>found, vector<Rect>&Obj_Out);
//时间：2016.8.13
//功能：对机器学习得到的目标进行选择
//参数：图像，目标尺度最大值，目标尺度最小值，原结果向量，输出向量
//说明：专门针对低空的处理
extern void Obj_Classify_Filter2(vector<Rect>&found, float threshold);
//时间：2016.8.13
//功能：用分类目标进行选择
//参数：结果向量，分类阈值
//说明：专门针对第低空的处理
extern Rect Rect_Merge(Rect r1, Rect r2);
//时间：2016.8.13
//功能：将两个矩形合并
//参数：两个矩形
//说明：返回新的矩形
extern void RGB_AverCal(Mat mat, vector<float>&v);
//时间：2016.8.18
//功能：计算图像的RGB通道均值
//参数：图像，返回向量
//说明：暂无

#endif