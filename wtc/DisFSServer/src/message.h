/*
 * message.h
 *
 *  Created on: Nov 22, 2013
 *      Author: Shi Yihui
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

enum method_e{
	READ,
	NEW_TXN,
	WRITE,
	COMMIT,
	ABORT,
	ACK,
	ASK_RESEND,
	ERROR
};

typedef struct {
	enum method_e method;
	int transaction_id;
	int seq_num;
	int content_length;
	char content[0];
}request_msg_t;

typedef struct {
	enum method_e method;
	int transaction_id;
	int error;
	int reason_length;
	char reason[0];
}response_msg_t;

void reply_error_msg(int sockfd, int error, int transaction_id, int seq);
int  get_next_message(int sockfd, request_msg_t* msg, int length, char* worker_name);
void reply_ack_msg(int sockfd, int transaction_id);
void reply_resend_msg(int sockfd, int transaction_id, int seq);

#endif /* MESSAGE_H_ */
