#include "Sam.h"

Rect SAM_Rec;
int Get_Sam_Flag = 0;	//是否可以进行HOG提取的标志，1为可以，0为不可以

Point pre_pt;
Point cur_pt;

int R_f = 0, G_f = 0, B_f = 0, N_f = 0, downflag = 0;

int drawflag = 1;

Mat src, srct, dst, temp;

void Mat_Init(void)
{
	Mat src = Mat::zeros(Size(I_W, I_H), CV_8UC3);
	Mat srct = Mat::zeros(Size(I_W, I_H), CV_8UC3);
	Mat temp = Mat::zeros(Size(I_W, I_H), CV_8UC3);
	Mat dst = Mat::zeros(SAMSIZE, CV_8UC3);
}
void Sam_R_Get(char *capname, int rectw, int recth, int numstart, int numend, int flag)
{
	VideoCapture capture(capname);

	if (!capture.isOpened())
		cout << "fail to open!" << endl;

	cout << "This capture has " << capture.get(CV_CAP_PROP_FRAME_COUNT) << "frames" << endl;

	setMouseCallback("src", on_mouse, 0);

	int a = numstart;

	while (1)
	{
		if (!capture.read(src))
			break;
		char c = 'l';

		if (a > numend)break;

		if (1)
		{
			src.copyTo(temp);
			//circle(temp, Point(I_W / 2, I_H / 2), I_H / 2 - 3, Scalar(255, 255, 0), 3);
			ellipse(temp, Point(I_W / 2, I_H / 2), Size(I_W / 2, I_H / 2), 0.0, 0, 360, Scalar(0, 255, 0), 3);
			circle(temp, { I_W / 2, I_H / 2 }, 2, Scalar(0, 255, 0), -1);
			imshow("temp", temp);
			
			imshow("src", src);
			c = waitKey(0);
			//c = cvWaitKey(33);
			if (Get_Sam_Flag)
			{
				SAM_Rec.x = MIN(pre_pt.x + 2, I_W);
				SAM_Rec.y = MIN(pre_pt.y + 2, I_H);
				SAM_Rec.width = SAM_Rec.height = MIN(cur_pt.x - pre_pt.x, cur_pt.y - pre_pt.y);
				SAM_Rec.width = MAX(0, SAM_Rec.width - 2);
				SAM_Rec.height = MAX(0, SAM_Rec.height - 2);

				cout << "Rectangle size: " << "( " << SAM_Rec.width << ", " << SAM_Rec.height << " )\n";

				src = src(SAM_Rec);
				resize(src, dst, SAMSIZE, 0., 0., INTER_CUBIC);
				imshow("dst", dst);
			}
			char filename[100] = "";

			switch (flag)
			{
			case 0:
				if (a <= numend && Get_Sam_Flag && c != 'd')
				{
					sprintf(filename, "../../../SAM/POS_R/Pos_R%d.jpg", a);
					imwrite(filename, dst);
					cout << "red ok " << a++ << endl;
				}
				else if (a > numend && c != 'd')
					cout << "Red ennough!\n";
				else if (c == 'd')
					cout << "Invaild!\n";
				else if (c == 'b')
					a = numend + 1;
				else;
				break;
			case 1:
				if (a <= numend && Get_Sam_Flag && c != 'd')
				{
					sprintf(filename, "../../../SAM/POS_G/Pos_G%d.jpg", a);
					imwrite(filename, dst);
					cout << "green ok " << a++ << endl;
				}
				else if (a > numend && c != 'd')
					cout << "Green enough!\n";
				else if (c == 'd')
					cout << "Invaild!\n";
				else if (c == 'b')
					a = numend + 1;
				else;
				break;
			case 2:
				if (a <= numend && Get_Sam_Flag && c != 'd')
				{
					sprintf(filename, "../../../SAM/POS_B/Pos_B%d.jpg", a);
					imwrite(filename, dst);
					cout << "barrier ok " << a++ << endl;
				}
				else if (a > numend && c != 'd')
					cout << "Barrier enough\n";
				else if (c == 'd')
					cout << "Invaild!\n";
				else if (c == 'b')
					a = numend + 1;
				else;
				break;
			case 3:
				if (a < numend && Get_Sam_Flag && c != 'd')
				{
					int offset = 2600;
					a++;
					sprintf(filename, "../../../SAM/NEG/NEG%d.jpg", a);
					imwrite(filename, dst);
					cout << "neg ok" << a++ << endl;
				}
				else if (a > numend && c != 'd')
					cout << "Neg enough\n";					
				else if (c == 'd')
					cout << "Invaild!\n";
				else if (c == 'b')
					a = numend + 1;
				else;
				break;
			default:
				break;
			}
			Get_Sam_Flag = 0;
			drawflag = 1;
		}
	}
	cout << "Sample get over!\n";
	src.setTo(0);
	srct.setTo(0);	

	return;
}
void on_mouse(int event, int x, int y, int flags, void* ustc)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		downflag = 1;
		pre_pt = Point(x, y);
		if (downflag && drawflag)
		{
			drawflag = 0;
			Get_Sam_Flag = 0;
			src.copyTo(srct);
			src.copyTo(temp);
			circle(src, pre_pt, 3, Scalar(255, 0, 0), CV_FILLED, CV_AA, 0);
			circle(temp, pre_pt, 3, Scalar(255, 0, 0), CV_FILLED, CV_AA, 0);
			circle(temp, { I_W / 2, I_H / 2 }, 2, Scalar(0, 255, 0), -1);
			ellipse(temp, Point(I_W / 2, I_H / 2), Size(I_W / 2, I_H / 2), 0.0, 0, 360, Scalar(0, 255, 0), 3);
			imshow("src", src);
			imshow("temp", temp);
		}
	}
	else if (event == CV_EVENT_MOUSEMOVE)
	{
		if (downflag)
		{
			srct.copyTo(src);
			src.copyTo(temp);
			cur_pt = Point(x, y);
			rectangle(src, pre_pt, cur_pt, Scalar(0, 255, 0), 1, 8, 0);
			SAM_Rec.x = MIN(pre_pt.x + 2, I_W);
			SAM_Rec.y = MIN(pre_pt.y + 2, I_H);
			SAM_Rec.width = SAM_Rec.height = MIN(cur_pt.x - pre_pt.x, cur_pt.y - pre_pt.y);
			SAM_Rec.width = MAX(0, SAM_Rec.width - 2);
			SAM_Rec.height = MAX(0, SAM_Rec.height - 2);
			rectangle(temp, SAM_Rec.tl(), SAM_Rec.br(), Scalar(0, 255, 0), 1, 8, 0);
			circle(temp, Point((SAM_Rec.tl().x + SAM_Rec.br().x) / 2, (SAM_Rec.tl().y + SAM_Rec.br().y) / 2), 3, Scalar(255, 0, 0), CV_FILLED, CV_AA, 0);
			ellipse(temp, Point(I_W / 2, I_H / 2), Size(I_W / 2, I_H / 2), 0.0, 0, 360, Scalar(0, 255, 0), 3);
			imshow("temp", temp);
			imshow("src", src);
		}
	}
	else if (event == CV_EVENT_LBUTTONUP)
	{
		cur_pt = Point(x, y); 
		cur_pt.x = MIN(cur_pt.x, I_W - 1);
		cur_pt.y = MIN(cur_pt.y, I_H - 1);
		rectangle(src, pre_pt, cur_pt, Scalar(0, 255, 0), 1, 8, 0);
		SAM_Rec.x = MIN(pre_pt.x + 2, I_W);
		SAM_Rec.y = MIN(pre_pt.y + 2, I_H);
		SAM_Rec.width = SAM_Rec.height = MIN(cur_pt.x - pre_pt.x, cur_pt.y - pre_pt.y);
		SAM_Rec.width = MAX(0, SAM_Rec.width - 2);
		SAM_Rec.height = MAX(0, SAM_Rec.height - 2);
		rectangle(temp, SAM_Rec.tl(), SAM_Rec.br(), Scalar(0, 255, 0), 1, 8, 0);
		circle(temp, Point((SAM_Rec.tl().x + SAM_Rec.br().x) / 2, (SAM_Rec.tl().y + SAM_Rec.br().y) / 2), 3, Scalar(255, 0, 0), CV_FILLED, CV_AA, 0);
		ellipse(temp, Point(I_W / 2, I_H / 2), Size(I_W / 2, I_H / 2), 0.0, 0, 360, Scalar(0, 255, 0), 3);
		imshow("temp", temp);
		imshow("src", src);
		Get_Sam_Flag = 1;
		downflag = 0;
	}else;
}