#include "IARC_Obj_Dec.h"

static int lalala = 0;

CvSVM mySVM;

Mat Text_Win;

HOGDescriptor myHOG;

vector<Rect>foundr;
vector<Rect>foundg;
vector<Rect>Obj_Out;

int extradim = 0;

void SVM_Load(CvSVM *svm, char *filename)
{
	printf("SVM加载开始！\n");
	(*svm).load(filename);
	printf("SVM加载结束！\n");
	return;
}
void Hog_Init(void)
{
	myHOG.winSize = WIN;							//检测窗口大小
	myHOG.blockSize = BLOCK;						//块大小，目前只支持Size(16, 16)
	myHOG.blockStride = STEP;						//块的滑动步长，大小只支持是单元格cell_size大小的倍数
	myHOG.cellSize = CELL;							//单元格的大小，目前只支持Size(8, 8)
	myHOG.nbins = 9;								//单元格的大小，目前只支持Size(8, 8)
	myHOG.derivAperture = 1;						//
	myHOG.winSigma = -1.0;							//高斯滤波窗口的参数
	myHOG.histogramNormType = 0;
	myHOG.L2HysThreshold = 0.20000000000000001;		//块内直方图归一化类型L2-Hys的归一化收缩率
	myHOG.gammaCorrection = false;					//是否gamma校正
	myHOG.nlevels = 64;								//检测窗口的最大数量
}
void Multi_Scale_Obj_Dec(int height, Mat src, HOGDescriptor myHOG, Point start, Size winsize, Size step, float scale, int S, int hflag, int fflag)
{
	Mat srctemp = src;

	int count = 0, time = 1, minpredict = 0, maxpredict = 0;

	Obj_MaxMinSize_Cal(height, MIN_POL_3_CSOFF_ROBUSTOFF, MAX_EXP_4_CSOFF, 5, 15, &minpredict, &maxpredict);	//高度限制
	minpredict = (int)(minpredict / S);
	maxpredict = (int)(maxpredict / S);
	//cout << "minpredict = " << minpredict << ",  " << "maxpredict = " << maxpredict << "\n";

	int minpredictfordec = minpredict;
	int maxpredictfordec = maxpredict;		//检测

	int minpredictforfilter = minpredict;
	int maxpredictforfilter = maxpredict;	//滤波
		
	if (!hflag)
	{
		minpredictfordec = 0;
		maxpredictfordec = MIN(I_H, I_W);
	}

	int widthref = (int)(winsize.width);
	int heightref = (int)(winsize.height);	//搜索框参考值

	int widthnew = (int)(srctemp.cols / S);
	int heightnew = (int)(srctemp.rows / S);	//图片大小

	resize(srctemp, srctemp, Size((int)(widthnew), (int)(heightnew)), 0, 0, INTER_CUBIC);

	if (maxpredictfordec <= widthref)			//如果拟合目标尺度最大值都要比初始搜索框小，那么就只进行一次全搜索
	{
		printf("Once enough!\n");
		Full_Search_Img(srctemp, start, winsize, step, myHOG, foundr, time, S, scale);
		time++;
	}
	else
	{
		printf("Need to carry on many times!\n");
		while (widthref < minpredictfordec || heightref < minpredictfordec)
		{			
			widthref = (int)(widthref*scale);		//搜索框扩大
			heightref = (int)(heightref*scale);

			widthnew = (int)(widthnew / scale);		//图幅缩小
			heightnew = (int)(heightnew / scale);
			time++;
		}
		
		while (srctemp.cols >= winsize.width && srctemp.rows >= winsize.height)		//如果图像尺度大于窗口，越变目标越小
		{
			resize(srctemp, srctemp, Size(widthnew, heightnew), 0, 0, INTER_CUBIC);
			if (srctemp.cols < winsize.width || srctemp.rows < winsize.height)
				break;			

			Full_Search_Img(srctemp, start, winsize, step, myHOG, foundr, time, S, scale);	//一次全搜索
			time++;

			widthnew = (int)(widthnew / scale);
			heightnew = (int)(heightnew / scale);		//图片大小

			widthref = (int)(widthref*scale);
			heightref = (int)(heightref*scale);
			if (widthref > maxpredictfordec || heightref > maxpredictfordec)
				break;
		}
	}

	Mat srcfilter = Mat::zeros(Size(src.cols, src.rows), CV_8UC3);
	src.copyTo(srcfilter);

	if (!fflag)
	{
		minpredictforfilter = 0;
		maxpredictforfilter = MIN(I_H, I_W);
	}
	cout << "min = " << minpredictforfilter*S << ",  " << "max = " << maxpredictforfilter*S << "\n";

	cout << "Target number: " << foundr.size() << "   not filted\n";
	for (int i = 0; i < foundr.size(); i++)
	{
		rectangle(src, foundr[i].tl(), foundr[i].br(), Scalar(0, 0, 255), 3);
		cout << "Size( " << foundr[i].width << " ," << foundr[i].height << " )\n";
	}
	cout << "Target number: " << foundg.size() << "   not filted\n";
	for (int i = 0; i < foundg.size(); i++)
	{
		rectangle(src, foundg[i].tl(), foundg[i].br(), Scalar(0, 255, 0), 3);
		cout << "Size( " << foundg[i].width << " ," << foundg[i].height << " )\n";
	}
	//if (height >= 100)
	//	Obj_Filter(src, maxpredictforfilter*S, minpredictforfilter*S, found, Obj_Out);
	//else if (height < 100)
	//	Obj_Filter2(src, maxpredictforfilter*S, minpredictforfilter*S, found, Obj_Out);

	/*cout << "Target number: " << Obj_Out.size() << "   filted\n";
	for (int i = 0; i < Obj_Out.size(); i++)
	{
		rectangle(srcfilter, Obj_Out[i].tl(), Obj_Out[i].br(), Scalar(0, 255, 0), 3);
		cout << "Size( " << Obj_Out[i].width << " ," << Obj_Out[i].height << " )\n";
	}*/

	//imshow("srcfilter", srcfilter);
	imshow("src", src);
	Vec_Init();
	
	//char name[200] = "";
	//sprintf(name, "../../../temp/src/%d.jpg", lalala);
	//imwrite(name, src);
	//sprintf(name, "../../../temp/srcfilted/%d.jpg", lalala);
	//imwrite(name, srcfilter);
	//lalala++;
	return;
}
void Multi_Scale_Obj_Dec2(int height, Mat src, HOGDescriptor myHOG, Point start, Size winsize, Size step, float scale, int S, int hflag, int fflag)
{
	Mat srctemp = src;
	int count = 0, time = 1, minpredict = 0, maxpredict = 0;

	Obj_MaxMinSize_Cal(height, MIN_POL_3_CSOFF_ROBUSTOFF, MAX_EXP_4_CSOFF, 5, 0, &minpredict, &maxpredict);

	minpredict = (int)(minpredict / S);
	maxpredict = (int)(maxpredict / S);

	cout << "minpredict = " << minpredict << ",  " << "maxpredict = " << maxpredict << "\n";

	int widthref = winsize.width;	//32
	int heightref = winsize.height;	//32

	resize(srctemp, srctemp, Size((int)(srctemp.cols / S), (int)(srctemp.rows / S)), 0, 0, INTER_CUBIC);

	if (maxpredict <= MIN(widthref, heightref))		//只搜索一次就好
	{
		cout << "Once enough!\n";
		step.width = (int)(widthref / 2);		//窗口移动步长
		step.height = (int)(heightref / 2);

		Full_Search_Img2(srctemp, start, Size(widthref, heightref), step, myHOG, foundr, time, S, scale);

		time++;
	}
	else
	{
		printf("Need to carry on many times!\n");
		while (widthref <minpredict || heightref <minpredict)
		{
			widthref = (int)(widthref*scale);
			heightref = (int)(heightref*scale);
			time++;
		}

		while (1)
		{
			if (widthref > srctemp.cols || heightref > srctemp.rows)break;

			step.width = (int)(widthref / 2);		//窗口移动步长
			step.height = (int)(heightref / 2);

			Full_Search_Img2(srctemp, start, Size(widthref, heightref), step, myHOG, foundr, time, S, scale);
			
			time++;
			//一次全搜索

			widthref = (int)(widthref*scale);
			heightref = (int)(heightref*scale);
			if (widthref > maxpredict || heightref > maxpredict)break;
		}
	}

	cout << "Target number: " << foundr.size() << "\n";
	for (int i = 0; i < foundr.size(); i++)
		rectangle(src, foundr[i].tl(), foundr[i].br(), Scalar(0, 0, 255), 3);
	foundr.clear();
	foundg.clear();
	imshow("src", src);
}
void Obj_MaxMinSize_Cal(float height, int Min_Fit_Met, int Max_Fit_Met, int minoff, int maxoff, int *sizemin, int *sizemax)
{
	*sizemin = Obj_MinSize_Cal(height, Min_Fit_Met, minoff);
	*sizemax = Obj_MaxSize_Cal(height, Max_Fit_Met, maxoff);
	//printf("min = %d\n", *sizemin);
	//printf("max = %d\n", *sizemax);
	if (*sizemin >= *sizemax)
	{
		*sizemin = 0;
		*sizemax = MIN(I_W, I_H);
	}
}
int Obj_MinSize_Cal(float height, int Min_Fit_Met, int minoff)
{
	float molecule = 0.;
	float denominator = 0.;
	float fitsize = 0.;
	if (height > 5.0)
	{
		switch (Min_Fit_Met)
		{
		case MIN_EXP_2_CSOFF:	//0 a*exp(b*h)
			fitsize = DATA_FIT[Min_Fit_Met].coefficient[0] * exp(DATA_FIT[Min_Fit_Met].coefficient[1] * height);
			break;
		case MIN_EXP_2_CSON:	//1
			fitsize = DATA_FIT[Min_Fit_Met].coefficient[0] * exp(DATA_FIT[Min_Fit_Met].coefficient[1] * height);
			break;
		case MIN_EXP_4_CSOFF:	//2
			fitsize = DATA_FIT[Min_Fit_Met].coefficient[0] * exp(DATA_FIT[Min_Fit_Met].coefficient[1] * height) +
				DATA_FIT[Min_Fit_Met].coefficient[2] * exp(DATA_FIT[Min_Fit_Met].coefficient[3] * height);
			break;
		case MIN_EXP_4_CSON:	//3
			fitsize = DATA_FIT[Min_Fit_Met].coefficient[0] * exp(DATA_FIT[Min_Fit_Met].coefficient[1] * height) +
				DATA_FIT[Min_Fit_Met].coefficient[2] * exp(DATA_FIT[Min_Fit_Met].coefficient[3] * height);
			break;
		case MIN_FOU_4_CSOFF:	//4
			fitsize = DATA_FIT[Min_Fit_Met].coefficient[0] + DATA_FIT[Min_Fit_Met].coefficient[1] * cos(DATA_FIT[Min_Fit_Met].coefficient[3] * height) +
				DATA_FIT[Min_Fit_Met].coefficient[2] * sin(DATA_FIT[Min_Fit_Met].coefficient[3] * height);
			break;
		case MIN_FOU_4_CSON:	//5
			fitsize = DATA_FIT[Min_Fit_Met].coefficient[0] + DATA_FIT[Min_Fit_Met].coefficient[1] * cos(DATA_FIT[Min_Fit_Met].coefficient[3] * height) +
				DATA_FIT[Min_Fit_Met].coefficient[2] * sin(DATA_FIT[Min_Fit_Met].coefficient[3] * height);
			break;
		case MIN_FOU_6_CSOFF:	//6
			fitsize = DATA_FIT[Min_Fit_Met].coefficient[0] + DATA_FIT[Min_Fit_Met].coefficient[1] * cos(DATA_FIT[Min_Fit_Met].coefficient[5] * height) +
				DATA_FIT[Min_Fit_Met].coefficient[2] * sin(DATA_FIT[Min_Fit_Met].coefficient[5] * height) +
				DATA_FIT[Min_Fit_Met].coefficient[3] * cos(DATA_FIT[Min_Fit_Met].coefficient[5] * height * 2) +
				DATA_FIT[Min_Fit_Met].coefficient[4] * sin(DATA_FIT[Min_Fit_Met].coefficient[5] * height * 2);
			break;
		case MIN_FOU_6_CSON:	//7
			fitsize = DATA_FIT[Min_Fit_Met].coefficient[0] + DATA_FIT[Min_Fit_Met].coefficient[1] * cos(DATA_FIT[Min_Fit_Met].coefficient[5] * height) +
				DATA_FIT[Min_Fit_Met].coefficient[2] * sin(DATA_FIT[Min_Fit_Met].coefficient[5] * height) +
				DATA_FIT[Min_Fit_Met].coefficient[3] * cos(DATA_FIT[Min_Fit_Met].coefficient[5] * height * 2) +
				DATA_FIT[Min_Fit_Met].coefficient[4] * sin(DATA_FIT[Min_Fit_Met].coefficient[5] * height * 2);
			break;
		case MIN_POL_3_CSOFF_ROBUSTOFF:	//8
			fitsize = DATA_FIT[Min_Fit_Met].coefficient[0] * height * height + DATA_FIT[Min_Fit_Met].coefficient[1] * height + DATA_FIT[Min_Fit_Met].coefficient[2];
			break;
		case MIN_POL_3_CSOFF_ROBUSTLAR:	//9
			fitsize = DATA_FIT[Min_Fit_Met].coefficient[0] * height * height + DATA_FIT[Min_Fit_Met].coefficient[1] * height + DATA_FIT[Min_Fit_Met].coefficient[2];
			break;
		case MIN_POL_3_CSOFF_ROBUSTBIS:	//10
			fitsize = DATA_FIT[Min_Fit_Met].coefficient[0] * height * height + DATA_FIT[Min_Fit_Met].coefficient[1] * height + DATA_FIT[Min_Fit_Met].coefficient[2];
			break;
		case MIN_POL_3_CSON_ROBUSTOFF:	//11
			fitsize = DATA_FIT[Min_Fit_Met].coefficient[0] * height * height + DATA_FIT[Min_Fit_Met].coefficient[1] * height + DATA_FIT[Min_Fit_Met].coefficient[2];
			break;
		case MIN_POL_3_CSON_ROBUSTLAR:	//12
			fitsize = DATA_FIT[Min_Fit_Met].coefficient[0] * height * height + DATA_FIT[Min_Fit_Met].coefficient[1] * height + DATA_FIT[Min_Fit_Met].coefficient[2];
			break;
		case MIN_POL_3_CSON_ROBUSTBIS:	//13
			fitsize = DATA_FIT[Min_Fit_Met].coefficient[0] * height * height + DATA_FIT[Min_Fit_Met].coefficient[1] * height + DATA_FIT[Min_Fit_Met].coefficient[2];
			break;
		case MIN_POL_4_CSOFF_ROBUSTOFF:	//14
			fitsize = DATA_FIT[Min_Fit_Met].coefficient[0] * pow(height, 3) + DATA_FIT[Min_Fit_Met].coefficient[1] * pow(height, 2) +
				DATA_FIT[Min_Fit_Met].coefficient[2] * height + DATA_FIT[Min_Fit_Met].coefficient[3];
			break;
		case MIN_POL_4_CSON_ROBUSTOFF:	//15
			fitsize = DATA_FIT[Min_Fit_Met].coefficient[0] * pow(height, 3) + DATA_FIT[Min_Fit_Met].coefficient[1] * pow(height, 2) +
				DATA_FIT[Min_Fit_Met].coefficient[2] * height + DATA_FIT[Min_Fit_Met].coefficient[3];
			break;
		case MIN_POL_4_CSON_ROBUSTLAR:	//16
			fitsize = DATA_FIT[Min_Fit_Met].coefficient[0] * pow(height, 3) + DATA_FIT[Min_Fit_Met].coefficient[1] * pow(height, 2) +
				DATA_FIT[Min_Fit_Met].coefficient[2] * height + DATA_FIT[Min_Fit_Met].coefficient[3];
			break;
		case MIN_POL_4_CSOFF_ROBUSTBIS:	//17
			fitsize = DATA_FIT[Min_Fit_Met].coefficient[0] * pow(height, 3) + DATA_FIT[Min_Fit_Met].coefficient[1] * pow(height, 2) +
				DATA_FIT[Min_Fit_Met].coefficient[2] * height + DATA_FIT[Min_Fit_Met].coefficient[3];
			break;
		case MIN_POL_4_CSON_ROBUSTBIS:	//18
			fitsize = DATA_FIT[Min_Fit_Met].coefficient[0] * pow(height, 3) + DATA_FIT[Min_Fit_Met].coefficient[1] * pow(height, 2) +
				DATA_FIT[Min_Fit_Met].coefficient[2] * height + DATA_FIT[Min_Fit_Met].coefficient[3];
			break;
		case MIN_POW_2_CSOFF:	//19
			fitsize = DATA_FIT[Min_Fit_Met].coefficient[0] * pow(height, DATA_FIT[Min_Fit_Met].coefficient[1]);
			break;
		case MIN_POW_3_CSOFF:	//20
			fitsize = DATA_FIT[Min_Fit_Met].coefficient[0] * pow(height, DATA_FIT[Min_Fit_Met].coefficient[1]) + DATA_FIT[Min_Fit_Met].coefficient[2];
			break;
		case MIN_RAT_7_CSOFF:	//21
			molecule = DATA_FIT[Min_Fit_Met].coefficient[0] * pow(height, 4) + DATA_FIT[Min_Fit_Met].coefficient[1] * pow(height, 3) +
				DATA_FIT[Min_Fit_Met].coefficient[2] * pow(height, 2) + DATA_FIT[Min_Fit_Met].coefficient[3] * height +
				DATA_FIT[Min_Fit_Met].coefficient[4];	//分子
			denominator = pow(height, 2) + DATA_FIT[Min_Fit_Met].coefficient[5] * height + DATA_FIT[Min_Fit_Met].coefficient[6];

			if (denominator < 1e-1)
			{
				printf("分子太小！\n");
				fitsize = (float)(SAM_W*1.0);
			}
			else
				fitsize = molecule / denominator;
			break;
		case MIN_SELF:	//46
			printf("公式不知道！\n");
			fitsize = (float)(SAM_W*1.0);
			break;
		default:
			printf("没有这个参数！\n");
			fitsize = (float)(SAM_W*1.0);
			break;
		}
	}
	else
	{
		fitsize = 10.0;
	}
	//printf("偏移之前mmin = %d\n", (int)fitsize);
	return MAX(0, (int)(fitsize - minoff));	
}
int Obj_MaxSize_Cal(float height, int Max_Fit_Met, int maxoff)
{
	float molecule = 0.;
	float denominator = 0.;
	float fitsize = 0.;
	if (height > 5.0)
	{
		switch (Max_Fit_Met)
		{
		case MAX_EXP_2_CSOFF:	//22
			fitsize = DATA_FIT[Max_Fit_Met].coefficient[0] * exp(DATA_FIT[Max_Fit_Met].coefficient[1] * height);
			break;
		case MAX_EXP_2_CSON:	//23
			fitsize = DATA_FIT[Max_Fit_Met].coefficient[0] * exp(DATA_FIT[Max_Fit_Met].coefficient[1] * height);
			break;
		case MAX_EXP_4_CSOFF:	//24
			fitsize = DATA_FIT[Max_Fit_Met].coefficient[0] * exp(DATA_FIT[Max_Fit_Met].coefficient[1] * height) +
				DATA_FIT[Max_Fit_Met].coefficient[2] * exp(DATA_FIT[Max_Fit_Met].coefficient[3] * height);
			break;
		case MAX_EXP_4_CSON:	//25
			fitsize = DATA_FIT[Max_Fit_Met].coefficient[0] * exp(DATA_FIT[Max_Fit_Met].coefficient[1] * height) +
				DATA_FIT[Max_Fit_Met].coefficient[2] * exp(DATA_FIT[Max_Fit_Met].coefficient[3] * height);
			break;
		case MAX_FOU_4_CSOFF:	//26
			fitsize = DATA_FIT[Max_Fit_Met].coefficient[0] + DATA_FIT[Max_Fit_Met].coefficient[1] * cos(DATA_FIT[Max_Fit_Met].coefficient[3] * height) +
				DATA_FIT[Max_Fit_Met].coefficient[2] * sin(DATA_FIT[Max_Fit_Met].coefficient[3] * height);
			break;
		case MAX_FOU_4_CSON:	//27
			fitsize = DATA_FIT[Max_Fit_Met].coefficient[0] + DATA_FIT[Max_Fit_Met].coefficient[1] * cos(DATA_FIT[Max_Fit_Met].coefficient[3] * height) +
				DATA_FIT[Max_Fit_Met].coefficient[2] * sin(DATA_FIT[Max_Fit_Met].coefficient[3] * height);
			break;
		case MAX_FOU_6_CSOFF:	//28
			fitsize = DATA_FIT[Max_Fit_Met].coefficient[0] + DATA_FIT[Max_Fit_Met].coefficient[1] * cos(DATA_FIT[Max_Fit_Met].coefficient[5] * height) +
				DATA_FIT[Max_Fit_Met].coefficient[2] * sin(DATA_FIT[Max_Fit_Met].coefficient[5] * height) +
				DATA_FIT[Max_Fit_Met].coefficient[3] * cos(DATA_FIT[Max_Fit_Met].coefficient[5] * height * 2) +
				DATA_FIT[Max_Fit_Met].coefficient[4] * sin(DATA_FIT[Max_Fit_Met].coefficient[5] * height * 2);
			break;
		case MAX_FOU_6_CSON:	//29
			fitsize = DATA_FIT[Max_Fit_Met].coefficient[0] + DATA_FIT[Max_Fit_Met].coefficient[1] * cos(DATA_FIT[Max_Fit_Met].coefficient[5] * height) +
				DATA_FIT[Max_Fit_Met].coefficient[2] * sin(DATA_FIT[Max_Fit_Met].coefficient[5] * height) +
				DATA_FIT[Max_Fit_Met].coefficient[3] * cos(DATA_FIT[Max_Fit_Met].coefficient[5] * height * 2) +
				DATA_FIT[Max_Fit_Met].coefficient[4] * sin(DATA_FIT[Max_Fit_Met].coefficient[5] * height * 2);
			break;
		case MAX_POL_3_CSOFF_ROBUSTOFF:	//30
			fitsize = DATA_FIT[Max_Fit_Met].coefficient[0] * height * height + DATA_FIT[Max_Fit_Met].coefficient[1] * height + DATA_FIT[Max_Fit_Met].coefficient[2];
			break;
		case MAX_POL_3_CSON_ROBUSTOFF:	//31
			fitsize = DATA_FIT[Max_Fit_Met].coefficient[0] * height * height + DATA_FIT[Max_Fit_Met].coefficient[1] * height + DATA_FIT[Max_Fit_Met].coefficient[2];
			break;
		case MAX_POL_3_CSOFF_ROBUSTLAR:	//32
			fitsize = DATA_FIT[Max_Fit_Met].coefficient[0] * height * height + DATA_FIT[Max_Fit_Met].coefficient[1] * height + DATA_FIT[Max_Fit_Met].coefficient[2];
			break;
		case MAX_POL_3_CSON_ROBUSTLAR:	//33
			fitsize = DATA_FIT[Max_Fit_Met].coefficient[0] * height * height + DATA_FIT[Max_Fit_Met].coefficient[1] * height + DATA_FIT[Max_Fit_Met].coefficient[2];
			break;
		case MAX_POL_3_CSOFF_ROBUSTBIS:	//34
			fitsize = DATA_FIT[Max_Fit_Met].coefficient[0] * height * height + DATA_FIT[Max_Fit_Met].coefficient[1] * height + DATA_FIT[Max_Fit_Met].coefficient[2];
			break;
		case MAX_POL_3_CSON_ROBUSTBIS:	//35
			fitsize = DATA_FIT[Max_Fit_Met].coefficient[0] * height * height + DATA_FIT[Max_Fit_Met].coefficient[1] * height + DATA_FIT[Max_Fit_Met].coefficient[2];
			break;
		case MAX_POL_4_CSOFF_ROBUSTOFF:	//36
			fitsize = DATA_FIT[Max_Fit_Met].coefficient[0] * pow(height, 3) + DATA_FIT[Max_Fit_Met].coefficient[1] * pow(height, 2) +
				DATA_FIT[Max_Fit_Met].coefficient[2] * height + DATA_FIT[Max_Fit_Met].coefficient[3];
			break;
		case MAX_POL_4_CSON_ROBUSTOFF:	//37
			fitsize = DATA_FIT[Max_Fit_Met].coefficient[0] * pow(height, 3) + DATA_FIT[Max_Fit_Met].coefficient[1] * pow(height, 2) +
				DATA_FIT[Max_Fit_Met].coefficient[2] * height + DATA_FIT[Max_Fit_Met].coefficient[3];
			break;
		case MAX_POL_4_CSON_ROBUSTLAR:	//38
			fitsize = DATA_FIT[Max_Fit_Met].coefficient[0] * pow(height, 3) + DATA_FIT[Max_Fit_Met].coefficient[1] * pow(height, 2) +
				DATA_FIT[Max_Fit_Met].coefficient[2] * height + DATA_FIT[Max_Fit_Met].coefficient[3];
			break;
		case MAX_POL_4_CSOFF_ROBUSTBIS:	//39
			fitsize = DATA_FIT[Max_Fit_Met].coefficient[0] * pow(height, 3) + DATA_FIT[Max_Fit_Met].coefficient[1] * pow(height, 2) +
				DATA_FIT[Max_Fit_Met].coefficient[2] * height + DATA_FIT[Max_Fit_Met].coefficient[3];
			break;
		case MAX_POL_4_CSON_ROBUSTBIS:	//40
			fitsize = DATA_FIT[Max_Fit_Met].coefficient[0] * pow(height, 3) + DATA_FIT[Max_Fit_Met].coefficient[1] * pow(height, 2) +
				DATA_FIT[Max_Fit_Met].coefficient[2] * height + DATA_FIT[Max_Fit_Met].coefficient[3];
			break;
		case MAX_POW_2_CSOFF:	//41
			fitsize = DATA_FIT[Max_Fit_Met].coefficient[0] * pow(height, DATA_FIT[Max_Fit_Met].coefficient[1]);
			break;
		case MAX_POW_3_CSOFF:	//42
			fitsize = DATA_FIT[Max_Fit_Met].coefficient[0] * pow(height, DATA_FIT[Max_Fit_Met].coefficient[1]) + DATA_FIT[Max_Fit_Met].coefficient[2];
			break;
		case MAX_RAT_3_CSOFF:	//43
			molecule = DATA_FIT[Max_Fit_Met].coefficient[0] * height + DATA_FIT[Max_Fit_Met].coefficient[1];
			denominator = height + DATA_FIT[Max_Fit_Met].coefficient[2];
			if (denominator < 1e-1)
			{
				printf("分子太小！\n");
				fitsize = (float)(I_H*1.0);
			}
			else
			{
				fitsize = molecule / denominator;
			}
			break;
		case MAX_RAT_4_CSON:	//44
			molecule = DATA_FIT[Max_Fit_Met].coefficient[0] * pow(height, 2) + DATA_FIT[Max_Fit_Met].coefficient[1] * height + DATA_FIT[Max_Fit_Met].coefficient[2];
			denominator = height + DATA_FIT[Max_Fit_Met].coefficient[3];
			if (denominator < 1e-1)
			{
				printf("分子太小\n");
				fitsize = (float)(I_H*1.0);
			}
			else
			{
				fitsize = molecule / denominator;
			}
			break;
		case MAX_RAT_5_CSOFF:	//45
			printf("感觉不对，不用了！\n");
			fitsize = (float)(I_H*1.0);
			break;
		case MAX_SELF:	//47
			break;
		default:
			printf("没有这个参数！\n");
			fitsize = (float)(I_H*1.0);
			break;
		}
	}
	else
	{
		fitsize = 10.0;
	}
	//printf("偏移之前mmax = %d\n", (int)fitsize);
	return MIN(I_H, (int)(fitsize + maxoff));
}
void Full_Search_Img(Mat src, Point start, Size winsize, Size stepsize, HOGDescriptor myHOG, vector<Rect>&found, int time, int S, float scale)
{
	//cout << "Image size: " << src.cols << "  " << src.rows << "\n";
	int xbin = (src.cols - winsize.width) / stepsize.width + 1;
	int ybin = (src.rows - winsize.height) / stepsize.height + 1;

	int totalcount = 0;
	for (int i = 0; i < xbin; i++)
	for (int j = 0; j < ybin; j++)
	{
		totalcount++;
		vector<float>descriptors;		//结果数组

		Rect win = Rect(start.x + i*stepsize.width, start.y + j*stepsize.height, winsize.width, winsize.height);
		Mat imghog = src(win);

		myHOG.compute(imghog, descriptors, Size(8, 8), Size(0, 0)); //调用计算函数开始计算

		Mat SVMtrainMat = Mat::zeros(1, descriptors.size() + extradim, CV_32FC1);
		//Mat SVMtrainMat = Mat::zeros(1, descriptors.size(), CV_32FC1);
		int n = 0;
		for (vector<float>::iterator iter = descriptors.begin(); iter != descriptors.end(); iter++)
		{
			SVMtrainMat.at<float>(0, n) = *iter;
			n++;
		}
		/*vector<float>v;
		RGB_AverCal(imghog, v);
		for (int pp = 0; pp < v.size(); pp++)
			SVMtrainMat.at<float>(0, descriptors.size() + pp) = v[pp];*/
		int ret = mySVM.predict(SVMtrainMat);

		if (ret == 0)
		{
			cout << "Target get!" << "ret = " << ret << "\n";
			win.x = (int)((win.x*pow(scale, time - 1))*S);
			win.y = (int)((win.y*pow(scale, time - 1))*S);
			win.width = (int)((win.width*pow(scale, time - 1))*S);
			win.height = (int)((win.height*pow(scale, time - 1))*S);
			foundr.push_back(win);
		}
		else if (ret == 1)
		{
			cout << "Target get!" << "ret = " << ret << "\n";			
			win.x = (int)((win.x*pow(scale, time - 1))*S);
			win.y = (int)((win.y*pow(scale, time - 1))*S);
			win.width = (int)((win.width*pow(scale, time - 1))*S);
			win.height = (int)((win.height*pow(scale, time - 1))*S);
			foundg.push_back(win);
		}
		else if (ret == 2)
		{
			cout << "Target get!" << "ret = " << ret << "\n";
			win.x = (int)((win.x*pow(scale, time - 1))*S);
			win.y = (int)((win.y*pow(scale, time - 1))*S);
			win.width = (int)((win.width*pow(scale, time - 1))*S);
			win.height = (int)((win.height*pow(scale, time - 1))*S);
			found.push_back(win);			
		}
		else;
	}
	//cout << "totalcount = " << totalcount << "\n";
}
void Full_Search_Img2(Mat src, Point start, Size winsize, Size stepsize, HOGDescriptor myHOG, vector<Rect>&found, int time, int S, float scale)
{
	cout << "Image size: " << src.cols << "  " << src.rows << "\n";
	int xbin = (src.cols - winsize.width) / stepsize.width + 1;
	int ybin = (src.rows - winsize.height) / stepsize.height + 1;

	int totalcount = 0;
	for (int i = 0; i < xbin; i++)
	for (int j = 0; j < ybin; j++)
	{
		totalcount++;
		vector<float>descriptors;		//结果数组

		Rect win = Rect(start.x + i*stepsize.width, start.y + j*stepsize.height, winsize.width, winsize.height);
		Mat imghog = src(win);
		resize(imghog, imghog, WIN, 0, 0, INTER_CUBIC);

		myHOG.compute(imghog, descriptors, Size(8, 8), Size(0, 0)); //调用计算函数开始计算

		Mat SVMtrainMat = Mat::zeros(1, descriptors.size(), CV_32FC1);
		int n = 0;
		for (vector<float>::iterator iter = descriptors.begin(); iter != descriptors.end(); iter++)
		{
			SVMtrainMat.at<float>(0, n) = *iter;
			n++;
		}
		int ret = mySVM.predict(SVMtrainMat);

		if (ret == 0)
		{
			cout << "Target get!" << "ret = " << ret << "\n";
			win.x *= 2;
			win.y *= 2;
			win.width *= 2;
			win.height *= 2;
			found.push_back(win);
		}
		else if (ret == 1)
		{
			cout << "Target get!" << "ret = " << ret << "\n";
			win.x *= 2;
			win.y *= 2;
			win.width *= 2;
			win.height *= 2;
			found.push_back(win);
		}
		else if (ret == 2)
		{
			cout << "Target get!" << "ret = " << ret << "\n";
			win.x *= 2;
			win.y *= 2;
			win.width *= 2;
			win.height *= 2;
			found.push_back(win);
		}
		else;
	}
	//cout << "totalcount = " << totalcount << "\n";
}
void Vec_Init(void)
{
	foundr.clear();
	foundg.clear();
	Obj_Out.clear();
}
void Obj_Filter(Mat src, int lmax, int lmin, vector<Rect>found, vector<Rect>&Obj_Out)
{
	Obj_Out.clear();	

	//step1: "too wide or too narrow"
	Obj_H_Filter(lmax, lmin, found);		//利用高度进行筛选

	cout << "Atfer Obj_H_Filter" << "\n";
	cout << "Target number: " << found.size() << "\n";
	for (int i = 0; i < found.size(); i++)
	{
		rectangle(src, found[i].tl(), found[i].br(), Scalar(0, 0, 255), 3);
		cout << "Size( " << found[i].width << " ," << found[i].height << " )\n";
	}
	imshow("src", src);
	//step2:
	Obj_Classify_Filter(found, 0.25);

	Obj_Out.assign(found.begin(), found.end());			//将found赋值给Obj_Out
}
void Obj_H_Filter(int lmax, int lmin, vector<Rect>&found)
{
	vector<Rect>temp;
	temp.assign(found.begin(), found.end());
	found.clear();
	for (int i = 0; i < temp.size(); i++)
	if (!(temp[i].width >lmax || temp[i].width < lmin))
		found.push_back(temp[i]);
}
void Obj_Classify_Filter(vector<Rect>&found, float threshold)
{
	int deflag = 1;
	if (found.size()>1)
	{
		while (deflag)	//如果有一个元素被删除，那么deflag就等于1
		{
			deflag = 0;
			for (int i = 0; i < found.size() - 1; i++)
			{
				for (int j = i + 1; j < found.size(); j++)
				{
					int a = 0;
					if (Re_Coin_Degree(found[i], found[j], &a) > threshold)		//则判断两个矩形属于一个目标
					{
						if (a == 1)			//小的是found[j]
							found.erase(found.begin() + j);
						else if (a == 2)	//小的是found[i]
							found.erase(found.begin() + i);
						else;
						deflag = 1;
					}
				}
			}
		}
	}
}
float Re_Coin_Degree(Rect r1, Rect r2, int *num)
{
	Point pt1 = { r1.x, r1.y };								//1左上
	Point pt2 = { r1.x + r1.width, r1.y + r1.height };		//1右下
	Point pt3 = { r2.x, r2.y };								//2左上
	Point pt4 = { r2.x + r2.width, r2.y + r2.height };		//2右下

	long s1 = r1.height*r1.width;	//1面积
	long s2 = r2.height*r2.width;	//2面积

	*num = s1 > s2 ? 1 : 2;

	int a1 = MIN(pt1.x, pt3.x);	//最左
	int a2 = MAX(pt2.x, pt4.x);	//最右
	int a3 = MIN(pt1.y, pt3.y);	//最上
	int a4 = MAX(pt2.y, pt4.y);	//最下

	int coinw = a2 - a1 - r1.width - r2.width < 0 ? -(a2 - a1 - r1.width - r2.width) : 0;		//横向重合
	int coinh = a4 - a3 - r1.height - r2.height < 0 ? -(a4 - a3 - r1.height - r2.height) : 0;	//纵向重合

	if (*num == 2)			//r1小
		return (((float)(coinw*1.0)) / ((float)(r1.width*1.0)))* (((float)(coinh*1.0)) / ((float)(r1.height*1.0)));
	else if (*num == 1)		//r2小	
		return (((float)(coinw*1.0)) / ((float)(r2.width*1.0)))* (((float)(coinh*1.0)) / ((float)(r2.height*1.0)));
	else
		return 1.0;
}
void Obj_Filter2(Mat src, int lmax, int lmin, vector<Rect>found, vector<Rect>&Obj_Out)
{
	Obj_Out.clear();

	//step1: "too wide or too narrow"
	Obj_H_Filter(lmax, lmin, found);		//利用高度进行筛选

	cout << "Atfer Obj_H_Filter" << "\n";
	cout << "Target number: " << found.size() << "\n";
	for (int i = 0; i < found.size(); i++)
	{
		rectangle(src, found[i].tl(), found[i].br(), Scalar(0, 0, 255), 3);
		cout << "Size( " << found[i].width << " ," << found[i].height << " )\n";
	}
	//imshow("src", src);
	//step2:
	Obj_Classify_Filter2(found, 0.125);

	Obj_Out.assign(found.begin(), found.end());			//将found赋值给Obj_Out

	cout << "Target number: " << Obj_Out.size() << "   filted\n";
	for (int i = 0; i < Obj_Out.size(); i++)
	{
		rectangle(src, Obj_Out[i].tl(), Obj_Out[i].br(), Scalar(0, 255, 0), 3);
		cout << "Size( " << Obj_Out[i].width << " ," << Obj_Out[i].height << " )\n";
	}
	imshow("src", src);
}
void Obj_Classify_Filter2(vector<Rect>&found, float threshold)
{
	int deflag = 1;
	if (found.size() > 1)
	{
		while (deflag)
		{
			deflag = 0;
			for (int i = 0; i < found.size(); i++)
			for (int j = i + 1; j < found.size(); j++)
			{
				int nouse = 0;
				if (Re_Coin_Degree(found[i], found[j], &nouse) > threshold)		//则判断两个矩形属于一个目标
				{
					if (i != j)
					{
						Rect newr = Rect_Merge(found[i], found[j]);
						found.push_back(newr);
						found.erase(found.begin() + j);
						found.erase(found.begin() + i);		//j的标号大，必须先擦除j

						deflag = 1;
					}
					//把两个矩形合并
				}
			}
		}
	}
}
Rect Rect_Merge(Rect r1, Rect r2)
{

	Rect out;
	out.x = MIN(r1.tl().x, r2.tl().x);
	out.y = MIN(r1.tl().y, r2.tl().y);
	out.width = MAX(r1.br().x, r2.br().x) - out.x;
	out.height = MAX(r1.br().y, r2.br().y) - out.y;

	out.width = out.height = MAX(out.width, out.height);

	if (out.x + out.width > I_W - 1 )
		out.x = I_W - out.width - 1;
	if (out.y + out.height > I_H - 1)
		out.y = I_H - out.height - 1;

	if (out.x < 0)
	{
		out.x = 0;
		out.width = I_W - 1;
	}
	if (out.y < 0)
	{
		out.y = 0;
		out.height = I_H - 1;
	}

	out.width = out.height = MIN(out.width, out.height);

	return out;
}
void RGB_AverCal(Mat mat, vector<float>&v)
{
	vector<Mat>sp;
	split(mat, sp);
	v.clear();
	//printf("spsize = %d\n", sp.size());
	for (int i = 0; i < sp.size(); i++)
	{
		float temp = 0.;
		for (int a = 0; a < mat.cols; a++)
		for (int b = 0; b < mat.rows; b++)
		{
			temp += (float)(sp[i].at<uchar>(a, b)*1.0);
		}
		temp = temp / (float)(mat.cols*mat.rows*1.0);
		v.push_back(temp);
	}
}
