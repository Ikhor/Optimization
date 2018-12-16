#include "Server.h"

void Server::InitServer(){

	ipServidor = "127.0.0.1";//Por Padrão está localhost

	// Winsock DLL
	WSADATA wsadata;
	if(WSAStartup(0x101,(LPWSADATA)&wsadata) != 0) {
		printf("Winsock Error\n");
		exit(1);                                         
	}
	
	//  server socket
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(server_sockfd == SOCKET_ERROR) {
		printf("Socket Error\n");
		exit(1);
	}

	server_address.sin_family = AF_INET; // AF_INT(IPv4)
	server_address.sin_addr.s_addr = inet_addr(ipServidor);// IP
	server_address.sin_port = 9734; //Porta 
	server_len = sizeof(server_address);

	if(bind(server_sockfd, (struct sockaddr *)&server_address, server_len) < 0) {
		printf("Bind Error\n");
		exit(1);
	}

	if(listen(server_sockfd, 5) < 0) {
		printf("Listen Error\n");
		exit(1);
	}

}

void Server::Start(){
	

	while(true) {

		cout << "Aguardando conexao...\n";
		client_len = sizeof(client_address);

		client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
		
		if(client_sockfd == SOCKET_ERROR) {
			printf("Accept Error\n");
			exit(1);
		}
		
		recvMsg(client_sockfd);
		sendMsg(client_sockfd);
		
		closesocket(client_sockfd); // socket close
	}
}

void Server::sendMsg(SOCKET sockfd){

	//Monta Msg
	char mensagemEnvia[60];
	cin >> mensagemEnvia;

	//Envia Msg
	send(sockfd, mensagemEnvia, 60, 0);
	 
}

void Server::recvMsg(SOCKET sockfd){

	char mensagemRecebe[60];
	//RecebeMsg
	recv(sockfd, mensagemRecebe, 60, 0);

	//imprime msg
	cout << mensagemRecebe << endl;

}

