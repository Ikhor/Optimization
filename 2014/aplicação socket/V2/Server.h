#include<winsock2.h>
#include<iostream>
#include<stdio.h>

using namespace std;

class Server{

private:

	  SOCKET server_sockfd;
	  SOCKET client_sockfd;
	  int server_len;
	  int client_len;
	  struct sockaddr_in server_address;
	  struct sockaddr_in client_address;
	  char *ipServidor; //Deixar por padrão localhost o valor está sendo colocado no metodo InitServer

public:

	void InitServer();
	void Start();
	void sendMsg(SOCKET sockfd);
	void recvMsg(SOCKET sockfd);

};
