#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <czmq.h>

#define HOST '127.0.0.1'
#define PORT 5555

void* receive_weak_client(void);
void* public_request(void);
void* receive_response_signal(void);


int main(void){
	pthread_t receive_weak_client_thread;
	pthread_t pub_request_thread;
	pthread_t receive_response_thread;

	int ret_recv_weak, ret_pub, ret_disconnect;
	ret_recv_weak = pthread_create(&receive_weak_client_thread, NULL, receive_weak_client, (void*) NULL);
	ret_pub = pthread_create(&pub_request_thread, NULL, public_request, (void*) NULL);
	ret_disconnect = pthread_create(&receive_response_thread, NULL, receive_response_signal, (void*) NULL);
	
	pthread_join( receive_weak_client_thread, NULL);
	pthread_join( pub_request_thread, NULL);
	pthread_join( receive_response_thread, NULL);
	exit(0);
}

void* public_request(void){
	zsock_t* socket = zsock_new_pub("@tcp://127.0.0.1:5555");
	assert(socket);

	while(!zsys_interrupted){
		zsys_info("Publish disconect client from AP 1");	
		zsock_send(socket, "sss", "SIGNAL", "DISCONNECT", "MAC1");
		zclock_sleep(1000);
	}
	
	zsock_destroy(&socket);
	return 0;
}

void* receive_weak_client(void){
	zsock_t* pull = zsock_new_pull(">tcp://127.0.0.1:6666");
	while(!zsys_interrupted){
		char* string_receive = zstr_recv(pull);
		printf("AP send client weak signal:%s", string_receive);
		zstr_free(&string_receive);
	}

}

void* receive_response_signal(void){
	zsock_t* pull = zsock_new_pull(">tcp://127.0.0.1:7777");
	while(!zsys_interrupted){
		char* string_receive = zstr_recv(pull);
		printf("AP disconnect client:%s", string_receive);
		zstr_free(&string_receive);
	}

}
