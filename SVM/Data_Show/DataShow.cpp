#include "DataShow.h"

const int histSize = HIST_HDIMS;
const int histSizeFiler = HIST_HDIMS;
float hranges_arr[2] = { 0, 255 };
const float* ranges[3] = { hranges_arr, hranges_arr, hranges_arr };
const float* rangesf[3] = { hranges_arr, hranges_arr, hranges_arr };
int channels[] = { 0, 1, 2 };
int channelsf[] = {0, 1, 2 };

Mat histshow,hist;

void Hist_Init(void)
{
	hist = Mat::zeros(Size(1, HIST_HDIMS), CV_32FC3);
	histshow = Mat::zeros(Size(1000, 600), CV_8UC3);
}
void Hist_Draw(Mat hist, float maxval, int yoffset, Scalar s)
{
	int offset = 5;			//zero offset
	int scale = 3;			//width of each reatangle
	float kain = 720;		//height kain
	
	for (int i = 0; i < HIST_HDIMS; i++)
	{
		float bin_val = hist.at<float>(i);
		//float bin_val = hist.at<float>(0, i);
		int intensity = cvRound(bin_val*kain);
		//cout << "bin_val[" << i << "] = " << bin_val << "\n";
		rectangle(histshow, Point(offset + i*scale, histshow.rows - yoffset - intensity), Point((i + 1)*scale, histshow.rows - yoffset), s, CV_FILLED);
	}
	rectangle(histshow, Point(offset + 225 * scale, 0), Point(offset + 225 * scale + 1, histshow.rows - 1), Scalar(255, 255, 255), CV_FILLED);
}
void HistGet(Mat img)
{
	vector<Mat>sp;
	Mat hsv;
	cvtColor(img, hsv, CV_BGR2HSV_FULL);
	//cvtColor(img, img, CV_BGR2GRAY);
	//split(hsv, sp);
	split(img, sp);
	calcHist(&sp[0], 1, channels, Mat(), hist, 1, &histSize, ranges, true, false);		//H
	//calcHist(&img, 1, channels, Mat(), hist, 1, &histSize, ranges, true, false);		//GRAY
	//normalize(hist, hist, 0., 1., NORM_MINMAX, -1, Mat());

	for (int i = 0; i < HIST_HDIMS; i++)			//手动归一化
	{
		hist.at<float>(i) = hist.at<float>(i) / (SAM_W  * SAM_H);
		//hist.at<float>(0, i) = hist.at<float>(0, i) / (SAM_W * SAM_H);
	}
}
void Letter_Show(char *buf, Point point, Mat& img, Scalar s)
{
	putText(img, buf, point, FONT_HERSHEY_COMPLEX, 0.5f, s);
}

