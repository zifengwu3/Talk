#include <inttypes.h>
#include <signal.h>
#include <semaphore.h>       //sem_t
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define COMMMAX 1024
#define COMMSENDMAX  20

#define _DEBUG
#define MAXCOMMSENDNUM  3
#define MAXSENDNUM  6
#define DELAYTIME  300

struct Local1 {
	pthread_mutex_t comm_lock; //������
	int Multi_Comm_Send_Run_Flag;
};
struct commbuf1 {
	int iput;
	int iget;
	int n;
	unsigned char buffer[COMMMAX];
};

struct Multi_Comm_Buff1 {
	int isValid;
	int SendNum;
	int m_Comm;
	unsigned char buf[1500];
	int nlength;
	int DelayTime;
};
#ifndef CommonH
#define CommonH
struct Local1 Local;
int Commfd;
int multi_comm_send_flag;
pthread_t multi_comm_send_thread;
void multi_comm_send_thread_func(void);
sem_t multi_comm_send_sem;
struct Multi_Comm_Buff1 Multi_Comm_Buff[COMMSENDMAX];
#else
extern struct Local1 Local;
extern int Commfd;
extern int multi_comm_send_flag;
extern pthread_t multi_comm_send_thread;
extern void multi_comm_send_thread_func(void);
extern sem_t multi_comm_send_sem;
extern struct Multi_Comm_Buff1 Multi_Comm_Buff[COMMSENDMAX];
#endif
