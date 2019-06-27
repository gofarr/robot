#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <math.h>
#include <linux/videodev2.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <img_prc/Num.h>

using namespace cv;
using namespace std;

typedef struct
{
	void *start;
	int length;
}BUFTYPE;

BUFTYPE *user_buf;
int n_buffer = 0;
int img_length=0;
char img_len[5];
int mainflag=1;
unsigned char imgbuffer[640*480*2];

int angle=0,dist=0;
double dist1=0,angle1=0;

void ctrlMessageReceived(const img_prc::Num&msgc)
{
	if (msgc.key == 'q')
	{
		ros::shutdown();
	}
}

int open_camer_device()
{
	int fd;

	if((fd = open("/dev/video0", O_RDWR, O_NONBLOCK)) < 0)
	{
		perror("Fail to open");
		exit(EXIT_FAILURE);
	} 

	return fd;
}

int init_mmap(int fd)
{
	int i = 0;
	struct v4l2_requestbuffers reqbuf;

	bzero(&reqbuf,sizeof(reqbuf));
	reqbuf.count = 4;
	reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	reqbuf.memory = V4L2_MEMORY_MMAP;
	

	if(-1 == ioctl(fd,VIDIOC_REQBUFS,&reqbuf))
	{
		perror("Fail to ioctl 'VIDIOC_REQBUFS'");
		exit(EXIT_FAILURE);
	}
	
	n_buffer = reqbuf.count;
	
	printf("n_buffer = %d\n",n_buffer);

	user_buf = (BUFTYPE*)calloc(reqbuf.count,sizeof(*user_buf));
	if(user_buf == NULL){
		fprintf(stderr,"Out of memory\n");
		exit(EXIT_FAILURE);
	}


	for(i = 0; i < reqbuf.count; i ++)
	{
		struct v4l2_buffer buf;
		
		bzero(&buf,sizeof(buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		
		if(-1 == ioctl(fd,VIDIOC_QUERYBUF,&buf))
		{
			perror("Fail to ioctl : VIDIOC_QUERYBUF");
			exit(EXIT_FAILURE);
		}

		user_buf[i].length = buf.length;
		user_buf[i].start = 
			mmap(
					NULL,/*start anywhere*/
					buf.length,
					PROT_READ | PROT_WRITE,
					MAP_SHARED,
					fd,buf.m.offset
				);
		if(MAP_FAILED == user_buf[i].start)
		{
			perror("Fail to mmap");
			exit(EXIT_FAILURE);
		}
	}	

	return 0;
}

int init_camer_device(int fd)
{
	struct v4l2_fmtdesc fmt;
	struct v4l2_capability cap;
	struct v4l2_format stream_fmt;
	int ret;
	
	
	memset(&fmt,0,sizeof(fmt));
	fmt.index = 0;
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	while((ret = ioctl(fd,VIDIOC_ENUM_FMT,&fmt)) == 0)
	{
		fmt.index ++ ;

		printf("{pixelformat = %c%c%c%c},description = '%s'\n",
				fmt.pixelformat & 0xff,(fmt.pixelformat >> 8)&0xff,
				(fmt.pixelformat >> 16) & 0xff,(fmt.pixelformat >> 24)&0xff,
				fmt.description);
	}

	
	ret = ioctl(fd,VIDIOC_QUERYCAP,&cap);
	if(ret < 0){
		perror("FAIL to ioctl VIDIOC_QUERYCAP");
		exit(EXIT_FAILURE);
	}

	
	if(!(cap.capabilities & V4L2_BUF_TYPE_VIDEO_CAPTURE))
	{
		printf("The Current device is not a video capture device\n");
		exit(EXIT_FAILURE);
	
	}

	
	if(!(cap.capabilities & V4L2_CAP_STREAMING))
	{
		printf("The Current device does not support streaming i/o\n");
		exit(EXIT_FAILURE);
	}

	
	stream_fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	stream_fmt.fmt.pix.width = 640;
	stream_fmt.fmt.pix.height = 480;
	stream_fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	stream_fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

	if(-1 == ioctl(fd,VIDIOC_S_FMT,&stream_fmt))
	{
		perror("Fail to ioctl");
		exit(EXIT_FAILURE);
	}
	
	init_mmap(fd);
	
	return 0;
}

int start_capturing(int fd)
{
	unsigned int i;
	enum v4l2_buf_type type;

	
	for(i = 0;i < n_buffer;i ++)
	{
		struct v4l2_buffer buf;

		bzero(&buf,sizeof(buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		
		if(-1 == ioctl(fd,VIDIOC_QBUF,&buf))
		{
			perror("Fail to ioctl 'VIDIOC_QBUF'");
			exit(EXIT_FAILURE);
		}
	}

	
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(-1 == ioctl(fd,VIDIOC_STREAMON,&type))//开始采集图像
	{
		printf("i = %d.\n",i);
		perror("Fail to ioctl 'VIDIOC_STREAMON'");
		exit(EXIT_FAILURE);
	}

	return 0;
}

int process_image(unsigned char *addr,int length)
{
	
	printf("---------------------------------\n");
	CvMat mat = cvMat(640*480*2,1,CV_8UC1,addr);   //一个8位无符号整型单通道矩阵
	IplImage* img = NULL;//cvCreateImage( cvSize( 640, 480 ), 8, 3 );

	CvMemStorage* storage = cvCreateMemStorage(0);     //用来创建一个内存存储器，来统一管理各种动态对象的内存
	CvSeq* point_seq = cvCreateSeq( CV_32FC2, sizeof(CvSeq), sizeof(CvPoint2D32f), storage );
	float line[4]={0};  //输出的直线参数。2D 拟合情况下，它是包含 4 个浮点数的数组 (vx, vy, x0, y0)  
                       //其中 (vx, vy) 是线的单位向量而 (x0, y0) 是线上的某个点  
	float d,t;
	int x=0,y=0;
	unsigned int tr,tb,tg;	

	
	char sendtty[8];
	//char distc[2];
	int nsend;
	double v1x,v1y,sc,v2x,v2y,v3x,v3y;
	

	CvPoint2D32f point;


	int i=0,flag=1;
	
		
	img = cvDecodeImage(&mat,1);	//解压缩图像
	
	cvShowImage("WGW",img);			//显示图像
	cvWaitKey(30);
	

	
	for(y=0;y<480;y=y+2)
	{
		for(x=0;x<640;x=x+2)
		{
			tb = (unsigned int)(unsigned char)img ->imageData[(y*640+x)*3];
			tg = (unsigned int)(unsigned char)img ->imageData[(y*640+x)*3+1];
			tr = (unsigned int)(unsigned char)img ->imageData[(y*640+x)*3+2];

			if(( tb>tr+55) && (tb>tg+55) && (tr < 100)&& (tg<100))//b>r+55    b>g+55     r<100   g<100
			{
				
				point.x=x;
				point.y=y;
				cvSeqPush(point_seq, &point);

			}
		}
			
	}
	
	
	if(point_seq->total<15)
	{
		angle1=0;				
	}
	else
	{
		cvFitLine(point_seq,CV_DIST_L2,0,0.01,0.01,line);//拟合直线// 二维空间时： line[0--3] 分别为 (vx, vy,x0, y0)

//     其中 vx， vy 是正规化之后的斜率向量（单位向量）。 x0,y0 是直线经过的点。

		v1x = 320-line[2];  //  line 0.1向量，2.3点   640*480
		v1y = 240-line[3];                      
		sc = v1x*line[0] + v1y*line[1];			//指向图像中心的一个向量
		v2x = line[0] * sc;
		v2y = line[1] * sc;
		v3x = v1x - v2x;
		v3y = v1y - v2y;
		dist1 = sqrt(pow(v3x,2) + pow(v3y,2));
		if(v3x<0)
			dist1=dist1*(-1);
		
		angle1=(-1)*atan(line[0]/line[1])*180/3.14159265;

		dist=short(dist1*70);
		angle=short(angle1*300);
		printf("%f,%f\n",line[0],line[1]);

		printf("dist=%f,angle=%f\n",dist1,angle1);   
		
		
		
	}
	
	cvClearSeq(point_seq);
	cvReleaseMemStorage(&storage);
	cvReleaseImage(&img);
	cvReleaseData(&mat);

	return 0;
}

int read_frame(int fd)
{
	struct v4l2_buffer buf;
	unsigned int i;

	bzero(&buf,sizeof(buf));
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	
	//从队列中取缓冲区
	if(-1 == ioctl(fd,VIDIOC_DQBUF,&buf))
	{
		perror("Fail to ioctl 'VIDIOC_DQBUF'");
		exit(EXIT_FAILURE);
	}
	printf("buf_length=%d\n",buf.length);
	assert(buf.index < n_buffer);
	//读取进程空间的数据到一个文件中
	process_image((unsigned char *)user_buf[buf.index].start,user_buf[buf.index].length);
	
	if(-1 == ioctl(fd,VIDIOC_QBUF,&buf))
	{
		perror("Fail to ioctl 'VIDIOC_QBUF'");
		exit(EXIT_FAILURE);
	}

	return 1;
}

void stop_capturing(int fd)
{
	enum v4l2_buf_type type;
	
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(-1 == ioctl(fd,VIDIOC_STREAMOFF,&type))
	{
		perror("Fail to ioctl 'VIDIOC_STREAMOFF'");
		exit(EXIT_FAILURE);
	}
	
	return;
}

void uninit_camer_device()
{
	unsigned int i;

	for(i = 0;i < n_buffer;i ++)
	{
		if(-1 == munmap(user_buf[i].start,user_buf[i].length))
		{
			exit(EXIT_FAILURE);
		}
	}
	
	free(user_buf);

	return;
}

void close_camer_device(int fd)
{
	if(-1 == close(fd))
	{
		perror("Fail to close fd");
		exit(EXIT_FAILURE);
	}

	return;
}

int main(int argc , char ** argv)
{

	
    //-----camera init--------------
	int fd;//camera fd; 
	cvNamedWindow("WGW",0);   
                         
	fd = open_camer_device();
	printf("open camera success!\n");

	init_camer_device(fd);
	printf("init_camera success!\n");
	
	start_capturing(fd);
	printf("capture capture start!\n");
	
	// Initialize the ROS system and become a node .
 	ros::init ( argc , argv ,"publish_velocity" ) ;
 	ros::NodeHandle nh ;

 // Create a publisher obj ect .
 	ros::Publisher pub = nh.advertise <geometry_msgs::Twist>("/cmd_vel" , 1000 ) ;
	ros::Subscriber sub = nh.subscribe("/control",1000,&ctrlMessageReceived);
	

 // Loop at 2Hz until the node is shut down.
 	ros::Rate rate ( 30 ) ;


	//-----capture and send image
	while(ros::ok ( ))
	{
		read_frame(fd);
		
		geometry_msgs::Twist msg ;
		
 			msg.linear.x = 0.1 ;
 			//msg.angular.z = - angle * 0.004 + dist * 0.0012;
			msg.angular.z = -angle1*0.004;
            
 // Publish the message .
 		pub.publish ( msg ) ;
		ros::spinOnce();

 // Send a message to rosout with the details .
 		//ROS_INFO_STREAM( "Sending random velocity command : "<< "linear=" << msg.linear.x<< "angular=" << msg.angular.z) ;

 // Wait untilit's time for another iteration .
 		rate.sleep ( ) ;
	}
	
	//-----close camera-------
	stop_capturing(fd);
	printf("stop!\n");
	uninit_camer_device();
	close_camer_device(fd);
	cvDestroyAllWindows();
	
	return 0;
}





