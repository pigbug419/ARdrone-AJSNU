#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//

#define SERVER_PORT 12345
#define DB_SIZE 10000
#define BUF_SIZE 2048
#define MAX_CLIENTS 8
// db size = 10000, bytes per one element = 1024
int main(int argc, char **argv)
{
	int ret = 0;
	// for handling db
	int db_fd;
	int element_num;
	int element_rec;
	char *tok;
	// for handling socket
	int server_sockfd, client_sockfd, sockfd;
	int state, client_len;
	int pid;
	int i, j, maxfd;
	int read_size;
	int client[MAX_CLIENTS];
	
	struct sockaddr_in clientaddr, serveraddr;
	fd_set readfds, otherfds, allfds;

	int client_cnt;
	
	char buf[BUF_SIZE]; // receiving data
	char temp[20]; // just shows client name
	int exp_client;	// expected number of the clients
	
	if(argv < 2){
		perror("not enough arguments, use ./(executable file name) (dbname) (client number)\n");
		return -1;
	}
	if(argv >= 3) exp_client = atoi[argv[2]];
	else exp_client = 1;
	// open db
	if((db_fd = open(argv[1], O_RDWR))<0)
	{
		perror("Fail to open the db file... exit\n");
		return -1;
	}
	// implementation for server socket
	memset(temp, 0x00, 20);
	memset(buf, 0x00, BUF_SIZE);
	state = 0;
	client_cnt = 0;
	
	for(i=0;i<exp_client;i++) client[i] = -1;

	if((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket error :");
		ret = -1;
		goto db_end;
	}

	memset(&serveraddr, 0x00, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVER_PORT);
	
	state = bind(server_sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if(state == -1)
	{
		perror("bind error: ");
		ret = -1;
		goto socket_end;
	}
	
	state = listen(server_sockfd, 5);
	if(state == -1)
	{
		perror("listen error: ");
		ret = -1;
		goto socket_end;
	}

	FD_ZERO(&readfds);
	FD_SET(server_sockfd, &readfds);
	printf("TCP Server is set up\n");
	fflush(stdout);
	
	maxfd = server_sockfd;
	// make a connection (accepting)
	while(client_cnt < exp_client)
	{
		allfds = readfds;
		if(FD_ISSET(server_sockfd, &allfds))
		{
			client_len = sizeof(clientaddr);
			client_sockfd = accept(server_sockfd, (struct sockaddr *) &clientaddr, &client-len);
			printf("\nClient (%s, %d) is connected\n", inet_ntoa(clientaddr.sin_addr), nthos(clientaddr.sin_port));
			client[client_cnt++] = client_sockfd;
			FD_SET(client_sockfd, &readfds);
			maxfd = maxfd > client_sockfd? maxfd: client_sockfd;
		}
	} // only accept clients until expected number of clients are connected... this is implemented for comparing response time amongthe server load (it is not the good server implementation....)
	
	// activate clients!
	sprintf(buf, "activate");
	for(i=0; i < client_cnt ; i++)
	{
		write(client[i], buf, strlen(buf));
	}
	
	while(client_cnt > 0)
	{
		allfds = readfds;
		state = select(maxfd + 1, &allfds, NULL, NULL, NULL);
		for(i = 0; i < exp_client; i++)
		{
			if((sockfd = client[i]) < 0) continue;
			if(FD_ISSET(sockfd, &allfds))
			{
				memset(buf, 0x00, BUF_SIZE);

				if((read_size = read(sockfd, buf, BUF_SIZE)) <= 0){
					close(sockfd);
					printf("close sockfd\n");
					FD_CLR(sockfd, &readfds);
					client[i] = -1;
				}
				else
				{
					buf[read_size] = '\0';
					// for debuging
					printf("RECV: %s", buf);
					// our protocol for read&write		
					tok = strtok(buf, "\t");
					if(tok[0] == 'r') // it is always r or w
					{
						tok = strtok(NULL, "\t");
						element_num = atoi(tok);
						tok = strtok(NULL, "\t");
						element_rec = atoi(tok);
						memset(buf, 0x00, BUF_SIZE);
						lseek(	
					}
					else
					{
						tok = strtok(NULL, "\t");
						element_num = atoi(tok);
						tok = strtok(NULL, "\t");
						element_rec = atoi(tok);
						tok = strtok(NULL, "\t");
						memset(buf, 0x00, BUF_SIZE);
					}
				}
				if(--state <= 0) break;
			}
		}
	}
	/* Receive message from client
	   
	 read mode
	 ex) message : r\ti\tj
	 
	 return i th db

	 write mode
	 
	 */
socket_end:
	for(i=0;i<exp_client;i++) if(client[i] > 0) close(client[i]);
	close(server_sockfd);
db_end:
	close(db_fd);
	return ret;	
}

