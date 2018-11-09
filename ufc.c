/*udp port number 8080 at host b
input argument is directory
client side has an input argument filename
if no file found the server will return "Not Found"
buffer size of the client and server program is 1024
for the files larger than 1km, use sendto recvfrom*/

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define SEND_BUFFER_SIZE 1024 /*buffer size of client*/
#define RECV_BUFFER_SIZE 1024 /*buffer size of server*/
#define SERVER_PORT_NO 9090 /*port number at host b*/
#define SERVER_IP_ADDR "10.10.1.100" /*ip address of host b*/
//#define FILE_NAME "sample_file.txt"

int main(void){
	int sockfd=0;
	int recievedByte=0;
	int sentByte=0;
	ssize_t recsize; /*the data type used to represent the sizes of blocks that can be read or written in a single operation*/
	char sendBuffer[SEND_BUFFER_SIZE];
	char recvBuffer[RECV_BUFFER_SIZE];
	char fileBuffer[100];
	/*memset(recvBuffer,'0',sizeof(recvBuffer) copies 0 to the first sizeof(recvBuffer) characters of the string pointo to, by the argument recvBuffer*/
	struct sockaddr_in sa;
	socklen_t len;
	int length;
	
	//create a socket
	if((sockfd=socket(AF_INET,SOCK_DGRAM,0))<0){ /*AF_INET: IP address family or domain in which socket is created; SOCK_DGRAM: type of socket. UDP; 0: protocol*/
		printf("\n Error: Socket could not be created. \n");
		return 1;	
	}else{
		printf("\n Socket Created!\n");
	}
	//zero out socket address
	memset(&sa,0,sizeof sa);
	
	//initialize socket address in data structure
	sa.sin_family=AF_INET;
	sa.sin_port=htons(SERVER_PORT_NO); //PORT
	sa.sin_addr.s_addr=inet_addr(SERVER_IP_ADDR);
	len=sizeof sa;
	
	printf("Enter the name of the file\n");
	scanf("%s", fileBuffer);

	strcpy(sendBuffer, fileBuffer);
	

	//send file name
	sentByte=sendto(sockfd, sendBuffer, strlen(sendBuffer),0,(struct sockaddr*)&sa, sizeof sa);
	if(sentByte < 0){
		printf("Send to: Error sending the file name: %s\n", strerror(errno));
		return 1;
	}
	printf("Filename sent!\n");
	/*Create file where data will be stored*/
	FILE *fp;
	fp=fopen(sendBuffer,"w");
	if(NULL==fp){
		printf("Error opening file");
		return 1;
	}
	printf("Begin recieving file...\n");
	
	
	//Recieve data in chunks of 1024 bytes
	while(1){
		recievedByte=recvfrom(sockfd, recvBuffer, sizeof recvBuffer, 0, (struct sockaddr*)&sa, &len);
	if(recievedByte < 0){
		printf("Recieved from: Error in recieving file \n");
		exit(1);
	}if(recievedByte<RECV_BUFFER_SIZE){
		printf("Number of bytes recieved: %d\n",recievedByte);
		//if(NULL==fp){
			//printf("%s",recvBuffer);
			//return 1;
		//}
		
		fwrite(recvBuffer,1,recievedByte,fp);
		exit(1);
	}
	
	printf("Number of bytes recieved: %d\n",recievedByte);
	//printf("%s",recvBuffer);
	fwrite(recvBuffer,1,recievedByte,fp);
	memset(recvBuffer,'0',sizeof(recvBuffer));
	
	/*if(fwrite(recvBuffer,1,recievedByte,fp)<0){
		printf("Error writting in file\n");
		exit(1);
	}*/
	
	}
	
	/*recievedByte=0;
	recvBuffer[RECV_BUFFER_SIZE];*/
	//memset(recvBuffer,'0',sizeof(recvBuffer));
	/*struct sockaddr_in incomingFile;	
	socklen_t incoming_size=sizeof(incomingFile);
	while(1){
		ssize_t msg_size=recvfrom(sockfd, recvBuffer, sizeof(recvBuffer),0, (struct sockaddr*)(&incomingFile),(&incoming_size));
		if(msg_size==-1){
			printf("Not Found!\n");
			return 3;
		}	
	}*/
	/*while(1){
		recievedByte=recvfrom(sockfd, recvBuffer, sizeof recvBuffer, 0, (struct sockaddr*)&sa, &len);
		if(recievedByte<0){
			printf("Error in recieving file\n");
			exit(1);
		}
		if(recievedByte<RECV_BUFFER_SIZE){
			printf("Bytes recieved %d\n",recievedByte);
			fwrite(recvBuffer,1,recievedByte,fp);
			exit(1);
		}*/
		/*for(;;){
		length=recvfrom(sockfd,recvBuffer,RECV_BUFFER_SIZE,0,(struct sockaddr *)&sa, &len);
		printf("recieved %d bytes\n",length);
			if(length>0){
				recvBuffer[length]=0;
				printf("%s\n",recvBuffer);
			}
		}
		printf("Bytes recieved %d\n",recievedByte);
		//printf("Message: %s\n",recvBuffer);
		fwrite(recvBuffer,1,recievedByte,fp);
		memset(recvBuffer,'0',sizeof(recvBuffer));
	}*/
	
	/*if(recievedByte<0){
		printf("\n Read Error\n");
	}*/
	fclose(fp);
	close(sockfd);
	return 0;
	
}



