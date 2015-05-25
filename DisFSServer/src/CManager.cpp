/*
 * CManager.cpp
 *
 *  Created on: Nov 21, 2013
 *      Author: Shi Yihui
 */
#include <stdio.h>
#include <unistd.h>

extern "C"{
#include  "unp.h"
}

#include "CManager.h"
#include "message.h"
#include "CParameter.h"

extern bool g_quit;
extern CParameter g_para;

CManager::CManager(vector<CWorker*> worker_list):
		m_list(worker_list)
{
	snprintf(m_name, 32, "mn-%03d", 0);
}

CManager::~CManager() {
}

CWorker* CManager::reserve_free_writer(){
	CWorker* result = NULL;

	vector<CWorker*>::iterator iter;
	for(iter=m_list.begin();iter!=m_list.end();iter++){
		if((*iter)->m_free){
			(*iter)->m_free = false;
			result = *iter;
			break;
		}
	}

	return result;
}

void CManager::notice_quit(){
	vector<CWorker*>::iterator iter;
	g_quit = true;
	for(iter=m_list.begin();iter!=m_list.end();iter++){
		sem_post(&((*iter)->m_work_sem));
	}

	return ;
}

int accept_timeout(int fd, struct sockaddr_in *addr, unsigned int wait_seconds)
{
	int ret;
	socklen_t addrlen = sizeof(struct sockaddr_in);


	if (wait_seconds > 0)
	{
		fd_set accept_fdset;
		struct timeval timeout;
		FD_ZERO(&accept_fdset);
		FD_SET(fd, &accept_fdset);
		timeout.tv_sec = wait_seconds;
		timeout.tv_usec = 0;
		do
		{
			ret = select(fd + 1, &accept_fdset, NULL, NULL, &timeout);
		} while (ret < 0 && errno == EINTR);
		if (ret == -1)
			return -1;
		else if (ret == 0)
		{
			errno = ETIMEDOUT;
			return -1;
		}
	}


	if (addr != NULL)
		ret = accept(fd, (struct sockaddr*)addr, &addrlen);
	else
		ret = accept(fd, NULL, NULL);

	return ret;
}


void CManager::run() {
	int     listenfd, connfd;
	struct sockaddr_in cliaddr, servaddr;

	listenfd = Socket (AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
	servaddr.sin_port = htons (g_para.server_port);

	int on=1;
	if((setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)
	{
	    printf("%s: setsockopt failed\n",m_name);
	    return;
	}

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	for ( ; ; )  {
	    connfd = accept_timeout(listenfd, &cliaddr, 2);

	    if(connfd < 0){
			if(g_quit)
			{
				notice_quit();
				break;
			}
			continue;
	    }

	    //reserver new worker
		CWorker* worker = reserve_free_writer();
		if(worker!=NULL){
			printf("%s: receive task, notice worker, fd %d\n",m_name, connfd);
			worker->m_connfd = connfd;
			sem_post(&(worker->m_work_sem));
		}
		else{
			printf("%s: no free worker, reject\n",m_name);
	    	reply_error_msg(connfd, -6, 0, 0);
			Close(connfd);          /* manager closes connected socket */
		}
	}
	printf("%s: close listenfd\n",m_name);
	Close(listenfd);
	printf("%s: quit\n",m_name);

}
