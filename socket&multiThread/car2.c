#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <assert.h>
#include <linux/videodev2.h>


#include <netinet/in.h>    
#include <sys/types.h>    
#include <sys/socket.h> 
#define HELLO_WORLD_SERVER_PORT    4000
#define HELLO_WORLD_SERVER_PORT2    5000
#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 1024
#define INT16 short
#include <cv.h>
#include <highgui.h>
#include <pthread.h>
#include <math.h>



#include <unistd.h>
#include <termios.h>

int ttyfd;
typedef struct
{
	void *start;
	int length;
}BUFTYPE;

BUFTYPE *user_buf;
int n_buffer = 0;
int img_length=0;
char img_len[5];
char tcprecvbuf='g';

int cflag=0;
int controlflag=1;
int mainflag=1;
unsigned char imgbuffer[640*480*2];

pthread_mutex_t mylock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mylock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t mylock_ready = PTHREAD_COND_INITIALIZER;


int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
	struct termios newtio,oldtio;
	if ( tcgetattr (fd,&oldtio) != 0)//获取终端参数，读取串口的参数设置
									//该函数与设备文件绑定，开始必须要调用
	{
		perror ("SetupSerial 1");
		return -1;
	}
	bzero( &newtio, sizeof( newtio ) );
	newtio.c_cflag |= CLOCAL | CREAD;//一般必设置的标志，本地连接本地接收
	newtio.c_cflag &= ~CSIZE;					 //屏蔽字符大小位
	switch( nBits )								 //设置数据位数
	{
		case 7:
		newtio.c_cflag |= CS7;
		break;
		case 8:
		newtio.c_cflag |= CS8;
		break;
	}
	switch( nEvent )							//设置校验位
	{
		case 'O':
		newtio.c_cflag |= PARENB;				//进行奇偶校验
		newtio.c_cflag |= PARODD;				//奇校验，否则为偶
		newtio.c_iflag |= (INPCK | ISTRIP);		//打开输入奇偶校验
												//剥除输入字符的第八位
		break;
		case 'E':
		newtio.c_iflag |= (INPCK | ISTRIP);
		newtio.c_cflag |= PARENB;				//进行奇偶校验
		newtio.c_cflag &= ~PARODD;				//偶校验
		break;
		case 'N':
		newtio.c_cflag &= ~PARENB;				//不进行奇偶校验
		break;
	}
	switch( nSpeed )							//波特率设置
	{
	case 2400:
		cfsetispeed (& newtio, B2400);
		cfsetospeed (&newtio, B2400);
		break;
	case 4800:
		cfsetispeed (& newtio, B4800);
		cfsetospeed (&newtio, B4800);
		break;
	case 9600:
		cfsetispeed (& newtio, B9600);
		cfsetospeed (&newtio, B9600);
		break;
	case 115200:
		cfsetispeed (&newtio, B115200 );
		cfsetospeed (&newtio, B115200);
		break;
	default:
		cfsetispeed (& newtio, B9600);
		cfsetospeed (&newtio, B9600);
		break;
	}
	if ( nStop == 1 )
		newtio.c_cflag &= ~CSTOPB;      //一个停止位
	else if ( nStop == 2 )
		newtio.c_cflag |= CSTOPB ;		//两个停止位
	newtio.c_cc [VTIME] = 0;
	newtio.c_cc [VMIN] = 1;
	tcflush (fd ,TCIFLUSH);
	if ((tcsetattr(fd,TCSANOW,&newtio))!=0)			
								//该函数与设备文件绑定，最后必须要调用
	{
		perror ("com set error");
		return -1;
	}
	printf("set done!\n");
	return 0 ;
}

