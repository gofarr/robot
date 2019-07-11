#include "DataShow.h"

Hist_Statistics pp1, pp2;

int startnode[6] = { 230, 230, 230, 230, 230, 230 };
int endnode[6] = { 255, 255, 255, 255, 255, 255 };

int main(void)
{
	//Hist_Is_Num(a, &aa, CHB | CHG | CHH | CHS | CHV);
	DataShow_Init(1500, 700);

	Hist_Statistics_Init(&pp1);
	Hist_Statistics_Init(&pp2);

	Hist_Node_Set(&pp1, startnode, endnode, CHH | CHV | CHS);
	Hist_Node_Set(&pp2, startnode, endnode, CHH | CHV | CHS);

	//cout << "Start cal!" << endl;
	//ChaH_Ave_Cal(1, 1000, 0);
	//ChaH_Ave_Cal(1, 1000, 1);
	//ChaH_Ave_Cal(1, 1000, 2);
	//cout << "Finish cal!" << endl;

	//cout << "Start Drawing!" << endl;
	//DataDraw(0,0);		//red b
	//DataDraw(1,0);		//red g
	//DataDraw(2,0);		//red r
	//cout << "Finish Drawing!" << endl;

	//namedWindow("DataShow");
	//moveWindow("DataShow", 50, 120);
	//imshow("DataShow", DataShow);

	Mat src;

	Hist_Init();
	char name[100] = "";
	for (int i = 0; i < 1; i++)
	{
		sprintf(name, "../../../temp/OBJ/PP%d.jpg", i + 1);
		src = imread(name);

		HistGet(src);		
		/*Hist_Draw(hist, 0., 2, Scalar(0, 0, 255));*/
		//Letter_Show("Red", Point(700, 550), histshow, Scalar(0, 0, 255));
		//HistGet(src);
		//Hist_Draw(hist, 0.,250, Scalar(0, 255, 0));
		//Letter_Show("Green", Point(700, 300), histshow, Scalar(0, 255, 0));
		//HistGet(src2);
		//Hist_Draw(hist, 0., 500, Scalar(255, 0, 0));
		//Letter_Show("Neg", Point(700, 50), histshow, Scalar(255, 0, 0));

		//Hist_Node_Cal(src, &pp1, CHH);
		//Hist_Variance_Cal(src, &pp1, CHV);

		cout << "V通道方差 = " << pp1.Is_Num_Stddev[5] << endl;
		cout << "H通道个数：" << pp1.Bet_Node_Count[3] << endl;

		Any_Ch_Variance(src, &pp1, CHV);
		Any_Ch_Node_Greater(src, &pp1, 0, CHH);

		cout << "V通道方差 = " << pp1.Is_Num_Stddev[5] << endl;
		cout << "H通道个数：" << pp1.Bet_Node_Count[3] << endl;
		//统计V通道方差

		//统计完毕
		//if (pp1.Bet_Node_Count[3] >= 16)
		//{
		//	//pp1是红色
		//	Letter_Show("src is ground", Point(100, 50), histshow, Scalar(255, 255, 255));
		//	imshow("red", src);
		//	imshow("green", src1);
		//	imshow("neg", src2);
		//	imshow("hist", histshow);
		//	//waitKey(0);
		//}

		imshow("src", src);	
		imshow("histshow", histshow);
		cout << "i = " << i << endl;
		waitKey(0);
		//waitKey(33);
		histshow.setTo(0);
	}

	waitKey(0);
	destroyAllWindows();

	return 0;
}