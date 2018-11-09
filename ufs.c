#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

#define RECV_BUFFER_SIZE 1024
#define SEND_BUFFER_SIZE 1024
#define PORT_NO 9090

int main(void){
	int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	struct sockaddr_in sa;
	char recvBuffer[RECV_BUFFER_SIZE];
	ssize_t recsize;
	socklen_t fromlength;
	char sendBuffer[SEND_BUFFER_SIZE];
	int numrv;

	memset(sendBuffer,0,sizeof(sendBuffer));

	memset(&sa,0,sizeof sa);
	sa.sin_family=AF_INET;
	sa.sin_addr.s_addr=htonl(INADDR_ANY);
	sa.sin_port=htons(PORT_NO);
	fromlength=sizeof(sa);

	if(-1==bind(sock,(struct sockaddr *)&sa, sizeof sa)){
		perror("Bind failed");
		return 1;
	}
	for(;;){
		recsize=recvfrom(sock,(void*)recvBuffer,sizeof recvBuffer, 0, (struct sockaddr*)&sa, &fromlength);
		if(recsize<0){
			fprintf(stderr,"%s\n",strerror(errno));
			return 1;
		}
		printf("Requested filename: %.*s\n",(int)recsize, recvBuffer);
		//open the file that we wish to tarnsfer
		FILE *fp=fopen(recvBuffer,"r");
		ssize_t sendmsg;
		if(fp==NULL){
			printf("File open error");
			/*char *msg="Not Found";
			sendmsg=sendto(sock,msg,strlen(msg),0,(struct sockaddr*)&sa,sizeof(sa));
			if(sendmsg<0){
				perror("sendto failed");
				return 0;
			}*/
			
			
			return 1;
		}if(fp!=NULL){ //file pointer not null
			while(!feof(fp)){ //not the end of file
				int nread=fread(sendBuffer,1,SEND_BUFFER_SIZE,fp);
				sendBuffer[nread]=0; //assign 0 to last index
				//printf("%s",sendBuffer);
				if(nread>0){
					printf("Sending the file...\n");
					int n=sendto(sock,sendBuffer,strlen(sendBuffer),0,(struct sockaddr*)&sa,sizeof sa);
					usleep(20000);
				
				}else{
					perror("Problem sendto\n");
					exit(1);		
				}
			//fclose(fp);
			}
			if(feof(fp))
				printf("End of file\n");
			if(ferror(fp))
				printf("Error reading the file at server program\n");
			break;
		}
		/*while(1){
			//Read file in chunks of 1024 bytes
			int nread=fread(sendBuffer,1, SEND_BUFFER_SIZE,fp);
			printf("Bytes read %d \n",nread);
			//If read was success, send data
			if(nread>0){
				printf("Sending the file...\n");
				int n=sendto(sock,sendBuffer,strlen(sendBuffer),0,(struct sockaddr*)&sa,sizeof sa);
				
				//printf("%s", sendBuffer);
				if(n<0){
					perror("Problem sending\n");
					exit(1);
				}
			}
			if(nread<SEND_BUFFER_SIZE){
				if(feof(fp))
					printf("End of file\n");
				if(ferror(fp))
					printf("Error reading the file at server program\n");
				break;
			}
		}*/
	}
	close(sock);
	return 0;
}	