void Hist_Statistic_Init(Hist_Statistic* statistic)
{
	/*
	typedef struct Hist_Statistic		//直方图统计结构体
	{
	//统计时需要将所有图片resize到64*64下
	int Is_Num_Count[6];			//B-G-R-H-S-V的256个数值中不为零的个数
	int Node_Thres[2][6];			//B-G-R-H-S-V选择的12个节点
	float Bet_Node_Count[6];		//在节点区间的且不为零的个数
	}Hist_Statistic;
	*/
	for (int i = 0; i < 6; i++)
	{
		statistic->Is_Num_Count[i] = 0;
		statistic->Node_Thres[0][i] = 0;
		statistic->Node_Thres[1][i] = 0;
		statistic->Bet_Node_Count[i] = 0.;
	}
}
int Hist_Ch_Num(Mat img)
{
	Mat hist = Mat::zeros(Size(1, HIST_HDIMS), CV_32FC1);
	calcHist(&img, 1, channels, Mat(), hist, 1, &histSize, ranges, true, false);
	int s = 0;
	for (int i = 0; i < HIST_HDIMS;i++)
	if (hist.at<float>(i) != 0)
		s++;
	return s;
}
void Hist_Is_Num(Mat img, Hist_Statistic *hist_statistic, uchar flag)
{
	Mat hsv;
	vector<Mat>spbgr;
	vector<Mat>sphsv;

	cvtColor(img, hsv, CV_BGR2HSV_FULL);
	split(hsv, sphsv);
	split(img, spbgr);

	if ((flag >> 6) != 0)	//如果前两位有数值
	{
		cout << "Input error!" << endl;
		return;
	}
	for (int i = 0; i < 6; i++)		//BGR
	{
		uchar flagtemp = (flag >> i) & 0x01;
		if (i < 3)	//B-G-R
		{
			if (flagtemp)		//说明这个通道需要统计
			{
				//cout << "i = " << i << endl;
				hist_statistic->Is_Num_Count[i] = Hist_Ch_Num(spbgr[i % 3]);
			}
		}
		if (i >= 3)
		{
			if (flagtemp)
			{
				//cout << "i = " << i << endl;
				hist_statistic->Is_Num_Count[i] = Hist_Ch_Num(sphsv[i % 3]);
			}
		}
	}
	return;
}
void Hist_Node_Set(Hist_Statistic *hist_statistic, int start[], int end[], uchar flag)
{
	if ((flag >> 6) != 0)	//如果前两位有数值
	{
		cout << "Input error!" << endl;
		return;
	}
	for (int i = 0; i < 6; i++)		//BGR
	{
		uchar flagtemp = (flag >> i) & 0x01;
		if (flagtemp)		//说明这个通道需要统计
		{
			hist_statistic->Node_Thres[0][i] = start[i];
			hist_statistic->Node_Thres[1][i] = end[i];
		}
	}
	return;
}
void Any_Ch_Node_Greater(Mat img, Hist_Statistic *hist_statistic, uchar flag)
{
	Mat hsv;
	vector<Mat>spbgr;
	vector<Mat>sphsv;

	cvtColor(img, hsv, CV_BGR2HSV_FULL);
	split(hsv, sphsv);
	split(img, spbgr);

	if ((flag >> 6) != 0)	//如果前两位有数值
	{
		cout << "Input error!" << endl;
		return;
	}
	for (int i = 0; i < 6; i++)		//BGR
	{
		uchar flagtemp = (flag >> i) & 0x01;
		if (i < 3)	//B-G-R
		{
			if (flagtemp)		//说明这个通道需要统计
			{
				hist_statistic->Bet_Node_Count[i] = Sig_Ch_Node_Greater(spbgr[i % 3], hist_statistic->Node_Thres[0][i], hist_statistic->Node_Thres[1][i]);
			}
		}
		if (i >= 3)
		{
			if (flagtemp)
			{
				hist_statistic->Bet_Node_Count[i] = Sig_Ch_Node_Greater(sphsv[i % 3], hist_statistic->Node_Thres[0][i], hist_statistic->Node_Thres[1][i]);
			}
		}
		hist_statistic->Node_Proportion[i] = (float)(hist_statistic->Bet_Node_Count[i] * 1.0) / (float)(SAM_W*SAM_H*1.0);
	}
	return;
}
int Sig_Ch_Node_Greater(Mat img, int start, int end)
{
	int binstart = start%HIST_HDIMS;
	int binend = end%HIST_HDIMS;
	Mat hist = Mat::zeros(Size(1, HIST_HDIMS), CV_32FC1);
	calcHist(&img, 1, channels, Mat(), hist, 1, &histSize, ranges, true, false);
	int s = 0;
	for (int i = binstart; i <= binend; i++)
	if (hist.at<float>(i) != 0)
		s++;
	return s;
}
void Hist_Variance_Cal(Mat img, Hist_Statistic *hist_statistic, uchar flag)
{
	Mat hsv;
	vector<Mat>spbgr;
	vector<Mat>sphsv;

	cvtColor(img, hsv, CV_BGR2HSV_FULL);
	split(hsv, sphsv);
	split(img, spbgr);

	if ((flag >> 6) != 0)	//如果前两位有数值
	{
		cout << "Input error!" << endl;
		return;
	}
	for (int i = 0; i < 6; i++)		//BGR
	{
		uchar flagtemp = (flag >> i) & 0x01;
		if (i < 3)	//B-G-R
		{
			if (flagtemp)		//说明这个通道需要统计
			{
				hist_statistic->Is_Num_Stddev[i] = Hist_Ch_Variance_Cal(spbgr[i % 3]);
			}
		}
		if (i >= 3)
		{
			if (flagtemp)
			{
				hist_statistic->Is_Num_Stddev[i] = Hist_Ch_Variance_Cal(sphsv[i % 3]);
			}
		}
	}
	return;
}
float Hist_Ch_Variance_Cal(Mat img)
{
	//先求平均
	float average = 0.0;
	for (int i = 0; i < SAM_W;i++)
	for (int j = 0; j < SAM_H; j++)
		average += (float)(img.at<uchar>(i, j)*1.0);
	average = average / (float)(SAM_W*SAM_H*1.0);
	//再求方差
	float variance = 0.0;
	for (int i = 0; i < SAM_W; i++)
	for (int j = 0; j < SAM_H; j++)
		variance += pow(((float)(img.at<uchar>(i, j)*1.0) - average), 2);
	variance = variance / ((float)(SAM_W*SAM_H*1.0) - 1.0);
	return variance;
}
float Ch_Entropy_Cal(Mat img)
{
	//cout << "img 通道数" << img.channels() << endl;

	Mat entropy = Mat::zeros(Size(img.cols, img.rows), CV_8UC1);

	float temp[256][256] = { 0.0 };
	float NN = SAM_H*SAM_W*1.0;
	float result = 0.0;

	for (int x = 0; x < img.cols; x++)
	for (int y = 0; y < img.rows; y++)
	{
		entropy.at<uchar>(y, x) = Eight_NP_Cal(img, x, y);	//生成均值图
		temp[img.at<uchar>(y, x)][entropy.at<uchar>(y, x)] = temp[img.at<uchar>(y, x)][entropy.at<uchar>(y, x)] + 1;
	}
	for (int i = 0; i < 256; i++)
	for (int j = 0; j < 256; j++)
	{
		if (temp[i][j] != 0)
		{
			temp[i][j] = temp[i][j] / NN;
			result = result - temp[i][j] * (log(temp[i][j]) / log(2.0));
		}
	}
	return result;
}
void Entropy_Cal(Mat img, Hist_Statistic *hist_statistic, uchar flag)
{
	Mat hsv;
	vector<Mat>spbgr;
	vector<Mat>sphsv;

	cvtColor(img, hsv, CV_BGR2HSV_FULL);
	split(hsv, sphsv);
	split(img, spbgr);

	if ((flag >> 6) != 0)	//如果前两位有数值
	{
		cout << "Input error!" << endl;
		return;
	}
	for (int i = 0; i < 6; i++)		//BGR
	{
		uchar flagtemp = (flag >> i) & 0x01;
		if (i < 3)	//B-G-R
		{
			if (flagtemp)		//说明这个通道需要统计
			{
				hist_statistic->Two_Dim_Entropy[i] = Ch_Entropy_Cal(spbgr[i % 3]);
			}
		}
		if (i >= 3)
		{
			if (flagtemp)
			{
				hist_statistic->Two_Dim_Entropy[i] = Ch_Entropy_Cal(sphsv[i % 3]);
			}
		}
	}
	return;
}

