#include "DataShow.h"

Mat DataShow;			//显示曲线
Mat src;				//图像
int reddata[3][PSAM_R];
int greendata[3][PSAM_G];
int negdata[3][NSAM];

MatND hist;
Mat histshow;

const int histSize = HIST_HDIMS;
float hranges_arr[2] = { 0, 255 };
const float* ranges[3] = { hranges_arr, hranges_arr, hranges_arr };
int channels[] = { 0, 1, 2 };

void Coor_RecDraw(int xoffleft, int yoffleft, int xoffright, int yoffright)
{
	if (xoffleft + xoffright >= DataShow.cols || yoffleft + yoffright >= DataShow.rows)
	{
		cout << "Invalid input parameter!" << endl;
		exit(0);
		return;
	}
	Rect coor;
	coor.x = xoffleft;
	coor.y = yoffleft;
	coor.width = DataShow.cols - xoffleft - xoffright;
	coor.height = DataShow.rows - yoffleft - yoffright;
	rectangle(DataShow, coor, Scalar(255, 255, 255), 1);
}
void CoorNumDraw(int xbin, int ybin, int xstart, int ystart, int xstep, int ystep)
{
	char a[10] = "";
	int ykain = 2;
	for (int i = 0; i < xbin; i++)
	{
		sprintf(a, "%d", xstart + i*xstep);
		int xx = Xoffleft + i*xstep;
		int yy = DataShow.rows - Yoffright;
		putText(DataShow, a, Point(xx - 10, yy + 15), 0.f, 0.5f, Scalar(255, 255, 255));
		line(DataShow, Point(xx, yy - 5), Point(xx, yy + 5), Scalar(255, 255, 255), 1);
	}
	for (int i = 1; i < ybin; i++)		
	{
		sprintf(a, "%d", ystart + i*ystep);
		int xx = Xoffleft;
		int yy = DataShow.rows - Yoffright - i*ystep*ykain;
		putText(DataShow, a, Point(xx - 35, yy+2), 0.f, 0.5f, Scalar(255, 255, 255));
		line(DataShow, Point(xx - 5, yy), Point(xx + 5, yy), Scalar(255, 255, 255), 1);
	}
}
void Grid_Draw(int xbin, int ybin, int xstart, int ystart, int xstep, int ystep)
{
	int ykain = 2;
	for (int i = 1;; i++)
	{
		if (Xoffleft + i*xstep >= DataShow.cols - Xoffright)
			break;
		line(DataShow, Point(Xoffleft + i*xstep, DataShow.rows - Yoffright), Point(Xoffleft + i*xstep, Yoffleft), Scalar(255, 255, 255), 1,8);
	}
	for (int i = 1;; i++)
	{
		if (DataShow.rows - Yoffright - i*ystep*ykain <= Yoffleft)
			break;
		line(DataShow, Point(Xoffleft, DataShow.rows - Yoffright - i*ystep*ykain), Point(DataShow.cols - Xoffright, DataShow.rows - Yoffright - i*ystep*ykain), Scalar(255, 255, 255), 1, 8);
	}
}
void DataShow_Init(int width, int height)
{
	src = Mat::zeros(Size(64, 64), CV_8UC3);

	DataShow = Mat::zeros(Size(width, height), CV_32FC3);
	Coor_RecDraw(Xoffleft, Yoffleft, Xoffright, Yoffright);
	CoorNumDraw(13, 8, 0, 0, 100, 40);
	Grid_Draw(13, 8, 0, 0, 100, 40);
}
void ChaH_Ave_Cal(int start, int end, int flag)
{
	char name[100] = "";
	//vector<Mat>sp;
	switch (flag)
	{
	case 0:		//red
		for (int i = start; i <= end; i++)
		{
			sprintf(name, "../../../SAM/POS_R/Pos_R%d.jpg", i);
			src = imread(name);
			cvtColor(src, src, CV_BGR2HSV);
			//split(src, sp);	//H-S-V
			Scalar a = mean(src, Mat());	//H-S-V
			reddata[0][i] = a[0];
			reddata[1][i] = a[1];
			reddata[2][i] = a[2];
			if (i % 100 == 0)
				cout << "Red " << i << " Finish!" << endl;
		}
		break;
	case 1:		//green
		for (int i = start; i <= end; i++)
		{
			sprintf(name, "../../../SAM/POS_G/Pos_G%d.jpg", i);
			src = imread(name);
			cvtColor(src, src, CV_BGR2HSV);
			//split(src, sp);	//H-S-V
			Scalar a = mean(src, Mat());
			greendata[0][i] = a[0];
			greendata[1][i] = a[1];
			greendata[2][i] = a[2];
			if (i % 100 == 0)
				cout << "Green " << i << " Finish!" << endl;
		}
		break;
	case 2:		//neg
		for (int i = start; i <= end; i++)
		{
			sprintf(name, "../../../SAM/NEG/NEG%d.jpg", i);
			src = imread(name);
			cvtColor(src, src, CV_BGR2HSV);
			//split(src, sp);	//H-S-V
			Scalar a = mean(src, Mat());
			negdata[0][i] = a[0];
			negdata[1][i] = a[1];
			negdata[2][i] = a[2];
			if (i % 100 == 0)
				cout << "Neg " << i << " Finish!" << endl;
		}
		break;
	default:
		break;
	}
}
void DataDraw(int flag, int channel)
{
	if (channel > 3 || channel < 0)
	{
		cout << "Error!\n";
		exit(0);
	}
	int ykain = 2;
	if (flag == 0)
	{
		for (int i = 1; i < 1000; i++)
		{
			int xxstart = Xoffleft + i - 1;
			int xxend = Xoffleft + i;
			int yyredstart = DataShow.rows - Yoffright - reddata[channel][i - 1] * ykain;
			int yyredend = DataShow.rows - Yoffright - reddata[channel][i] * ykain;
			if (channel == 0)
				line(DataShow, Point(xxstart, yyredstart), Point(xxend, yyredend), Scalar(0, 0, 255), 1);	//red
			if (channel == 1)
				line(DataShow, Point(xxstart, yyredstart), Point(xxend, yyredend), Scalar(255, 0, 0), 1);	//blue
			if (channel == 2)
				line(DataShow, Point(xxstart, yyredstart), Point(xxend, yyredend), Scalar(0, 255, 0), 1);	//green
		}
	}
	else if (flag == 1)
	{
		for (int i = 1; i < 1000; i++)
		{
			int xxstart = Xoffleft + i - 1;
			int xxend = Xoffleft + i;
			int yygreenstart = DataShow.rows - Yoffright - greendata[channel][i - 1] * ykain;
			int yygreenend = DataShow.rows - Yoffright - greendata[channel][i] * ykain;
			if (channel == 0)
				line(DataShow, Point(xxstart, yygreenstart), Point(xxend, yygreenend), Scalar(0, 255, 0), 1);	//green
			if (channel == 1)
				line(DataShow, Point(xxstart, yygreenstart), Point(xxend, yygreenend), Scalar(0, 0, 255), 1);	//red
			if (channel == 2)
				line(DataShow, Point(xxstart, yygreenstart), Point(xxend, yygreenend), Scalar(255, 0, 0), 1);	//blue
		}
	}
	else if (flag == 2)
	{
		for (int i = 1; i < 1000; i++)
		{
			int xxstart = Xoffleft + i - 1;
			int xxend = Xoffleft + i;
			int yynegstart = DataShow.rows - Yoffright - negdata[channel][i - 1] * ykain;
			int yynegend = DataShow.rows - Yoffright - negdata[channel][i] * ykain;
			if (channel == 0)
				line(DataShow, Point(xxstart, yynegstart), Point(xxend, yynegend), Scalar(255, 0, 0), 1);
			if (channel == 1)
				line(DataShow, Point(xxstart, yynegstart), Point(xxend, yynegend), Scalar(0, 255, 0), 1);
			if (channel == 1)
				line(DataShow, Point(xxstart, yynegstart), Point(xxend, yynegend), Scalar(0, 0, 255), 1);
		}
	}
	else;
}

