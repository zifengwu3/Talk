#include     <stdio.h>      
#include     <stdlib.h>     
#include     <unistd.h>     
#include     <fcntl.h>      
#include     <termios.h>    
#include     <errno.h>      

#include     <sys/types.h>  
#include     <sys/stat.h>   
#include     <sys/time.h>   

#include "log_jni.h"
#define CommonH
#include "common.h"

#include "comm.h"

#define FALSE  -1
#define TRUE   0
int Init_MultiCommThread(void); //初始化 Comm发送线程模块
int Uninit_MultiCommThread(void); //释放 Comm发送线程模块

int Commfd;
int OpenDev(char *Dev);
int set_speed(int fd, int speed);
int set_Parity(int fd, int databits, int stopbits, int parity);
int OpenComm(int CommPort, int BautSpeed, int databits, int stopbits,
		int parity);
int CommSendBuff(int fd, unsigned char buf[1024], int nlength);
void CloseComm();

short CommRecvFlag;
pthread_t comm_rcvid;
void CreateComm_RcvThread(int fd);
void Comm_RcvThread(int fd);
void print_haier_comm_data(char const *sbuf, int length);
/********************************************************************************/
int OpenDev(char *Dev) {
	int fd = open(Dev, O_RDWR);
	if (FALSE == fd) {
		LOGE("Can't Open Serial Port");
		return FALSE;
	} else {
		return fd;
	}
}

/**
 *@brief
 *@param  fd     
 *@param  speed 
 *@return  void
 */
int speed_arr[] = { B38400, B19200, B9600, B4800, B2400, B1200, B300, B38400,
		B19200, B9600, B4800, B2400, B1200, B300, };
int name_arr[] = { 38400, 19200, 9600, 4800, 2400, 1200, 300, 38400, 19200,
		9600, 4800, 2400, 1200, 300, };
int set_speed(int fd, int speed) {
	int i;
	int status;
	struct termios Opt;
	tcgetattr(fd, &Opt);

	for (i = 0; i < (sizeof(speed_arr) / sizeof(int)); i++) {
		if (speed == name_arr[i]) {
			tcflush(fd, TCIOFLUSH);
			cfsetispeed(&Opt, speed_arr[i]);
			cfsetospeed(&Opt, speed_arr[i]);
			status = tcsetattr(fd, TCSANOW, &Opt);

			if (status != 0) {
				LOGE("tcsetattr fd1");
				return FALSE;
			}
			tcflush(fd, TCIOFLUSH);
		}
	}
	return TRUE;
}

/**
 *@brief   
 *@param  fd     
 *@param  databits 
 *@param  stopbits
 *@param  parity
 */
int set_Parity(int fd, int databits, int stopbits, int parity) {
	struct termios options;

	if (tcgetattr(fd, &options) != 0) {
		LOGE("SetupSerial 1");
		return (FALSE);
	}

	options.c_cflag &= (~CSIZE);

	switch (databits) {
	case 7:
		options.c_cflag |= CS7;
		break;
	case 8:
		options.c_cflag |= CS8;
		break;
	default:
		LOGE("Unsupported data size");
		return (FALSE);
	}

	switch (parity) {
	case 'n':
	case 'N':
		options.c_cflag &= ~PARENB; /* Clear parity enable */
		options.c_iflag &= ~INPCK; /* Enable parity checking */
		break;
	case 'o':
	case 'O':
		options.c_cflag |= (PARODD | PARENB);
		options.c_iflag |= INPCK; /* Disnable parity checking */
		break;
	case 'e':
	case 'E':
		options.c_cflag |= PARENB; /* Enable parity */
		options.c_cflag &= ~PARODD;
		options.c_iflag |= INPCK; /* Disnable parity checking */
		break;
	case 'S':
	case 's': /*as no parity*/
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		break;
	default:
		LOGE(stderr, "Unsupported parity\n");
		return (FALSE);
	}

	switch (stopbits) {
	case 1:
		options.c_cflag &= ~CSTOPB;
		break;
	case 2:
		options.c_cflag |= CSTOPB;
		break;
	default:
		LOGE(stderr, "Unsupported stop bits\n");
		return (FALSE);
	}

	/* Set input parity option */
	if (parity != 'n') {
		options.c_iflag |= INPCK;
	}

	options.c_cflag |= CLOCAL | CREAD;
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	options.c_oflag &= ~OPOST;
	options.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

	tcflush(fd, TCIFLUSH);
	options.c_cc[VTIME] = 10;
	options.c_cc[VMIN] = 0;

	if (tcsetattr(fd, TCSANOW, &options) != 0) {
		LOGE("SetupSerial 3");
		return (FALSE);
	}

	return (TRUE);
}

