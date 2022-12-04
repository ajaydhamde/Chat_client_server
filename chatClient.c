#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
/*	dataset for quory	*/
typedef struct{
	int sendid;
	int recid;
	char msg[128];
}DATASET_T;
int main(){
	int sd,portno,n;
	int ret;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buff[256];
	struct sockaddr_in serv;
	/*	socket init	*/
	sd = socket(AF_INET,SOCK_STREAM,0);
	if(sd<0){
		perror("socket init failed..\n");
		return -1;
	}
	/*	reset sockaddr configuration	*/
	bzero(&serv,sizeof(struct sockaddr_in));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(1105);
	inet_aton("localhost",&serv.sin_addr);
	/*	connection	*/
	connect(sd,(struct sockaddr *)&serv,sizeof(serv));
	DATASET_T obj = {sd,89,"linux-kernel"};
	for(;;){
		send(sd,(DATASET_T*)&obj,sizeof(obj),0);
		/*	rec socket msg	*/
		recv(sd,(DATASET_T*)&obj,sizeof(obj),0);
	
	}
	close(sd);

}