void Hist_Init(void)
{
	hist = Mat::zeros(Size(1, HIST_HDIMS), CV_32FC3);
	histshow = Mat::zeros(Size(1000, 720), CV_8UC3);
}
void Hist_Draw(Mat hist, float maxval, int yoffset, Scalar s)
{
	int offset = 5;			//zero offset
	int scale = 3;			//width of each reatangle
	float kain = 640;		//height kain

	for (int i = 0; i < HIST_HDIMS; i++)
	{
		float bin_val = hist.at<float>(i);
		int intensity = cvRound(bin_val*kain);
		//cout << "bin_val[" << i << "] = " << bin_val << "\n";
		rectangle(histshow, Point(offset + i*scale, histshow.rows - yoffset - intensity), Point((i + 1)*scale, histshow.rows - yoffset), s, CV_FILLED);
	}
	rectangle(histshow, Point(offset + 230 * scale, 0), Point(offset + 230 * scale + 1, histshow.rows - 1), Scalar(255, 255, 255), CV_FILLED);
}
void HistGet(Mat img)
{
	vector<Mat>sp;
	Mat hsv;
	//cvtColor(img, hsv, CV_BGR2HSV_FULL);
	//cvtColor(img, img, CV_BGR2GRAY);
	//split(hsv, sp);
	split(img, sp);

	calcHist(&sp[0], 1, channels, Mat(), hist, 1, &histSize, ranges, true, false);		//B-G-R
	for (int i = 0; i < HIST_HDIMS; i++)			//手动归一化
	{
		hist.at<float>(i) = hist.at<float>(i) / (SAM_W  * SAM_H);
		//hist.at<float>(0, i) = hist.at<float>(0, i) / (SAM_W  * SAM_H);
	}
	Hist_Draw(hist, 0., 2, Scalar(0, 0, 255));

	calcHist(&sp[1], 1, channels, Mat(), hist, 1, &histSize, ranges, true, false);		//B-G-R
	for (int i = 0; i < HIST_HDIMS; i++)			//手动归一化
	{
		hist.at<float>(i) = hist.at<float>(i) / (SAM_W  * SAM_H);
		//hist.at<float>(0, i) = hist.at<float>(0, i) / (SAM_W  * SAM_H);
	}
	Hist_Draw(hist, 0., 200, Scalar(0, 0, 255));

	calcHist(&sp[2], 1, channels, Mat(), hist, 1, &histSize, ranges, true, false);		//B-G-R
	for (int i = 0; i < HIST_HDIMS; i++)			//手动归一化
	{
		hist.at<float>(i) = hist.at<float>(i) / (SAM_W  * SAM_H);
		//hist.at<float>(0, i) = hist.at<float>(0, i) / (SAM_W  * SAM_H);
	}
	Hist_Draw(hist, 0., 400, Scalar(0, 0, 255));
}
void Letter_Show(char *buf, Point point, Mat& img, Scalar s)
{
	putText(img, buf, point, FONT_HERSHEY_COMPLEX, 0.5f, s);
}
float Hist_Wei_Cal(Mat hist, int binstart, int binend)
{
	float sum = 0.;
	int Nbinstart = binstart % HIST_HDIMS;
	int Nbinend = binend % HIST_HDIMS;
	for (int i = Nbinstart; i <= Nbinend; i++)
	{
		sum += hist.at<float>(0, i);
	}
	return sum;
}

