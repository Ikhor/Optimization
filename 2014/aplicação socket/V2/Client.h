#include<winsock2.h>
#include<iostream>
#include<stdio.h>

using namespace std;

class Client{

private:

	  SOCKET sockfd;
	  int len;
	  struct sockaddr_in address;
	  int result;
	  char *ipServidor; //Deixar por padr�o localhost o valor est� sendo colocado no metodo InitServer

public:

	void InitClient();
	void Start();
	void sendMsg(SOCKET sockfd);
	void recvMsg(SOCKET sockfd);

};