int OpenComm(int CommPort, int BautSpeed, int databits, int stopbits,
		int parity) {
	char dev[50] = "/dev/ttyS";
	int Commfd;
	char serailport[50];

	sprintf(dev, "/dev/ttyS%d", CommPort);
	Commfd = OpenDev(dev);

	if (Commfd == FALSE) {
		return FALSE;
	}

	if (set_speed(Commfd, BautSpeed) == FALSE) {
		LOGE("Set Baut Error\n");
		return FALSE;
	}

	if (set_Parity(Commfd, databits, stopbits, parity) == FALSE) {
		LOGE("Set Parity Error\n");
		return FALSE;
	}

	/*
	 switch (CommPort)
	 {
	 case 7:
	 CreateComm_RcvThread(Commfd);
	 break;
	 default:
	 break;
	 }
	 */

	return Commfd;
}

#define COMM_DEBUG 0
void CreateComm_RcvThread(int fd) {

	int ret;
	CommRecvFlag = 1;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	ret = pthread_create(&comm_rcvid, &attr, (void *) Comm_RcvThread, (int *) fd);
	pthread_attr_destroy(&attr);

	LOGD("Create comm pthread!\n");
	if (ret != 0) {
		LOGD("Create comm pthread error!\n");
	}
}

void Comm_RcvThread(int fd) {

	struct timeval tv;
	uint32_t prev_comm_time;
	uint32_t nowtime;
	int len, validlen;
	int i;
	char buff[150];
	unsigned char validbuff[150];
	unsigned char m_crc;
	struct commbuf1 commbuf;

	LOGD("This is comm pthread.\n");
	//上一次串口数据接收时间
	gettimeofday(&tv, NULL);
	prev_comm_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	while (CommRecvFlag == 1) //循环读取数据
	{
		//系统初始化标志,如没有初始化完成则等待
		while ((len = read(fd, buff, 512)) > 0) {
			gettimeofday(&tv, NULL);
			nowtime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
			//与上一次接收超过50ms,则判断为超时
			if ((nowtime - prev_comm_time) >= 100) {
				commbuf.iget = 0;
				commbuf.iput = 0;
				commbuf.n = 0;
			}
			prev_comm_time = nowtime;

			LOGD("Len %d Comm\n", len);
			memcpy(commbuf.buffer + commbuf.iput, buff, len);
			commbuf.iput += len;
			if (commbuf.iput >= COMMMAX) {
				commbuf.iput = 0;
			}
			commbuf.n += len;
			while (commbuf.n >= 5) {
				if (commbuf.buffer[commbuf.iget] == CMD_SPI_QSA_HEAD) {
					validlen = commbuf.buffer[commbuf.iget + 1] + 3;
					if (commbuf.n >= validlen) {
						memcpy(validbuff, (commbuf.buffer + commbuf.iget),
								validlen);
						commbuf.iget += validlen;
						if (commbuf.iget >= COMMMAX) {
							commbuf.iget = 0;
						}

						commbuf.n -= validlen;
						if ((validbuff[0] == CMD_SPI_QSA_HEAD)) {
							LOGD("COMM: ");
							for (i = 0; i < (validbuff[1] + 3); i++) {
								printf("0x%02x ", validbuff[i]);
							}
							printf("\n");
							m_crc = 0;
							for (i = 0; i < (validlen - 1); i++) {
								m_crc += validbuff[i];
							}
							if (m_crc == validbuff[validlen - 1]) {
								send_data_to_a20(validbuff);
							} else {
								LOGD("crc error\n");
							}
						}
					} else {
						break;
					}
				} else {
					commbuf.iget++;
					if (commbuf.iget >= COMMMAX) {
						commbuf.iget = 0;
					}
					commbuf.n--;
				}
			}
		}
	}
}

void CloseComm() {
	CommRecvFlag = 0;
	usleep(40 * 1000);
	pthread_cancel(comm_rcvid);
	close(Commfd);
}

