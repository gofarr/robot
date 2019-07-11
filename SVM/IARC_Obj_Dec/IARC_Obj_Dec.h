#ifndef _IARC_OBJ_DEC_H_
#define _IARC_OBJ_DEC_H_

#include "Para.h"

extern void SVM_Load(CvSVM *svm, char *filename);
//ʱ�䣺2016.8.2
//���ܣ�����SVM
//�������ļ�Ŀ¼
//˵��������
extern void Hog_Init(void);
//ʱ�䣺2016.8.2
//���ܣ���ʼHOG��ģ��
//���������ޣ����ɺ����ڲ��趨
//˵��������
extern void Multi_Scale_Obj_Dec(int height, Mat src, HOGDescriptor myHOG, Point start, Size winsize, Size step, float scale, int S, int hflag, int fflag);
//ʱ�䣺2016.8.2
//���ܣ���߶�Ŀ����
//������ԭͼ��HOG�࣬SVM�࣬��⿪ʼ�㣬��ⴰ�ڵĴ�С��ÿһ���ƶ��Ĵ�С��ͼƬ��ѹ���߶ȣ�ͼƬ���ų߶ȣ��߶����Ʊ�־λ������˲���־λ
//˵��������
extern void Multi_Scale_Obj_Dec2(int height, Mat src, HOGDescriptor myHOG, Point start, Size winsize, Size step, float scale, int S, int hflag, int fflag);
//ʱ�䣺2016.8.3
//���ܣ���߶�Ŀ����
//������ԭͼ��HOG�࣬SVM�࣬��⿪ʼ�㣬��ⴰ�ڵĴ�С��ÿһ���ƶ��Ĵ�С�����ڵ���չ�߶ȣ�ͼƬ���ų߶ȣ��߶����Ʊ�־λ������˲���־λ
//˵��������
extern void Obj_MaxMinSize_Cal(float height, int Min_Fit_Met, int Max_Fit_Met, int minoff, int maxoff, int *sizemin, int *sizemax);
//ʱ�䣺2016.8.6
//���ܣ���ĳһ�߶������Ŀ���С������
//�������������߶ȣ���Сֵ��Ϸ�ʽ�����ֵ��Ϸ�ʽ����Сֵԣ�������ֵԣ���������Сֵ��������ֵ
//˵��������
extern int Obj_MinSize_Cal(float height, int Min_Fit_Met, int minoff);
//ʱ�䣺2016.8.6
//���ܣ���ĳһ�߶������Ŀ���С����
//�������������߶ȣ���Сֵ��Ϸ�ʽ����Сֵԣ���������Сֵ
//˵��������
extern int Obj_MaxSize_Cal(float height, int Max_Fit_Met, int maxoff);
//ʱ�䣺2016.8.6
//���ܣ���ĳһ�߶������Ŀ���С����
//�������������߶ȣ����ֵ��Ϸ�ʽ�����ֵԣ����
//˵��������
extern void Full_Search_Img(Mat src, Point start, Size winsize, Size stepsize, HOGDescriptor myHOG, vector<Rect>&found, int time, int S, float scale);
//ʱ�䣺2016.8.8
//���ܣ���һ��ͼ����һ��ȫ����
//������ͼ����ʼ�㣬���ڴ�С��������HOG������������Ѿ�ѭ���Ĵ������߶����ӣ����ų߶�
//˵��������
extern void Full_Search_Img2(Mat src, Point start, Size winsize, Size stepsize, HOGDescriptor myHOG, vector<Rect>&found, int time, int S, float scale);
//ʱ�䣺2016.8.8
//���ܣ���һ��ͼ����һ��ȫ����
//������ͼ����ʼ�㣬���ڴ�С��������HOG������������Ѿ�ѭ���Ĵ������߶����ӣ����ų߶�
//˵��������
extern void Vec_Init(void);
//ʱ�䣺2016.8.12
//���ܣ���ʼ������
//����������
//˵��������
extern void Obj_Filter(Mat src, int lmax, int lmin, vector<Rect>found, vector<Rect>&Obj_Out);
//ʱ�䣺2016.8.12
//���ܣ��Ի���ѧϰ�õ���Ŀ�����ѡ��
//������ͼ��Ŀ��߶����ֵ��Ŀ��߶���Сֵ��ԭ����������������
//˵��������
extern void Obj_H_Filter(int lmax, int lmin, vector<Rect>&found);
//ʱ�䣺2016.8.12
//���ܣ��ø߶����ƶ�Ŀ�����ѡ��
//������Ŀ��߶����ֵ��Ŀ��߶���Сֵ���������
//˵��������
extern void Obj_Classify_Filter(vector<Rect>&found, float threshold);
//ʱ�䣺2016.8.12
//���ܣ��÷���Ŀ�����ѡ��
//���������������������ֵ
//˵��������
extern float Re_Coin_Degree(Rect r1, Rect r2, int *num);
//ʱ�䣺2016.8.12
//���ܣ������������ε��غϲ���ռ���С�ߵı���
//����������1������2��С�ı��
//tips: ����ֵ���غ϶Ⱥ����С�ľ��α�ţ�ǰ�߷���1�����߷���2��
extern void Obj_Filter2(Mat src, int lmax, int lmin, vector<Rect>found, vector<Rect>&Obj_Out);
//ʱ�䣺2016.8.13
//���ܣ��Ի���ѧϰ�õ���Ŀ�����ѡ��
//������ͼ��Ŀ��߶����ֵ��Ŀ��߶���Сֵ��ԭ����������������
//˵����ר����ԵͿյĴ���
extern void Obj_Classify_Filter2(vector<Rect>&found, float threshold);
//ʱ�䣺2016.8.13
//���ܣ��÷���Ŀ�����ѡ��
//���������������������ֵ
//˵����ר����ԵڵͿյĴ���
extern Rect Rect_Merge(Rect r1, Rect r2);
//ʱ�䣺2016.8.13
//���ܣ����������κϲ�
//��������������
//˵���������µľ���
extern void RGB_AverCal(Mat mat, vector<float>&v);
//ʱ�䣺2016.8.18
//���ܣ�����ͼ���RGBͨ����ֵ
//������ͼ�񣬷�������
//˵��������

#endif