int open_port( int fd,int comport)
{
	char *dev[]={ "/dev/ttyS0","/dev/ttyS1","/dev/ttyS2" };
	long vdisable;
	if (comport==1)
	{ 
		fd = open ( "/dev/ttyUSB0", O_RDWR|O_NOCTTY);
		if (-1 == fd )
		{
			perror ("Can't Open Serial Port");
			return(-1);
		}
		else
			printf("open ttyUSB0 .....\n" );
	}
	else if (comport==2)
	{ 	
		fd = open ( "/dev/ttyS1", O_RDWR|O_NOCTTY|O_NDELAY);
		if (-1 == fd )
		{
			perror ("Can't Open Serial Port");
			return(-1);
		}
		else
			printf("open ttyS1 .....\n" );
	}
	else if ( comport==3)
	{
		fd = open( "/dev/ttyS2", O_RDWR|O_NOCTTY|O_NDELAY);
		if (-1 == fd ){
			perror ("Can't Open Serial Port");
			return(-1);
		}
		else
			printf("open ttyS2 .....\n" );
	}
	else if ( comport==4)
	{
		fd = open( "/dev/tty03", O_RDWR|O_NOCTTY|O_NDELAY);
		if (-1 == fd ){
			perror ("Can't Open Serial Port");
			return(-1);
		}
		else
			printf("open ttyS3 .....\n" );
	}
	if (fcntl(fd , F_SETFL, 0)<0)
		printf("fcntl failed!\n" );
	else
		printf("fcntl=%d\n", fcntl(fd , F_SETFL,0));
	if (isatty (STDIN_FILENO)==0)
		printf("standard input is not a terminal device\n");
	else
		printf("isatty success!\n");
	printf("fd-open=%d\n", fd);
	return fd ;
}








void *commurecv(void *client_socket)
{
	int length_recv = 0;
	for(;;)
	{		
		length_recv=recv((int)client_socket,&tcprecvbuf,1,0);
		
		if(length_recv==1)
		{
			printf("I have receive something\n");
		
		}					
	}		
}



