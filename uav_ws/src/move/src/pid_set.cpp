#include"pid_set.h"
//void PID_set(float *pid_x.kp,float *pid_x.ki,float *pid_x.kp,float *pid_x.kp,float *pid_x.kp,float *pid_x.kp,float *pid_x.kp,float *pid_x.kp,float *pid_x.kp)
void PID_set(struct pid_x* p_x,struct pid_y* p_y,struct pid_h* p_h,struct pid_yaw* p_yaw)			//set x paras
{

	FILE *para;
    	para=fopen("/home/ubuntu/apriltags_ros/apriltags_ws/src/move/src/pidconfig.txt/pidconfig.txt","r");

	if(NULL==para)
	{
		printf("not find x_para!\n");
	}

	fscanf(para,"%f",&p_x->kp);		//x
	fscanf(para,"%f",&p_x->ki);
	fscanf(para,"%f",&p_x->kd);
	
	fscanf(para,"%f",&p_y->kp);		//y
	fscanf(para,"%f",&p_y->ki);
	fscanf(para,"%f",&p_y->kd);
	
	fscanf(para,"%f",&p_h->kp);		//h
	fscanf(para,"%f",&p_h->ki);
	fscanf(para,"%f",&p_h->kd);
	
	fscanf(para,"%f",&p_yaw->kp);	//yaw
	fscanf(para,"%f",&p_yaw->ki);
	fscanf(para,"%f",&p_yaw->kd);
	
	fclose(para);
	
	p_x->error=0.0;
	p_x->error_last=0.0;
	p_x->integral=0.0;
	p_x->v=0.0;
	
	p_y->error=0.0;
	p_y->error_last=0.0;
	p_y->integral=0.0;
	p_y->v=0.0;
	
	p_h->error=0.0;
	p_h->error_last=0.0;
	p_h->integral=0.0;
	p_h->v=0.0;
	
	p_yaw->error=0.0;
	p_yaw->error_last=0.0;
	p_yaw->integral=0.0;
	p_yaw->v=0.0;
	
}
