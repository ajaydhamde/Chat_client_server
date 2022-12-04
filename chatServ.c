#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include "myqueue.h"

/*	global variable	*/
int sd,cd;
struct sockaddr_in serv,clie;
pthread_t th[20];
/*	thread protection contex switch	*/

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

/*	dataset for qoury	*/
typedef struct {
      int sendid;
      int recid;
      char msg[128];
}DATASET_T;

void request_handler(void *pclient){
      int cli_sock = *((int *)pclient);
	free(pclient);
      char buff[128];
      size_t read_bytes;
      DATASET_T obj = {0};
      read_bytes = recv(cli_sock,(DATASET_T*)&obj,sizeof(DATASET_T),0);
      if(read_bytes<0)
      	perror("Server rec failed..\n");
      printf("Server rec sendid : %d recid : %d msg : %s\n"
      ,obj.sendid,obj.recid,obj.msg);
      /*	extracting information	*/
      //update information
      //get another client id
      /*	send msg to another client	*/
//      send((int)obj.sendid,(DATASET_T*)&obj,sizeof(DATASET_T),0);
      /*	close another client	*/
      //close(obj.recid);
      /*	close main rec client	*/
//      close(obj.recid);
}
void *thread_fun(void *){
      /*	thread function	*/
      while(1){
      	/*remove con from queue	*/

      	int *pclient;
      	pthread_mutex_lock(&mutex);
      	if((pclient = dequeue()) == NULL){
      		pthread_cond_wait(&cond,&mutex);
      		pclient = dequeue();
      	}
      	pthread_mutex_unlock(&mutex);
      	if(pclient != NULL){
      		/*	connection detect	*/
      		request_handler(pclient);
      	}
	pthread_detach(pthread_self());
      }
}

int main(){
      /*	init variables	*/
      int ret;
      /*	init socket	*/
      pthread_mutex_init(&mutex,NULL);
      pthread_cond_init(&cond,NULL);
      sd = socket(AF_INET,SOCK_STREAM,0);
      if(sd < 0){
      	perror("socket construct failed..\n");
      }
      bzero(&serv,sizeof(struct sockaddr_in));
      /*	set socket value	*/
      serv.sin_family = AF_INET;
      serv.sin_port = htons(1105);
      inet_aton("localhost",&serv.sin_addr);
      /*	bind socket	*/
      int b = bind(sd,(struct sockaddr*)&serv,sizeof(serv));
      /*	listen socket	*/
      listen(sd,8);
      /*	perform accept connection	*/
      for(int i = 0;i<20;i++){
      	pthread_create(&th[i],NULL,thread_fun,NULL);
      }
      while(1){
      	
      	
      	cd = accept(sd,NULL,NULL);
      	printf("server cd : %d\n",cd);
      	if(cd < 0){
      		perror("client accept failed..\n");
      	}
      	int val = 1;
      	setsockopt(cd,SOL_SOCKET,MSG_NOSIGNAL,&val,sizeof(val));
      	
      	int *pclient  = malloc(sizeof(int));
              *pclient = cd;
      /*	put connection in queue	*/
      	pthread_mutex_lock(&mutex);
      	enqueue(pclient);
      	pthread_cond_signal(&cond);
      	pthread_mutex_unlock(&mutex);



      }
      /*	destroy objects	*/
      pthread_mutex_destroy(&mutex);
      pthread_cond_destroy(&cond);



return 0;
}
