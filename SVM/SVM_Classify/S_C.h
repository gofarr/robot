#ifndef _S_C_H_
#define _S_C_H_

#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "core/core.hpp"
#include <iostream>
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <time.h>

using namespace std;
using namespace cv;

#define PSAM_B	0				//the number of the obstacles
#define PSAM_G	1000			//the number of the green sample
#define PSAM_R	1000			//the number of the red sample
#define NSAM	3000			//the number of the negative sample

#define SAM_W	64				//width of sample
#define SAM_H	64				//height of sample

#define I_W	640					//width of image
#define I_H	480					//height of image

#define WIN		Size(32,32)		//���ڴ�С		ԭ��(32,32)
#define BLOCK	Size(16,16)		//���С		ԭ��(16,16)
#define STEP	Size(8,8)		//����			ԭ��(8,8)
#define CELL	Size(8,8)		//ϸ����С		ԭ��(8,8)
#define HBIN	9				//ֱ��ͼ������

extern Mat HogMat;					//��ȡ�����ľ���
extern Mat sampleFeatureMat;		//����ѵ������������������ɵľ��������������������ĸ�������������HOG������ά��
extern Mat sampleLabelMat;			//ѵ����������������������������������ĸ�������������1��0-r  1-g  2-b  3-neg

extern HOGDescriptor hog;			//hog������
extern vector<float> descriptors;	//HOG����������
extern int DescriptorDim;			//������ά��
extern int extradim;				//����ά��
extern int supportvector;			//֧����������

extern unsigned long dwLast;
extern unsigned long dwCurrent;

extern int Hog_Dim_Cal(Size win, Size block, Size step, Size cell, int hbin, int offset);
//ʱ�䣺2016.8.1
//���ܣ�����HOG����ά��
//���������ڴ�С�����С��ѭ��������Ԫ�ش�С��ֱ��ͼ������������ά��
//˵��������������
extern void Data_Mat_Init(int dim, int samn);
//ʱ�䣺2016.8.1
//���ܣ���ʼ�����ݺͱ�ǩ����
//������������ά���������������������ࣩ
//˵��������
extern void Hog_Init(void);
//ʱ�䣺2016.8.2
//���ܣ���ʼHOG��ģ��
//���������ޣ����ɺ����ڲ��趨
//˵��������
extern void Hog_Get(int flag, int Hog_N, int offset, Size win, Size block, Size step, Size cell);
//ʱ�䣺2016.8.1
//���ܣ���ȡHOG����
//��������ǩ����ȡͼƬ�������Ѿ���ɵ������������ڴ�С�����С��ѭ��������Ԫ�ش�С
//˵����0-R  1-G  2-B  3-NEG��ÿһ��ִ�иú���ֻ��ѵ��ͬһ������������ѵ����������
extern int SVM_Train(char *filename, Mat sampleFeatureMat, Mat sampleLabelMat);
//ʱ�䣺2016.8.2
//���ܣ�ѵ��SVM������
//�����������������Ŀ¼�����ݾ��󣬱�ǩ����
//˵��������
extern void HogReadtxt(char *filename);
//ʱ�䣺2016.8.2
//���ܣ�д��˵���ĵ�
//�������ļ�Ŀ¼
//˵��������
extern void RGB_AverCal(Mat mat, vector<float>&v);
//ʱ�䣺2016.8.18
//���ܣ�����ͼ���RGBͨ����ֵ
//������ͼ�񣬷�������
//˵��������

#endif