//使用读摄像头的方式，每一帧图像中至多有一个条形码和一块表，先识别条形码，然后根据条形码位置检测电表，
//当识别条形码之后判断该条形码是否在上一次识别过，如果识别过就不进行检表程序，如果想要退出程序需要按下“q"键

//sqlie_3.cpp : 定义控制台应用程序的入口点。
//创建数据库，创建以年月日为表名,包含条形码（bar_code）,比例（ratio），时间（TIME），时间只记录此次检表的整点数
//以循环的方式一条一条的插入数据。
//删除，先查询前一个月相同日期的表格是否存在，若存在则删除，然后创建新的表格  

#include <opencv2/opencv.hpp>
#include <iostream> 
#include "opencvcode.h"
#include <zbar.h>
#include <time.h>
#include <sys/types.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <stdio.h>
#include <string.h>

#include <ros/ros.h>
#include <img_prc/Num.h>

using namespace cv;
using namespace std;
using namespace zbar;
//system("rmmod uvcvideo");
//usleep(1000);
//system("modprobe uvcvideo qirks=128");
//usleep(100);
int sflag=0;
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	for (i = 0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

void ctrlMessageReceived(const img_prc::Num&msg)
{
	if (msg.key == 'q')
	{
		sflag=1;		
	}
}

//Mat srcImage;
int main(int argc, char* argv[])
{

	VideoCapture capture(0);
	int N = 1;//写文件中统计条形码个数
	int flag1 = 0;//跳出循环即不继续检测的标志
	int flag2 = 0;//判断是否进行电表指针检测的标志
	vector<string> M1;//记录条形码
	vector<double> M2;//记录指针比例

	// Initialize the ROS system and become a node .
 	ros::init ( argc , argv ,"publish_info" ) ;
 	ros::NodeHandle nh ;

 	// Create a publisher obj ect .
 	ros::Publisher pub = nh.advertise <img_prc::Num>("/infomation" , 1000 ) ;
	ros::Subscriber sub = nh.subscribe("/control",1000,&ctrlMessageReceived);


 	// Loop at 30Hz until the node is shut down.
 	ros::Rate rate ( 30 ) ;


	while (ros::ok())
	{

		//读取摄像头
		Mat frame;
		capture >> frame;
		if (frame.empty())
		{

			cout << "image file read error" << endl;

			return -1;
		}
		imshow("读取视频", frame);
		//cvWaitKey(30);
		//去除大部分机柜及背景图像
		Mat srcImageROI;
		srcImageROI = frame(Range(0, 1 * frame.rows), Range(0, frame.cols));
		imshow("电表及条形码区域", srcImageROI);
		//waitKey();





		//分割出条形码区域
		Mat  grayImage, blurImage, thresholdImage, gradientXImage, gradientYImage, gradientImage, morphImage;

		//图像转换为灰度图像
		if (srcImageROI.channels() == 3)
		{
			cvtColor(srcImageROI, grayImage, CV_RGB2GRAY);
		}
		else
		{
			grayImage = srcImageROI.clone();
		}

		//建立图像的梯度幅值，应用scharr滤波器
		Scharr(grayImage, gradientXImage, CV_32F, 1, 0);
		Scharr(grayImage, gradientYImage, CV_32F, 0, 1);
		//因为我们需要的条形码在需要X方向水平,所以更多的关注X方向的梯度幅值,而省略掉Y方向的梯度幅值
		subtract(gradientXImage, gradientYImage, gradientImage);
		//归一化为八位图像
		convertScaleAbs(gradientImage, gradientImage);
		//对图片进行相应的模糊化,使一些噪点消除，均值滤波
		blur(gradientImage, blurImage, Size(5, 5));
		//模糊化以后进行阈值化,得到到对应的黑白二值化图像,二值化的阈值可以根据实际情况调整
		threshold(blurImage, thresholdImage, 210, 255, THRESH_BINARY);
		//Mat tepImage0, thresholdImage_small;
		//tepImage0 = thresholdImage;
		//resize(tepImage0, thresholdImage_small, Size(tepImage0.cols / 8, tepImage0.rows / 8), (0, 0), (0, 0), 3);
		//imshow("模糊后二值图像", thresholdImage_small);
		imshow("模糊后二值图像", blurImage);
		//waitKey(0);

		//二值化以后的图像,条形码之间的黑白没有连接起来,就要进行形态学运算,消除缝隙,相当于小型的黑洞,选择闭运算
		//因为是长条之间的缝隙,所以需要选择宽度大于长度
		Mat kernel = getStructuringElement(MORPH_RECT, Size(27, 13));
		//morphologyEx(thresholdImage, morphImage, MORPH_OPEN, kernel);
		morphologyEx(thresholdImage, morphImage, MORPH_CLOSE, kernel);
		//Mat tepImage3, morphImage_small;
		//tepImage3 = morphImage;
		//resize(tepImage3, morphImage_small, Size(tepImage3.cols / 4, tepImage3.rows / 4), (0, 0), (0, 0), 3);
		//imshow("闭运算后二值图像", morphImage_small);
		imshow("闭运算后二值图像", morphImage);
		//waitKey(0);

		//现在要让条形码区域连接在一起,先腐蚀,让其他区域的亮的地方变少最好是消除,然后膨胀回来,消除干扰,迭代次数根据实际情况选择
		erode(morphImage, morphImage, getStructuringElement(MORPH_RECT, Size(9, 9)), Point(-1, -1), 4);
		dilate(morphImage, morphImage, getStructuringElement(MORPH_RECT, Size(9, 9)), Point(-1, -1), 4);
		//Mat tepImage4, morphImage1_small;
		//tepImage4 = morphImage;
		//resize(tepImage4, morphImage1_small, Size(tepImage4.cols / 4, tepImage4.rows / 4), (0, 0), (0, 0), 3);
		//imshow("腐蚀膨胀后二值图像", morphImage1_small);
		imshow("腐蚀膨胀后二值图像", morphImage);
		//waitKey(0);
		//dilate(morphImage, morphImage, getStructuringElement(MORPH_RECT, Size(3, 3)), Point(-1, -1), 4);
		//imshow("腐蚀膨胀后", morphImage);

		//接下来对目标轮廓进行查找,目标是为了计算图像面积
		vector< vector<Point2i> >contours;
		vector<float>contourArea;
		vector<Vec4i>  hierarchy;
		//查找轮廓  
		findContours(morphImage, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
		//遍历所有顶层轮廓，随机生成颜色值绘制给各连接组成部分 
		Mat drawing = Mat::zeros(morphImage.size(), CV_8UC3);
		if (!contours.empty() && !hierarchy.empty())
		{
			int idx = 0;
			for (; idx >= 0; idx = hierarchy[idx][0])
			{
				Scalar color((rand() & 255), (rand() & 255), (rand() & 255));
				//绘制填充轮廓  
				drawContours(drawing, contours, idx, color, 2, 8, hierarchy);
			}
		}
		//显示窗口  
		//Mat tepImage1, drawing_small;
		//tepImage1 = drawing;
		//resize(tepImage1, drawing_small, Size(tepImage1.cols / 4, tepImage1.rows / 4), (0, 0), (0, 0), 3);
		//imshow("Connected Components", drawing_small);
		imshow("Connected Components", drawing);
		//waitKey(0);

		//找到最大轮廓
		int num = 0, area = 0;
		for (int i = 0; i<contours.size(); i++)
		{
			//Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));  //轮廓颜色取任意值
			if (fabs(cv::contourArea(contours[i])) > area)
			{
				area = cv::contourArea(contours[i]);
				num = i;
			}
		}

		//找到所有条形码的轮廓
		vector<int> no(contours.size());
		int a = 0;
		for (int i = 0; i < contours.size(); i++)
		{
			if (fabs(cv::contourArea(contours[i])) >= area * 1)
			{
				no[a] = i + 1;
				a = a + 1;
			}
		}

		//对条形码区域进行提取
		Mat drawing2 = Mat::zeros(morphImage.size(), CV_8UC3);
		Scalar color2((rand() & 255), (rand() & 255), (rand() & 255));
		RotatedRect minRect;
		char windowNameString1[50];
		char windowNameString2[50];
		char windowNameString3[50];

		//FileStorage fs("test1.yaml", FileStorage::APPEND);

		for (int i = 0; i< a; i++)
		{
			Scalar color((rand() & 255), (rand() & 255), (rand() & 255));
			//绘制填充轮廓  
			drawContours(drawing2, contours, no[i] - 1, color, 2, 8, hierarchy);

			//提取条形码区域
			minRect = minAreaRect(contours[no[i] - 1]);
			Rect ZbarRect;
			if (minRect.angle < 2.0)
			{
				ZbarRect = boundingRect(contours[no[i] - 1]);
				//myRect.x = myRect.x;
				if (ZbarRect.x - (ZbarRect.width / 10) < 0)
				{
					ZbarRect.x = 0;
				}
				else
				{
					ZbarRect.x = ZbarRect.x - (ZbarRect.width / 10);
				}
				if (ZbarRect.width*1.2 <srcImageROI.cols - ZbarRect.x)
				{
					ZbarRect.width = ZbarRect.width*1.2;
				}
				else
				{
					ZbarRect.width = srcImageROI.cols - ZbarRect.x;
				}
				//ZbarRect.width = ZbarRect.width*1.2;
				Mat zbarImage = Mat(srcImageROI, ZbarRect);//根据这个区域，保留原图像
				snprintf(windowNameString1, 50 ,"zbar_result%d", i);
				imshow(windowNameString1, zbarImage);
				//waitKey();

				//识别条形码数值
				std::string bar_code_NO;
				bar_code_NO = recognise(zbarImage);


				if (bar_code_NO == "N")
				{
					continue;
				}

				//waitKey();

				//判断是否需要进行指针识别
				if (N >1 && M1[N - 2] == bar_code_NO)
				{
					continue;
				}

				M1.push_back(bar_code_NO);
				cout << "条形码信息2" << endl << bar_code_NO << endl << endl;
				cout << "条形码信息1" << endl << M1[N - 1] << endl << endl;

				//分割出电表区域(修改)
				cv::Rect MeterRect;
				if (ZbarRect.y - ZbarRect.width * 1.1 < 0)
				{
					MeterRect.y = 0;
				}
				else
				{
					MeterRect.y = ZbarRect.y - ZbarRect.width * 1;
				}
				MeterRect.x = ZbarRect.x;
				MeterRect.width = ZbarRect.width * 1;
				MeterRect.height = ZbarRect.width * 1;
				cv::Mat MeterImage = cv::Mat(srcImageROI, MeterRect);//根据这个区域，保留原图像
				snprintf(windowNameString2, 50,"meter_result%d", i);
				imshow(windowNameString2, MeterImage);
				imshow("电表图像", MeterImage);
				//Mat tepImage, MeterImage_small;
				//tepImage = MeterImage;
				//resize(tepImage, MeterImage_small, Size(tepImage.cols / 2, tepImage.rows / 2), (0, 0), (0, 0), 3);
				//snprintf(windowNameString3, "meter_small%d", i);
				//imshow(windowNameString3, MeterImage_small);
				//imwrite("电表图像.jpg", MeterImage_small);
				//waitKey();

				//识别指针
				double angle1;
				angle1 = pointer_recognise(MeterImage);
				M2.push_back(angle1);
				cout << "指针所占比例1" << endl << angle1 << endl << endl;
				cout << "指针所占比例2" << endl << M2[N - 1] << endl << endl;

				cout << "条形码个数" << N;
				N = N + 1;

			}


		}
		//Mat tepImage2, drawing2_small;
		//tepImage2 = drawing2;
		//resize(tepImage2, drawing2_small, Size(tepImage2.cols / 4, tepImage2.rows / 4), (0, 0), (0, 0), 3);
		//imshow("Connected Components 2", drawing2_small);
		imshow("Connected Components 2", drawing2);



		//waitKey();
		//获得了读取一张图像所需的时间单位为秒。 其中getTickFrequency()函数返回CPU一秒钟所走得时钟周期数。
		//double time2 = (static_cast<double>(getTickCount()) - time1) / getTickFrequency();
		//cout << "程序运行时间：" << time2 << "秒" << endl;//输出运行时间


		
		

		/*if (key == 'q')
		{
			break;
			
		}*/

		waitKey(30);
		ros::spinOnce();
		if(sflag==1)
		{
			break;
		}
		
		img_prc::Num msg;
		//msg.m1 = "1111454363999";
		//pub.publish ( msg );
		if(!M1.empty())
		{
		
			msg.m1 = M1[N-2];
			msg.m2 = int(M2[N-2]);
			msg.n = N;
			pub.publish ( msg );
			//cout<<M1[N-1]<<endl;
			//cout<<N<<endl;
		}
		
		// Publish the message .
 		//pub.publish ( msg ) ;
		rate.sleep ( ) ;


	}
	
	//sqlite数据库调用
	int rc;
	int i, nrows, ncols, tr;
	char *errmsg = NULL;
	char **results;

	sqlite3 *db = NULL;
	rc = sqlite3_open("cardb", &db);
	if (rc)
	{
		fprintf(stderr, "can't open db!\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}
	else
	{
		printf("db open successfully!\n");
	}

	//查询日期，时间
	struct tm *t;   //tm结构指针
	time_t cur_time = time(NULL);  //声明time_t类型变量
	//time(&now);      //获取系统日期和时间
	t = localtime(&cur_time);   //获取当地日期和时间

	//格式化输出本地时间
	printf("年：%d\n", t->tm_year + 1900);
	printf("月：%d\n", t->tm_mon + 1);
	printf("日：%d\n", t->tm_mday);
	printf("周：%d\n", t->tm_wday);
	printf("一年中：%d\n", t->tm_yday);
	printf("时：%d\n", t->tm_hour);
	printf("分：%d\n", t->tm_min);
	printf("秒：%d\n", t->tm_sec);
	printf("夏令时：%d\n", t->tm_isdst);
	char tabletime_1[100];//新的要创建的表格名
	char tabletime_2[100];//旧的要删除的表格名
	snprintf(tabletime_1, 100," T%d%d%d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
	snprintf(tabletime_2, 100," T%d%d%d", t->tm_year + 1900, t->tm_mon, t->tm_mday);
	printf("%s\n", tabletime_1);
	printf("%s\n", tabletime_2);
	//getchar();


	//删除表格
	char sql_1[100];
	char *zErrMsg_1 = 0;
	snprintf(sql_1,100, "DROP TABLE %s", tabletime_2);
	printf("%s\n", sql_1);
	//snprintf(sql_1, "DROP TABLE COMPANY");
	rc = sqlite3_exec(db, sql_1, callback, 0, &zErrMsg_1);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg_1);
		sqlite3_free(zErrMsg_1);
	}
	else{
		fprintf(stdout, "Table delete successfully\n");
	}

	//创建表格,存储总的检测次数
	/* Create SQL statement */
	char sql_4[1000];
	char *zErrMsg_4 = 0;
	snprintf(sql_4,1000, "CREATE TABLE TIMES(NO INT NOT NULL )");//创建表格
	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql_4, callback, 0, &zErrMsg_4);
	if (rc != SQLITE_OK)
	{
		//创建表格失败，证明表格已经存在
		fprintf(stderr, "SQL error: %s\n", zErrMsg_4);
		sqlite3_free(zErrMsg_4);
		sqlite3_stmt * stmt = NULL;
		const char *zTail;
		if (sqlite3_prepare_v2(db,
			"SELECT MAX(NO) FROM TIMES;", -1, &stmt, &zTail) == SQLITE_OK)
			//查询表格中次数的最大值
		{
			while (sqlite3_step(stmt) == SQLITE_ROW)
			{
				int num = sqlite3_column_int(stmt, 0);
				num = num + 1;
				printf("当前次数: %d\n", num);
				char sql_6[1000];
				char *zErrMsg_6 = 0;
				snprintf(sql_6, 1000,"INSERT INTO TIMES(NO)VALUES(%d)", num);
				//向表格中插入当前次数
				//printf("%s\n", sql_6);
				rc = sqlite3_exec(db, sql_6, callback, 0, &zErrMsg_6);
				if (rc != SQLITE_OK){
					fprintf(stderr, "SQL error: %s\n", zErrMsg_6);
					sqlite3_free(zErrMsg_6);
				}
				else{
					fprintf(stdout, "@@Records created successfully\n");
				}
			}
		}
	}
	else{
		fprintf(stdout, "Table created successfully\n");
		//插入第一条数据
		char sql_5[1000];
		char *zErrMsg_5 = 0;
		snprintf(sql_5, 1000,"INSERT INTO TIMES (NO)VALUES (1)");
		printf("%s\n", sql_5);
		rc = sqlite3_exec(db, sql_5, callback, 0, &zErrMsg_5);
		if (rc != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg_5);
			sqlite3_free(zErrMsg_5);
			char sql_5[1000];
			char *zErrMsg_5 = 0;
			snprintf(sql_5,1000,"INSERT INTO TIMES (NO)VALUES (1)");
			printf("%s\n", sql_5);
			rc = sqlite3_exec(db, sql_5, callback, 0, &zErrMsg_5);
		}
		else{
			fprintf(stdout, "Records created successfully\n");
		}
	}




	//创建表格,存储每一天的检测结果
	/* Create SQL statement */
	char sql_2[1000];
	char *zErrMsg_2 = 0;
	snprintf(sql_2, 1000,"CREATE TABLE %s(ID TEXT NOT NULL,RATIO REAL NOT NULL,TIME TEXT NOT NULL)", tabletime_1);
	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql_2, callback, 0, &zErrMsg_2);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg_2);
		sqlite3_free(zErrMsg_2);
	}
	else{
		fprintf(stdout, "Table created successfully\n");
	}

	//创建记录
	//char *sql_3;
	//char *zErrMsg_3 = 0;
	/* Create SQL statement */
	/*sql_2 = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
	"VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
	"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
	"VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
	"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
	"VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
	"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
	"VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";
	*/
	/* Execute SQL statement */
	//rc = sqlite3_exec(db, sql_2, callback, 0, &zErrMsg_2);


	for (int i = 1; i <N; i++)
	{
		char sql_3[1000];
		char *zErrMsg_3 = 0;
		
		
		const char *id;    //条形码信息是以string形式储存的，需要转成char类型
		id = M1[i-1].c_str();
		
		//snprintf(id,"%s",M1[i-1]);
		//printf("测%s\n",id);
		float angle2 = M2[i-1];
		char time[30];
		snprintf(time,30, "%d", t->tm_hour);
		snprintf(sql_3,1000, "INSERT INTO %s (ID,RATIO,TIME)VALUES (%s,%f,%s)", tabletime_1, id, angle2, time);
		printf("%s\n", sql_3);
		//sql_3 = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)VALUES (8, 'paul', 32, 'California', 20000.00)";
		rc = sqlite3_exec(db, sql_3, callback, 0, &zErrMsg_3);
		if (rc != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg_3);
			sqlite3_free(zErrMsg_3);
		}
		else{
			fprintf(stdout, "Records created successfully\n");
		}
	}



	sqlite3_close(db);
	



	FileStorage fs("test.yaml", FileStorage::WRITE);

	time_t filetime;
	time(&filetime);
	fs << "date" << asctime(localtime(&filetime));
	for (int i = 1; i <N; i++)
	{
		char bar_string[50];
		snprintf(bar_string, 50,"bar_code_NO%d", i);
		char ratio_string[50];
		snprintf(ratio_string, 50,"ratio%d", i);
		//条形码和指针比例存储
		fs << bar_string << M1[i - 1];
		fs << ratio_string << M2[i - 1];

	}
	&cv::FileStorage::release;
	
	printf("-----------------");

	//int a;
	//
	//scanf_s("%d", &a);//没有此行代码，运行窗口出现后会马山消失。
	return 0;
}