uchar Eight_NP_Cal(Mat img, int x, int y)
{
	int res = 0;
	int f = 0;
	for (int i = y - 1; i <= y + 1; i++)
	for (int j = x - 1; j <= x + 1; j++)
	{
		if (i == y && j == x)
			continue;
		if (i >= 0 && i < img.cols && j >= 0 && j < img.rows)
		{
			res += img.at<uchar>(i, j);
			f++;
		}
	}
	if (f != 0)
	{
		res = res / f;
		return res;
	}
	else
		return 0;
}
void Color_Balance(Mat &src)
{
	int histo[HIST_HDIMS] = { 0 };
	int pixels = src.cols*src.rows;

	for (int i = 0; i < src.cols; ++i)
	for (int j = 0; j < src.rows; ++j)
		histo[src.at<uchar>(i, j)]++;
	for (int i = 1; i < HIST_HDIMS; ++i)
		histo[i] = histo[i] + histo[i - 1];

	double s = 0.99;	//0.1
	int vmin = 0;

	while (histo[vmin + 1] <= cvRound(pixels*s / 2) && vmin <= HIST_HDIMS - 1 - 1)
		vmin = vmin + 1;

	int vmax = 255 - 1;
	while (histo[vmax - 1] > cvRound(pixels*(1 - s / 2)) && vmax >= 1)
		vmax = vmax - 1;
	if (vmax < 255 - 1)
		vmax = vmax + 1;
	for (int i = 0; i < src.cols; ++i)
	for (int j = 0; j < src.rows; ++j)
	{
		if (src.at<uchar>(i, j) < vmin)
			src.at<uchar>(i, j) = vmin;
		if (src.at<uchar>(i, j)> vmax)
			src.at<uchar>(i, j) = vmax;
	}
	for (int i = 0; i < src.cols; ++i)
	for (int j = 0; j < src.rows; ++j)
	{
		src.at<uchar>(i, j) = cvRound((src.at<uchar>(i, j) - vmin)*255.0 / (vmax - vmin));
	}
}
Mat HSI_Thres(Mat img)
{
	Mat dst = Mat::zeros(Size(img.cols, img.rows), CV_8UC1);

	uchar data_B, data_G, data_R;
	float data_I, data_S, data_H;

	for (int i = 0; i<img.rows; i++)
	{
		for (int j = 0; j<img.cols; j++)
		{
			Vec3b data = img.at<Vec3b>(i, j);

			data_B = data.val[0];
			data_G = data.val[1];
			data_R = data.val[2];

			data_I = (data_R + data_G + data_B) / 3.0;
			//data_I = 0;
			int temp = (data_R>data_G) ? data_G : data_R;
			temp = (temp>data_B) ? data_B : temp;
			data_S = 1 - 3.0*temp / (data_R + data_G + data_B);
			float deno, nume;
			nume = (data_R - data_G + data_R - data_B) / 2.0;
			deno = sqrtf((data_R - data_G)*(data_R - data_G) + (data_R - data_B)*(data_G - data_B));
			if (deno == 0)
			{
				data_H = 0;
			}
			else
			{
				float theta = acosf(nume / deno) * 180 / CV_PI;
				if (data_B <= data_G)
				{
					data_H = theta;
				}
				else
				{
					data_H = 360 - theta;
				}
			}
			/*if(fabs(data_H)<40|fabs(data_H-360)<40)
			{
				if(data_S>0.1)
				{
					img_binary.at<uchar>(i,j)=255;
					img_binary_forever.at<uchar>(i,j) = 255;
				}
			}*/
			if (data_I<70)
			{
				if (data_S>0.08)
					dst.at<uchar>(i, j) = 255;				
			}
		}
	}
	return dst;
}

int Pic_Not_0_Cal(Mat img)
{
	int sum = 0;
	for (int i = 0; i < img.cols; i++)
	for (int j = 0; j < img.rows; j++)
	if (img.at<uchar>(i, j) != 0)
		sum++;
	return sum;
}
int Pic_Not_0_CirCal(Mat img, int r, Point center)
{
	int sum = 0;

	for (int i = 0; i < img.cols; i++)
	for (int j = 0; j < img.rows; j++)
	{
		if (pow(center.x - i, 2) + pow(center.y - j, 2) - pow(r, 2) <= 0)
		if (img.at<uchar>(i, j) != 0)
			sum++;
	}
	return sum;
}