void Hist_Statistics_Init(Hist_Statistics* statistics)
{
	/*
	typedef struct Hist_Statistics		//直方图统计结构体
	{
	//统计时需要将所有图片resize到64*64下
	int Is_Num_Count[6];			//B-G-R-H-S-V的256个数值中不为零的个数
	int Node_Thres[2][6];			//B-G-R-H-S-V选择的12个节点
	float Bet_Node_Count[6];		//在节点区间的且不为零的个数
	}Hist_Statistics;
	*/
	for (int i = 0; i < 6; i++)
	{
		statistics->Is_Num_Count[i] = 0;
		statistics->Node_Thres[0][i] = 0;
		statistics->Node_Thres[1][i] = 0;
		statistics->Bet_Node_Count[i] = 0.;
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
void Hist_Is_Num(Mat img, Hist_Statistics *hist_statistics, uchar flag)
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
				hist_statistics->Is_Num_Count[i] = Hist_Ch_Num(spbgr[i % 3]);
			}
		}
		if (i >= 3)
		{
			if (flagtemp)
			{
				//cout << "i = " << i << endl;
				hist_statistics->Is_Num_Count[i] = Hist_Ch_Num(sphsv[i % 3]);
			}
		}
	}
	return;
}
void Hist_Node_Set(Hist_Statistics *hist_statistics, int start[], int end[], uchar flag)
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
			hist_statistics->Node_Thres[0][i] = start[i];
			hist_statistics->Node_Thres[1][i] = end[i];
		}
	}
	return;
}
void Hist_Node_Cal(Mat img, Hist_Statistics *hist_statistics, uchar flag)
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
				hist_statistics->Bet_Node_Count[i] = Hist_Ch_Node_Num(spbgr[i % 3], hist_statistics->Node_Thres[0][i], hist_statistics->Node_Thres[1][i]);
			}
		}
		if (i >= 3)
		{
			if (flagtemp)
			{
				hist_statistics->Bet_Node_Count[i] = Hist_Ch_Node_Num(sphsv[i % 3], hist_statistics->Node_Thres[0][i], hist_statistics->Node_Thres[1][i]);
			}
		}
		hist_statistics->Node_Proportion[i] = (float)(hist_statistics->Bet_Node_Count[i] * 1.0) / (float)(SAM_W*SAM_H*1.0);
	}
	return;
}
int Hist_Ch_Node_Num(Mat img, int start, int end)
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
void Hist_Variance_Cal(Mat img, Hist_Statistics *hist_statistics, uchar flag)
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
				hist_statistics->Is_Num_Stddev[i] = Hist_Ch_Variance_Cal(spbgr[i % 3]);
			}
		}
		if (i >= 3)
		{
			if (flagtemp)
			{
				hist_statistics->Is_Num_Stddev[i] = Hist_Ch_Variance_Cal(sphsv[i % 3]);
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
void Entropy_Cal(Mat img, Hist_Statistics *hist_statistics, uchar flag)
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
				hist_statistics->Two_Dim_Entropy[i] = Ch_Entropy_Cal(spbgr[i % 3]);
			}
		}
		if (i >= 3)
		{
			if (flagtemp)
			{
				hist_statistics->Two_Dim_Entropy[i] = Ch_Entropy_Cal(sphsv[i % 3]);
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

int Sig_Ch_Node_Greater(Mat img, int start, int end, int threshold)
{
	if (img.channels() != 1)
		return 0;
	int binstart = start%HIST_HDIMS;
	int binend = end%HIST_HDIMS;
	Mat hist = Mat::zeros(Size(1, HIST_HDIMS), CV_32FC1);
	calcHist(&img, 1, channels, Mat(), hist, 1, &histSize, ranges, true, false);
	int s = 0;
	for (int i = binstart; i <= binend; i++)
	if (hist.at<float>(i) > threshold)
		s++;
	return s;
}
void Any_Ch_Node_Greater(Mat img, Hist_Statistics *hist_statistic, int threshold, uchar flag)
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
				hist_statistic->Bet_Node_Count[i] = Sig_Ch_Node_Greater(spbgr[i % 3], hist_statistic->Node_Thres[0][i], hist_statistic->Node_Thres[1][i], threshold);
			}
		}
		if (i >= 3)
		{
			if (flagtemp)
			{
				hist_statistic->Bet_Node_Count[i] = Sig_Ch_Node_Greater(sphsv[i % 3], hist_statistic->Node_Thres[0][i], hist_statistic->Node_Thres[1][i], threshold);
			}
		}
		hist_statistic->Node_Proportion[i] = (float)(hist_statistic->Bet_Node_Count[i] * 1.0) / (float)(SAM_W*SAM_H*1.0);
	}
	return;
}
float Sig_Ch_Variance(Mat img)
{
	if (img.channels() != 1)
		return 0.0;
	//先求平均
	float average = 0.0;
	for (int i = 0; i < SAM_W; i++)
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
void Any_Ch_Variance(Mat img, Hist_Statistics *hist_statistic, uchar flag)
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
				hist_statistic->Is_Num_Stddev[i] = Sig_Ch_Variance(spbgr[i % 3]);
			}
		}
		if (i >= 3)
		{
			if (flagtemp)
			{
				hist_statistic->Is_Num_Stddev[i] = Sig_Ch_Variance(sphsv[i % 3]);
			}
		}
	}
	return;
}