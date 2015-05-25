/*
 * message.cpp
 *
 *  Created on: Nov 22, 2013
 *      Author: Shi Yihui
 */

#include <string.h>
#include <stdio.h>

#include "message.h"

extern "C"{
#include  "unp.h"
}


int get_head_in_reqmsg(char* buff, request_msg_t* msg){
	int n;
	if(strncmp(buff, "READ ", 5)==0)
	{
		msg->method = READ;
		n = 5;
	}
	else if(strncmp(buff, "NEW_TXN ", 8)==0)
	{
		msg->method = NEW_TXN;
		n = 8;
	}
	else if(strncmp(buff, "WRITE ", 6)==0)
	{
		msg->method = WRITE;
		n = 6;
	}
	else if(strncmp(buff, "COMMIT ", 7)==0)
	{
		msg->method = COMMIT;
		n = 7;
	}
	else if(strncmp(buff, "ABORT ", 6)==0)
	{
		msg->method = ABORT;
		n = 6;
	}
	else
	{
		return -2;
	}

	if(sscanf(buff+n,"%d %d %d\n",
			&msg->transaction_id,
			&msg->seq_num,
			&msg->content_length) != 3)
	{
		return -3;
	}

	return n;
}

// >0 message length
// =0 time out
// -1 cannot find message head split
// -2 message method invalid
// -3 message transaction id, seq, content length invalid
// -4 message too long
// -5 message length mismatch with head
// -255 socket closed by client
int get_next_message(int sockfd, request_msg_t* msg, int length, char* name){

	ssize_t		n;
	int 		curr_length = 0;
	int			read_buff_length = 0;
	int 		body_begin;
	char		buff[MAXLINE];
	bool		head_ready = false;
	bool		has_body = false;
	int			time_out_counter = 0;
    fd_set  	rset;
    struct timeval timeout;

	timeout.tv_sec = 0;
    timeout.tv_usec = 400000;

    FD_ZERO(&rset);
	time_out_counter = 0;
	n = 0;
    for ( ; ; )  {
        FD_SET(sockfd, &rset);
        Select(sockfd+1, &rset,  NULL,  NULL,  &timeout);

        if (FD_ISSET(sockfd,  &rset))  {  /* socket is readable */
        	if ( ( n = read(sockfd, buff+curr_length, MAXLINE-curr_length)) > 0){
        		time_out_counter = 0;
        		curr_length += n;
//        		printf("%s: ###read %d bytes\n",name, n);
        	}
        	else{
//        		printf("%s: ###read EOF\n",name);
        		return -255;
        	}
        }
        else{
//    		printf("%s: ###select time out 1\n",name);
    		if(curr_length!=0)
    		{
    			n = curr_length;
    			break;
    		}
//    		printf("%s: ###select time out 2\n",name);
    		usleep(50000);
    		time_out_counter++;
    		if(time_out_counter > 40){
    			return 0;
    		}
        }
    }

	read_buff_length = curr_length;

	head_ready = false;
	has_body = false;
	for(int i=0;i<read_buff_length-6;i++)
	{
		if(
			buff[i]== 0xD
			&&buff[i+1]== 0xA
			&&buff[i+2]== 0xD
			&&buff[i+3]== 0xA
			&&buff[i+4]== 0xD
			&&buff[i+5]== 0xA
		)
		{
			body_begin = i + 6;
			buff[i] = '\n';
			buff[i+1] = '\0';
			head_ready = true;
			has_body = false;
			break;
		}
	}

	if( head_ready != true ){
		for(int i=0;i<read_buff_length-4;i++)
		{
			if(
				buff[i]== 0xD
				&&buff[i+1]== 0xA
				&&buff[i+2]== 0xD
				&&buff[i+3]== 0xA
			)
			{
				body_begin = i + 4;
				buff[i] = '\n';
				buff[i+1] = '\0';
				head_ready = true;
				has_body = true;
				break;
			}
		}
	}

	if(!head_ready)
	{
		return -1;
	}

	if(head_ready){
		int n;
		n = get_head_in_reqmsg(buff, msg);
		if(n<0)
			return n;
	}

	if(!has_body)
		msg->content_length = 0;

	if(has_body){
		if((unsigned int)msg->content_length > (length - sizeof(request_msg_t)) )
		{
			printf("-4, %d,%d %d\n",msg->content_length, length, (int)sizeof(request_msg_t) );
			return -4;
		}
		memcpy(msg->content, buff+body_begin, msg->content_length);
	}

	return msg->content_length+sizeof(request_msg_t);
}

typedef struct{
	int input;
	int error_code;
	char err_string[64];
}err_string_t;

static err_string_t s_err_string[]={
	{ -1 ,  204, "Cannot find valid head split.\n",                    },
	{ -2 ,  204, "Method invalid.\n",                                  },
	{ -3 ,  204, "Number in head format incorrect.\n",                 },
	{ -4 ,  204, "Message too long.\n",                                },
	{ -5 ,  204, "Message length mismatch between body and head.\n",   },
	{ 201,  201, "Invalid transaction id.\n",                          },
	{ 202,  202, "Invalid operation.\n",                               },
	{ 204,  204, "Wrong message format.\n",                            },
	{ 205,  205, "File i o error.\n",                                  },
	{ 206,  206, "File not found.\n",                                  },
	{ -6 ,  207, "Server too busy.\n",                                 },
	{ -7 ,  208, "Conflict with other client.\n",                      },
	{ -8 ,  209, "Sequence number conflict with previous request.\n",  },
	{ -9 ,  210, "Client has no message for too long time.\n",         },
	{ -10,  211, "Invalid sequence number in write operation.\n",      }
};

void reply_ack_msg(int sockfd, int transaction_id){
	char buf[1024];
	snprintf(buf, 1024, "%s %d %d %d %d \r\n\r\n\r\n",
		"ACK",
		transaction_id,
		0,
		0,
		0);

	writen(sockfd, buf, strlen(buf));
}

void reply_resend_msg(int sockfd, int transaction_id, int seq){
	char buf[1024];
	snprintf(buf, 1024, "%s %d %d %d %d \r\n\r\n\r\n",
		"ASK_RESEND",
		transaction_id,
		seq,
		0,
		0);

	writen(sockfd, buf, strlen(buf));
}


void reply_error_msg(int sockfd, int error, int transaction_id, int seq){
	char	buf[1024];
	int 	i;
	int 	size = (sizeof(s_err_string))/(sizeof(err_string_t));

	for(i=0;i<size;i++){
		if(s_err_string[i].input == error)
			break;
	}

	if(i>=size){
		printf("cannot find corresponding error string\n");
		return;
	}

	snprintf(buf, 1024, "%s %d %d %d %d \r\n\r\n%s",
		"ERROR",
		transaction_id,
		seq,
		s_err_string[i].error_code,
		(int)strlen(s_err_string[i].err_string),
		s_err_string[i].err_string);

	writen(sockfd, buf, strlen(buf));
}

