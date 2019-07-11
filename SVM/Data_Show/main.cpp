#include "DataShow.h"
#include "stdio.h"

Hist_Statistic hist_sta;

int startnode[6] = { 230, 230, 230, 230, 230, 230 };
int endnode[6] = { 255, 255, 255, 255, 255, 255 };

int count = 1;
int redcount = 1;
int greencount = 1;
int negcount = 1;

Mat srcred, green, srcneg, barrier;
Mat srcredb, greenb, srcnegb, barrierb;
Hist_Statistic ppred, ppgreen, ppneg, ppbarrier;

vector<Mat>srcredsp;
vector<Mat>greensp;
vector<Mat>srcnegsp;
vector<Mat>barriersp;

vector<Mat>srcredbsp;
vector<Mat>greenbsp;
vector<Mat>srcnegbsp;
vector<Mat>barrierbsp;

Mat bar;

int greensum = 0;
int barriersum = 0;

int sum1 = 0;
int sum2 = 0;

void on_trackbar(int pos)
{
	threshold(green, green, pos, 255, CV_THRESH_BINARY);
	threshold(barrier, barrier, pos, 255, CV_THRESH_BINARY);

	//imshow("green", green);
	//imshow("barrier", barrier);
	//imshow("bar", bar);
}

int main(void)
{
	//namedWindow("bar", CV_WINDOW_AUTOSIZE);
	Hist_Init();

	Hist_Statistic_Init(&ppred);
	Hist_Statistic_Init(&ppgreen);
	Hist_Statistic_Init(&ppneg);
	Hist_Statistic_Init(&ppbarrier);
	Hist_Node_Set(&ppred, startnode, endnode, CHH | CHV | CHS);
	Hist_Node_Set(&ppgreen, startnode, endnode, CHH | CHV | CHS);
	Hist_Node_Set(&ppneg, startnode, endnode, CHH | CHV | CHS);
	Hist_Node_Set(&ppbarrier, startnode, endnode, CHH | CHV | CHS);

	char name[100] = "";
	char name1[100] = "";
	char name2[100] = "";
	char name3[100] = "";

	int nThreshold = 156;

	FILE * fp;
	fp = fopen("../../../data.txt", "w");
	if (!fp)
		exit(0);

	for (int i = 0; i < PSAM_G; i++)	//PSAM_G
	{
		cout << "****************START****************\n";
		sprintf(name1, "../../../temp/green/%d.jpg", i % PSAM_G + 1);
		sprintf(name3, "../../../temp/barrier/%d.jpg", i % PSAM_B + 1);

		green = imread(name1);
		barrier = imread(name3);

		cvtColor(green, green, COLOR_BGR2HSV);
		cvtColor(barrier, barrier, COLOR_BGR2HSV);
		split(green, greensp);
		split(barrier, barriersp);
		equalizeHist(greensp[2], greensp[2]);
		equalizeHist(barriersp[2], barriersp[2]);	//V通道均衡化

		merge(greensp, green);
		merge(barriersp, barrier);
		cvtColor(barrier, barrier, COLOR_HSV2BGR);
		cvtColor(green, green, COLOR_HSV2BGR);

		cvtColor(green, green, COLOR_BGR2GRAY);
		cvtColor(barrier, barrier, COLOR_BGR2GRAY);

		bar = Mat::zeros(Size(500, 100), CV_8UC3);
		bar.setTo(255);

		Rect roi;
		roi.x = 17;
		roi.y = 17;
		roi.width = 30;
		roi.height = 30;
		
		//cvCreateTrackbar("阈值", "bar", &nThreshold, 255, on_trackbar);
		//on_trackbar(nThreshold);

		//threshold(green, green, 165, 255, CV_THRESH_BINARY);
		//threshold(barrier, barrier, 165, 255, CV_THRESH_BINARY);

		threshold(barrier, barrier, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
		threshold(green, green, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

		dilate(green, green, Mat(), Point(-1, -1), 2);
		dilate(barrier, barrier, Mat(), Point(-1, -1), 2);

		erode(green, green, Mat(), Point(-1, -1), 2);
		erode(barrier, barrier, Mat(), Point(-1, -1), 2);

		//green = green(roi);
		//barrier = barrier(roi);
		
		//greensum = Pic_Not_0_Cal(green);
		//barriersum = Pic_Not_0_Cal(barrier);

		greensum = Pic_Not_0_CirCal(green, 15, Point(32, 32));
		barriersum = Pic_Not_0_CirCal(barrier, 15, Point(32, 32));

		cout << "greensum  =  " << greensum << endl;
		cout << "barriersum = " << barriersum << endl;

		if (greensum >= 520)		//绿色误检
			sum1++;
		if (barriersum < 520)	//障碍误检
			sum2++;

		sprintf(name, "greensum = %d\tbarriersum = %d\n", greensum, barriersum);
		fprintf(fp, name);
		Mat greenmask, barriermake;

		//Any_Ch_Node_Greater(srcred, &ppred, CHH);		//统计H分布
		//Any_Ch_Node_Greater(green, &ppgreen, CHH);
		//Any_Ch_Node_Greater(srcneg, &ppneg, CHH);
		//Any_Ch_Node_Greater(barrier, &ppbarrier, CHH);

		//Hist_Variance_Cal(srcred, &ppred, CHV);		//统计V方差
		//Hist_Variance_Cal(green, &ppgreen, CHV);
		//Hist_Variance_Cal(srcneg, &ppneg, CHV);
		//Hist_Variance_Cal(barrier, &ppbarrier, CHV);
		//cvtColor(srcred, srcred, COLOR_BGR2HSV);
		//cvtColor(green, green, COLOR_BGR2HSV);


		//HistGet(green);
		//Hist_Draw(hist, 0., 300, Scalar(0, 255, 0));
		//HistGet(barrier);
		//Hist_Draw(hist, 0., 450, Scalar(255, 255, 255));

		cout << "i = " << i << endl;
		cout << "*****************END*****************\n";

		//imshow("histshow", histshow);	histshow.setTo(0);
		//imshow("srcred", srcred);
		imshow("green", green);
		//imshow("srcneg", srcneg);
		imshow("barrier", barrier);
		moveWindow("green", 600, 300);
		moveWindow("barrier", 680, 300);
		//imshow("bar", bar);
		//waitKey(0);
		waitKey(33);
	}
	fclose(fp);
	waitKey(0);
	destroyAllWindows();

	cout << "绿色误检" << sum1 << "\t" << "误检率" << ((float)(sum1*1.0)) / ((float)(PSAM_G*1.0)) << endl;
	cout << "障碍误检" << sum2 << "\t" << "误检率" << ((float)(sum2*1.0)) / ((float)(PSAM_G*1.0)) << endl;

	return 0;
}