int CommSendBuff(int fd, unsigned char buf[1024], int nlength) {

	int nByte;
	int j;

	nByte = write(fd, buf, nlength);
#ifdef _DEBUG
	LOGD("[%d]Send:", nByte);
	for (j = 0; j < nlength; j++) {
		printf(" %02X", buf[j]);
	}
	printf("\n");
#endif
	return nByte;
}

int Init_MultiCommThread(void) //初始化 Comm发送线程模块
{
	int i;
	pthread_attr_t attr;
	//将COMM发送缓冲置为无效
	for (i = 0; i < COMMSENDMAX; i++) {
		Multi_Comm_Buff[i].isValid = 0;
		Multi_Comm_Buff[i].SendNum = 0;
		Multi_Comm_Buff[i].m_Comm = Commfd;
	}
	//主动命令数据发送线程：终端主动发送命令，如延时一段没收到回应，则多次发送
	//用于UDP和Comm通信
	sem_init(&multi_comm_send_sem, 0, 0);
	multi_comm_send_flag = 1;
	//创建互斥锁
	pthread_mutex_init(&Local.comm_lock, NULL);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&multi_comm_send_thread, &attr,
			(void *) multi_comm_send_thread_func, NULL);
	pthread_attr_destroy(&attr);
	if (multi_comm_send_thread == 0) {
		LOGD("Fail Create Comm Send Thread\n");
		return (-1);
	}
	return 0;
}

int Uninit_MultiCommThread(void) {
//释放 Comm发送线程模块
	multi_comm_send_flag = 0;
	usleep(40 * 1000);
	pthread_cancel(multi_comm_send_thread);
	sem_destroy(&multi_comm_send_sem);
	pthread_mutex_destroy(&Local.comm_lock); //删除互斥锁
	return 0;
}

void multi_comm_send_thread_func(void) {
	int i;
	int HaveDataSend;
#ifdef _DEBUG
	LOGD("Success Create Comm Send Thread\n");
#endif
	while (multi_comm_send_flag == 1) {
		sem_wait(&multi_comm_send_sem);
		if (Local.Multi_Comm_Send_Run_Flag == 0) {
			Local.Multi_Comm_Send_Run_Flag = 1;
		} else {
			HaveDataSend = 1;
			while (HaveDataSend) {
				for (i = 0; i < COMMSENDMAX; i++) {
					if (Multi_Comm_Buff[i].isValid == 1) {
						if (Multi_Comm_Buff[i].SendNum < MAXSENDNUM) {
							if (Multi_Comm_Buff[i].isValid == 1) {
								//COMM发送
								CommSendBuff(Multi_Comm_Buff[i].m_Comm,
										Multi_Comm_Buff[i].buf,
										Multi_Comm_Buff[i].nlength);

								if (Multi_Comm_Buff[i].SendNum > 0) {
									usleep((Multi_Comm_Buff[i].DelayTime + 100) * 1000);
								}
							}
							Multi_Comm_Buff[i].SendNum++;
							break;
						}
					}
				}

				if ((Multi_Comm_Buff[i].isValid == 1)
						&& (Multi_Comm_Buff[i].SendNum >= MAXSENDNUM)) {
					//锁定互斥锁
					pthread_mutex_lock(&Local.comm_lock);
					switch (Multi_Comm_Buff[i].buf[0]) {
					case CMD_SPI_QSA_HEAD: {
						Multi_Comm_Buff[i].isValid = 0;
						LOGD(" COMM Send Fail, 0x%02X:: 0x%02X\n", Multi_Comm_Buff[i].buf[0], Multi_Comm_Buff[i].buf[2]);
					}
						break;
					default: //为其它命令，本次通信结束
					{
						Multi_Comm_Buff[i].isValid = 0;
						LOGD("COMM Send Fail: 0x%02X\n", Multi_Comm_Buff[i].buf[0]);
					}
						break;
					}
					//打开互斥锁
					pthread_mutex_unlock(&Local.comm_lock);
				}

				//判断数据是否全部发送完，若是，线程终止
				HaveDataSend = 0;
				for (i = 0; i < COMMSENDMAX; i++) {
					if (Multi_Comm_Buff[i].isValid == 1) {
						HaveDataSend = 1;
						break;
					}
				}
			}
		}
	}
}

