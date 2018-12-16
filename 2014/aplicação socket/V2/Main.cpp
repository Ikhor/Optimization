#include "Client.h"
#include "Server.h"

int main(){

	Server servidor;
	servidor.InitServer();

	servidor.Start();


	Client cliente;
	cliente.InitClient();

	cliente.Start();
}