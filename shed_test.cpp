/* 
 * Copyright (C) 2009 Chris Simmonds (chris@2net.co.uk)
 *
 * This is a demonstration of periodic threads using the Linux timerfd
 * interface which was introduced in GNU libc 2.8 and kernel 2.6.25.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/timerfd.h>
#include <time.h>
#include <iostream>
#include <fstream>  
using namespace std;

// 89000000 is 1 sec:
#define N_CPU_BURN 87000000*2 

pthread_mutex_t lock;
pthread_t t_0;
pthread_t t_1;
pthread_t t_2;
pthread_t t_3;

struct periodic_info {
	int timer_fd;
	unsigned long long wakeups_missed;
};


static int make_periodic(unsigned int period, struct periodic_info *info)
{
	int ret;
	unsigned int ns;
	unsigned int sec;
	int fd;
	struct itimerspec itval;

	/* Create the timer */
	fd = timerfd_create(CLOCK_MONOTONIC, 0);
	info->wakeups_missed = 0;
	info->timer_fd = fd;
	if (fd == -1)
		return fd;

	/* Make the timer periodic */
	sec = period / 1000000;
	ns = (period - (sec * 1000000)) * 1000;
	itval.it_interval.tv_sec = sec;
	itval.it_interval.tv_nsec = ns;
	itval.it_value.tv_sec = sec;
	itval.it_value.tv_nsec = ns;
	ret = timerfd_settime(fd, 0, &itval, NULL);
	return ret;
}


static void wait_period(struct periodic_info *info)
{
	unsigned long long missed;
	int ret;

	/* Wait for the next timer event. If we have missed any the
	   number is written to "missed" */
	ret = read(info->timer_fd, &missed, sizeof(missed));
	if (ret == -1) {
		perror("read timer");
		return;
	}

	info->wakeups_missed += missed;
}


static void CPUburn(unsigned long n)
{
	for (unsigned long i = 0; i<= n;i++) 
	{
	  // do nothing  
	}
}

void work(int time_units, int task_no, char output) 
{ 
  static unsigned int globalTime = 0;
  for(int j = 0; j < time_units; j++)
  {
		printf("%3d: ",globalTime);
		globalTime++;
		for(int j = 0; j < task_no ; j++) printf("\t");
		printf("%d%c\n",task_no,output);
		fflush(stdout);
		CPUburn(N_CPU_BURN);
  }
}

static void* thread_0(void *vptr)
{
	// Initialize thread.
	int thread_no = *(int*)vptr;	
	struct periodic_info info;
	make_periodic(20000000, &info);

	// Do periodic work.
	while(1)
 	{
  	work(1,thread_no,' ');
		pthread_mutex_lock(&lock);
  	work(3,thread_no,'R');
		pthread_mutex_unlock(&lock);
		wait_period(&info);
	}

 	return NULL;
}

static void *thread_1(void *vptr)
{
	// Initialize thread.
	int thread_no = *(int*)vptr;	
	struct periodic_info info;
	make_periodic(20000000, &info);

	// Do periodic work.
	while(1)
 	{
		sleep(4);
  	work(2,thread_no,' ');
		wait_period(&info);
	}

 	return NULL;
}

static void *thread_2(void *vptr)
{
	// Initialize thread.
	int thread_no = *(int*)vptr;	
	struct periodic_info info;
	make_periodic(20000000, &info);

	// Do periodic work.
	while(1)
 	{
		sleep(3);
  	work(3,thread_no,' ');
		pthread_mutex_lock(&lock);
  	work(1,thread_no,'R');
		pthread_mutex_unlock(&lock);
  	work(1,thread_no,' ');
		wait_period(&info);
	}

 	return NULL;
}

static void *thread_3(void *vptr)
{
	// Initialize thread.
	int thread_no = *(int*)vptr;	
	struct periodic_info info;
	make_periodic(20000000, &info);

	// Do periodic work.
	while(1)
 	{
		sleep(7);
  	work(2,thread_no,' ');
		wait_period(&info);
	}

 	return NULL;
}



int main(int argc, char *argv[])
{

// work calibration of N_CPU_BURN to 1 sec.:
// int s,e;s=clock();work(1,0,'C');e=clock();cout<<endl<<e-s<<"us"<<endl;	
	
	// Scheduling parameters
	struct sched_param params_t0;
	params_t0.sched_priority = 19;

	struct sched_param params_t1;
	params_t1.sched_priority = 23;

	struct sched_param params_t2;
	params_t2.sched_priority = 24;
	
	struct sched_param params_t3;
	params_t3.sched_priority = 25;
	
	
	pthread_attr_t attr_t0;
	pthread_attr_t attr_t1;
	pthread_attr_t attr_t2;
	pthread_attr_t attr_t3;
		
	pthread_attr_init(&attr_t0);
	pthread_attr_init(&attr_t1);
	pthread_attr_init(&attr_t2);
	pthread_attr_init(&attr_t3);

	pthread_attr_setinheritsched(&attr_t0, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setinheritsched(&attr_t1, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setinheritsched(&attr_t2, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setinheritsched(&attr_t3, PTHREAD_EXPLICIT_SCHED);
	
	pthread_attr_setschedpolicy(&attr_t0, SCHED_FIFO);
	pthread_attr_setschedpolicy(&attr_t1, SCHED_FIFO);
	pthread_attr_setschedpolicy(&attr_t2, SCHED_FIFO);
	pthread_attr_setschedpolicy(&attr_t3, SCHED_FIFO);

	pthread_attr_setschedparam(&attr_t0, &params_t0);
	pthread_attr_setschedparam(&attr_t1, &params_t1);
	pthread_attr_setschedparam(&attr_t2, &params_t2);
	pthread_attr_setschedparam(&attr_t3, &params_t3);	

	// Initialize mutex.
	pthread_mutex_init(&lock, NULL);

	//Create threads with scheduling parameters
	int ids[] = { 0, 1, 2, 3 };

	pthread_create(&t_3, &attr_t3, thread_3, (void*)&ids[3]);
	pthread_create(&t_2, &attr_t2, thread_2, (void*)&ids[2]);
	pthread_create(&t_1, &attr_t1, thread_1, (void*)&ids[1]);
	pthread_create(&t_0, &attr_t0, thread_0, (void*)&ids[0]);

	pthread_join(t_0,NULL);
	pthread_join(t_1,NULL);
	pthread_join(t_2,NULL);
	pthread_join(t_3,NULL);

	sleep(200);
	return 0;
}