int open_camer_device()
{
	int fd;

	if((fd = open("/dev/video0",O_RDWR | O_NONBLOCK)) < 0)
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

	user_buf = calloc(reqbuf.count,sizeof(*user_buf));
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

	CvMemStorage* storage = cvCreateMemStorage(0);     //用来创建一个内存存储器，来统一管理??侄韵蟮哪诖?
	CvSeq* point_seq = cvCreateSeq( CV_32FC2, sizeof(CvSeq), sizeof(CvPoint2D32f), storage );
	float line[4]={0};  //输出的直线参数。2D 拟合情况下，它是包含 4 个浮点数的数组 (vx, vy, x0, y0)  
                       //其中 (vx, vy) 是线的单位向量而 (x0, y0) 是线上的某个点  
	float d,t;
	int x=0,y=0;
	unsigned int tr,tb,tg;	

	int angle,dist;
	char sendtty[8];
	//char distc[2];
	int nsend;
	double v1x,v1y,sc,v2x,v2y,v3x,v3y;
	double dist1,angle1;

	CvPoint2D32f point;


	int i=0,flag=1;
	
		
	img = cvDecodeImage(&mat,1);	//解压缩图像
	
	for(y=0;y<480;y=y+2)
	{
		for(x=0;x<640;x=x+2)
		{
			tb = (unsigned int)(unsigned char)img ->imageData[(y*640+x)*3];
			tg = (unsigned int)(unsigned char)img ->imageData[(y*640+x)*3+1];
			tr = (unsigned int)(unsigned char)img ->imageData[(y*640+x)*3+2];

			if(( tb>tr+45) && (tb>tg+45) && (tr < 150)&& (tg<150))//红>蓝+55    红>绿+55     b<100   g<100
			{
				
				point.x=x;
				point.y=y;
				cvSeqPush(point_seq, &point);

			}
			
				
		}	
	}


	if(point_seq->total<15)
	{
			
			sendtty[0]='a';
			sendtty[1]='b';
			sendtty[2]='s';
			sendtty[3]='0';
			sendtty[4]='0';
			sendtty[5]='0';
			sendtty[6]='0';
			sendtty[7]=sendtty[2]+sendtty[3]+sendtty[4]+sendtty[5]+sendtty[6];
						
	}
	else
	{
		cvFitLine(point_seq,CV_DIST_L2,0,0.01,0.01,line);//拟合直线

		v1x = 320-line[2];  //  line 0.1向量，2.3点   640*480
		v1y = 240-line[3];
		sc = v1x*line[0] + v1y*line[1];
		v2x = line[0] * sc;
		v2y = line[1] * sc;
		v3x = v1x - v2x;
		v3y = v1y - v2y;
		dist1 = sqrt(pow(v3x,2) + pow(v3y,2));
		if(v3x<0)
			dist1=dist1*(-1);
		
		angle1=(-1)*atan(line[0]/line[1])*180/3.14159265;

		dist=(INT16)(dist1*70);
		angle=(INT16)(angle1*300);

		sendtty[0]='a';
		sendtty[1]='b';
		sendtty[2]='g';
		sendtty[3]=(char)(angle>>8);
		sendtty[4]=(char)((angle<<8)>>8);
		sendtty[5]=(char)(dist>>8);
		sendtty[6]=(char)((dist<<8)>>8);
		sendtty[7]=sendtty[2]+sendtty[3]+sendtty[4]+sendtty[5]+sendtty[6];
		
		printf("dist=%f,angle=%f\n",dist1,angle1);
		
		
	}
	//pthread_mutex_lock(&mylock2);
	if(tcprecvbuf=='g' && (sendtty[2]=='g'))
	{
		controlflag=1;
		sendtty[2]='g';
		sendtty[7]=sendtty[2]+sendtty[3]+sendtty[4]+sendtty[5]+sendtty[6];
	}
	else if(tcprecvbuf=='q')
	{
		sendtty[2]='s';
		sendtty[7]=sendtty[2]+sendtty[3]+sendtty[4]+sendtty[5]+sendtty[6];
		mainflag=0;
		
	}
	else if((tcprecvbuf=='s')||controlflag==0)
	{
		sendtty[2]='s';
		sendtty[7]=sendtty[2]+sendtty[3]+sendtty[4]+sendtty[5]+sendtty[6];
		controlflag=0;
	}
	
	//pthread_mutex_unlock(&mylock2);

	nsend=write(ttyfd,sendtty,sizeof(sendtty));
	printf("%c\n",sendtty[2]);
	
	
	
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

int mainloop(int fd)
{ 
	int count = 0;
	int nsend;
        //int flag = 1;
	char sendtty[8];
 
	struct timeval cur_tv;
	long long int start_time ,end_time;
	gettimeofday(&cur_tv, NULL);
	start_time=1000000*cur_tv.tv_sec+cur_tv.tv_usec;

	while( mainflag)
	{
		
		for(;;)
		{
			fd_set fds;
			struct timeval tv;
			int r;

			FD_ZERO(&fds);
			FD_SET(fd,&fds);

			//Timeout
			tv.tv_sec = 2;
			tv.tv_usec = 0;
		
			r = select(fd + 1,&fds,NULL,NULL,&tv);

			if(-1 == r)
			{
				if(EINTR == errno)
					continue;
				
				perror("Fail to select");
				exit(EXIT_FAILURE);
			}

			if(0 == r)
			{
				fprintf(stderr,"select Timeout\n");
				exit(EXIT_FAILURE);
			}

			if(read_frame(fd))

				break;
		}
		printf("count=%d\n",count);
		count++;
		
	
	}
	sendtty[0]='a';
	sendtty[1]='b';
	sendtty[2]='s';
	sendtty[3]='0';
	sendtty[4]='0';
	sendtty[5]='0';
	sendtty[6]='0';
	sendtty[7]=sendtty[2]+sendtty[3]+sendtty[4]+sendtty[5]+sendtty[6];
	nsend=write(ttyfd,sendtty,sizeof(sendtty));
	gettimeofday(&cur_tv, NULL);
	end_time=1000000*cur_tv.tv_sec+cur_tv.tv_usec;
	int time1 = (int)((end_time-start_time)/1000);  //计时
	printf("time is %d\n",time1);
	return 0;
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



//----------------------------------------------------------------------------
//---------------------------main--------------------------------------------
int main()
{
      
      
	int opt = 1;
      	int err = 0;
	int i;
     	pthread_t tfd,tfd2;

	//tty init
	if ((ttyfd=open_port(ttyfd,1))< 0)
	{
		perror ("open_port error");
		return 0;
	}
	if ((i =set_opt(ttyfd,115200,8,'N',1))<0)
	{
		perror ("set_opt error");
		return 0;
	}

	//--------socket init--------------
	
	struct sockaddr_in server_addr2;// sockaddr_in，存放Internet socket地址信息
	int server_socket2;
	struct sockaddr_in server_addr;
	int server_socket;

	bzero(&server_addr,sizeof(server_addr)); 
    	server_addr.sin_family = AF_INET;//sin_family指代协议族,AF_INET(IPV4)
										 //在socket编程中只能是AF_INET
   	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    	server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);
  
	
	bzero(&server_addr2,sizeof(server_addr)); 
   	server_addr2.sin_family = AF_INET;
   	server_addr2.sin_addr.s_addr = htons(INADDR_ANY);
    	server_addr2.sin_port = htons(HELLO_WORLD_SERVER_PORT2);

 

   	/* client socket init*/
	struct sockaddr_in client_addr2;
	int client_socket2;

	 socklen_t length;
    //char buffer[BUFFER_SIZE];
    length = sizeof(client_addr2);
    
   	//server2
	server_socket2 = socket(PF_INET,SOCK_STREAM,0);
    if( server_socket2 < 0)
    {
        printf("Create Socket Failed!");
        exit(1);
    }
 
    /* bind socket to a specified address*/
    setsockopt(server_socket2, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	//在套接字级别上设置选项,打开或关闭地址复用功能
    if( bind(server_socket2,(struct sockaddr*)&server_addr2,sizeof(server_addr2)))
    {
        printf("Server Bind Port : %d Failed!", HELLO_WORLD_SERVER_PORT); 
        exit(1);
    }

    /* listen a socket */
    if(listen(server_socket2, LENGTH_OF_LISTEN_QUEUE))
    {
        printf("Server Listen Failed!"); 
        exit(1);
    }
    printf("server2 listening!\n");

    
    /* accept socket from client */

    while (1) 
    {
	client_socket2 = accept(server_socket2, (struct sockaddr*)&client_addr2, &length);
        if( client_socket2 < 0)
        {
            printf("Server Accept Failed!\n");
        }
	else
        {
            printf("Server Accept Success!\n");
		break;
        }		
    }

	


	//-----camera init--------------
	int fd;//camera fd;       
                         
	fd = open_camer_device();
	printf("open camera success!\n");

	init_camer_device(fd);
	printf("init_camera success!\n");
	
	start_capturing(fd);
	printf("capture capture start!\n");
	//new_thread

	//----------------------------------
	
	while(1)
	{
		recv((int)client_socket2,&tcprecvbuf,1,0);
		if(tcprecvbuf=='g')
			break;
	}

	
	
	
	err=pthread_create(&tfd2,NULL,commurecv,(void *)client_socket2);//线程标识符；线程属性设置；线程函数的起始地址；传递给前述地址的参数；
	if(err!= 0)
	{
		printf("new thread error\n");
		exit(1);
	}

	//-----capture and send image
	mainloop(fd);
	

	

	//-----close camera-------
	stop_capturing(fd);


	pthread_cancel(tfd2);

	printf("stop!\n");
	uninit_camer_device(fd);
	close_camer_device(fd);
	//----close tty-----------
	close(ttyfd);

	//----close server---------

	close(client_socket2);
	close(server_socket2);
	return 0;
}
