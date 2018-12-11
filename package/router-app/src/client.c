#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <czmq.h>

#define HOST '127.0.0.1'
#define PORT 5555

void* send_weak_client(void);
void* check_signal(void);
void* send_disconnect_signal(void);

int main(void){
	pthread_t send_weak_client_thread;
	pthread_t sub_check_signal_thread;
	pthread_t send_disconnect_signal_thread;

	int ret_send_weak, ret_check, ret_disconnect;
	ret_send_weak = pthread_create(&send_weak_client_thread, NULL, send_weak_client, (void*) NULL);
	ret_check = pthread_create(&sub_check_signal_thread, NULL, check_signal, (void*) NULL);
	ret_disconnect = pthread_create(&send_disconnect_signal_thread, NULL, send_disconnect_signal, (void*) NULL);
	
	pthread_join(send_weak_client_thread, NULL);
	pthread_join(sub_check_signal_thread, NULL);
	pthread_join(send_disconnect_signal_thread, NULL);
	exit(0);
}

void* check_signal(void){
	zsock_t* socket = zsock_new_sub(">tcp://127.0.0.1:5555", "SIGNAL");
	assert(socket);

	char* topic;
	char* frame;
	zmsg_t* msg;
	int rc = zsock_recv(socket, "sm", &topic, &msg);
	assert(rc == 0);

	
	while(!zsys_interrupted){
		zsys_info("Recv on %s\n", topic);	
		while(frame = zmsg_popstr(msg)){
			zsys_info("> %s", frame);
			free(frame);
		}
		zsock_recv(socket, "sm", &topic, &msg);
	}
	
	free(topic);
	zmsg_destroy(&msg);

	zsock_destroy(&socket);
	return 0;
}

void* send_weak_client(void){
	zsock_t* push = zsock_new_push(">tcp://127.0.0.1:6666");
	while(!zsys_interrupted){
		char* string_to_send = "MAC1";
		zstr_send(push, string_to_send);
		printf("client send %s", string_to_send);
		zclock_sleep(1000);
	}

}

void* send_disconnect_signal(void){
	zsock_t* push = zsock_new_push(">tcp://127.0.0.1:7777");
	while(!zsys_interrupted){
		char* string_to_send = "MAC1 disconnected";
		zstr_send(push, string_to_send);
		printf("client send %s", string_to_send);
		zclock_sleep(1000);
	}

}
