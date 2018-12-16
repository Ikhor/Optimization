#include "Client.h"


void Client::sendMsg(SOCKET sockfd){

	//Monta Msg
	char mensagemEnvia[60];
	cin >> mensagemEnvia;

	//Envia Msg
	send(sockfd, mensagemEnvia, 60, 0);
	 
}

void Client::recvMsg(SOCKET sockfd){

	char mensagemRecebe[60];
	//RecebeMsg
	recv(sockfd, mensagemRecebe, 60, 0);

	//imprime msg
	cout << mensagemRecebe << endl;

}

void Client::InitClient(){

	ipServidor = "127.0.0.1";//Por Padrão está localhost

	WSADATA wsadata;
	if(WSAStartup(0x101,(LPWSADATA)&wsadata) != 0) {
		printf("Winsock Error\n"); 
		exit(1);
	}
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(ipServidor);
	address.sin_port = 9734;
	len = sizeof(address);

	result = connect(sockfd, (struct sockaddr *)&address, len);  

	if(result == -1) {
		printf("Connect Error");
		exit(1);
	}

}
   
void Client::Start(){
  
	sendMsg(sockfd);
	recvMsg(sockfd);
	
	closesocket(sockfd);
}