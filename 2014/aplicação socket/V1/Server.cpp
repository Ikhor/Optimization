#include<winsock2.h>
#include<iostream>
#include<stdio.h>
#include <fstream>
#include "Poset.h"
#include "RegistroOT.h"

//Incluir no linker a biblioteca ws2_lib.32
using namespace std;


//Prototipos das funções GTS
void BuscaLocal(Poset p,list<Vertice*>* ExtensaoLinear);
double Grasp(Poset p,float alfa,int qtdRodadas,bool buscaLocal,bool aleatorio, int* solucao);
void ConstruirSolucao(Poset &p,float valorAlfa,bool aleatorio, list<Vertice*>* ExtLinear);
Poset* construirPoset(char* arq);



int main() {
  SOCKET server_sockfd, client_sockfd;
  int server_len, client_len;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;
 
  // ?? Winsock DLL
  WSADATA wsadata;
  if(WSAStartup(0x101,(LPWSADATA)&wsadata) != 0) {
    printf("Winsock Error\n");
    exit(1);                                         
  }
 
  // ?? server socket
  server_sockfd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET(??IPv4); SOCK_STREAM; 0(????????,?TCP)
  if(server_sockfd == SOCKET_ERROR) {
    printf("Socket Error\n");
    exit(1);
  }
 
  // struct sockaddr_in {
  //     short int               sin_family; /* AF_INT(??IPv4) */
  //     unsigned short int sin_port;    /* Port(??) */
  //     struct in_addr       sin_addr;   /* IP?? */
  // };
  // sturct in_addr {
  //     unsigned long int s_addr;
  // };
  server_address.sin_family = AF_INET; // AF_INT(??IPv4)
  server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // ??IP??
  server_address.sin_port = 9734; //????
  server_len = sizeof(server_address);
 
  if(bind(server_sockfd, (struct sockaddr *)&server_address, server_len) < 0) {
    printf("Bind Error\n");
    exit(1);
  }

  if(listen(server_sockfd, 5) < 0) {
    printf("Listen Error\n");
    exit(1);
  }

  //InicioGTS
	float alfa = 1.0f;

	int qtdRodadas = 1;

	bool buscaLocal = false;
	
	bool aleatorio = false;
	
	srand(1234);
	//Fim GTS
	int incomparabilidades;
	char nome[50];
	strcpy(nome, "poset6.txt");

	Poset *poset = construirPoset(nome);

 
  while(1) {
    
    printf("Server waiting...\n");
    client_len = sizeof(client_address);

    client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
    if(client_sockfd == SOCKET_ERROR) {
      printf("Accept Error\n");
      exit(1);
    }

	char mensagemEnvia[60];
	char mensagemRecebe[60];
	do{
	
    recv(client_sockfd, mensagemRecebe, 60, 0); // Linux socket programming ? read
    cout << mensagemRecebe << endl;


	int *solucao = new int[ poset->getNumeroDeVertices() ];

	double SolucaoGRASP = Grasp(*poset,alfa,qtdRodadas,buscaLocal,aleatorio, solucao);

	int fobj = SolucaoGRASP;

	cout << "Solucao GTS: "<< fobj << endl;

	itoa (fobj,mensagemEnvia,10);

    send(client_sockfd, mensagemEnvia, 60, 0); // Linux socket programming ? write
	}while(mensagemRecebe[0] != 'E');

    closesocket(client_sockfd); // Linux socket programming ? close
  }
}



/* 
Inicio Código GTS 
*/
void ConstruirSolucao(Poset &p,float valorAlfa,bool aleatorio, list<Vertice*>* ExtLinear){		

	//Descobre o maior grau de saida presente no poset para montar as "caixas" da ordenação topologica
	int maiorGrau = -1;
	for(vector<Vertice*>::iterator it = p.getVertices()->begin()++;it != p.getVertices()->end();it++)
		if((*it)->getGrauDeSaida() > maiorGrau)
			maiorGrau = (*it)->getGrauDeSaida();

	int *NaCaixa = new int[p.getNumeroDeVertices()];//Vetor que indica em que "caixa" está o vertice

	vector<list<Vertice*>> *caixas = new vector<list<Vertice*>>(maiorGrau+1);
	//Vector de iteradores será utilizado para realocação em tempo O(1) durante a ordenação topológica por caixa
	list<Vertice*>::iterator *CaixaIterator = new list<Vertice*>::iterator[p.getNumeroDeVertices()];

	//Preenche as "caixas"
	for(vector<Vertice*>::iterator it = p.getVertices()->begin()++;it != p.getVertices()->end();it++) {
		int indiceDaCaixa = (*it)->getGrauDeSaida();
		caixas->at(indiceDaCaixa).push_back((*it));
		NaCaixa[(*it)->getIndice()] = indiceDaCaixa;
		auto itAuxiliarParaCapturarFinal = caixas->at(indiceDaCaixa).end();
		CaixaIterator[(*it)->getIndice()] = --itAuxiliarParaCapturarFinal;
	}		

	
	bool PrimeiraSelecao = true;//Variavel de controle para execução da primeira rodada da ordenação topologica por caixa

	while(caixas->at(0).size() > 0) {
		Dados *VetorDeCustosParaCaixa0 = new Dados[caixas->at(0).size()];
		list<Vertice*>::iterator itV = caixas->at(0).begin();
		/*
		Percorrer a caixa de grau de saida 0 para armazenar os custos
		dos elementos atualmente na caixa com relação ao último
		posto na extensão linear. Caso seja a primeira vez todos os custos serão zero
		*/
		for(unsigned int i = 0; i < caixas->at(0).size(); i++){
			VetorDeCustosParaCaixa0[i].setIndice((*itV)->getIndice());
			if(PrimeiraSelecao == true){
				VetorDeCustosParaCaixa0[i].setCusto(0);
			}else{
				double custo;
				(*itV)->obterCustoParaVertice(ExtLinear->front()->getIndice(),custo);
				VetorDeCustosParaCaixa0[i].setCusto(custo);
			}
			itV++;
		}
		vector<Dados> HeapComTodosOsElementosDaCaixaZero(caixas->at(0).size());
		PrimeiraSelecao = false;
		
		for(int i = 0; i < caixas->at(0).size(); i++) {
			HeapComTodosOsElementosDaCaixaZero.at(i) = VetorDeCustosParaCaixa0[i];
		}
		/*
		O valor de alfa deverá estar entre 0.0 e 1.0. O if foi modificado para ser acima de 0.99
		devido a Tiberius lembrar algo sobre os tipos de dados e o 1 ser armazenado como quase 1, logo
		para valores maiores que 0.99 será assumido que o valor de aleatoriedade será 1.0, ou seja, o 
		algoritmo será totalmente aleatorio. 
		Devido ao arredondamento caso o TamanhoDaListaDeMelhoresCandidatos tenha como resultado o valor zero
		será adicionado um unidade ficando então com o tamanho igual a 1.
		*/
		
		int TamanhoDaListaDeMelhoresCandidatos;

		if(valorAlfa > 0.99)
			TamanhoDaListaDeMelhoresCandidatos = HeapComTodosOsElementosDaCaixaZero.size();
		else
			TamanhoDaListaDeMelhoresCandidatos = int(HeapComTodosOsElementosDaCaixaZero.size()*valorAlfa);
		if(TamanhoDaListaDeMelhoresCandidatos == 0)
			TamanhoDaListaDeMelhoresCandidatos++;

		vector<Dados> ListaRestritaDeCandidatos(TamanhoDaListaDeMelhoresCandidatos);
		/*for(int i = 0 ; i < TamanhoDaListaDeMelhoresCandidatos ; i++) 
			ListaRestritaDeCandidatos.at(i) = VetorDeCustosParaCaixa0[i];*/

		/*
		As funções make_heap estão com o construtor no formato begin e end de cada vector para torna-lo
		uma heap.
		http://www.cplusplus.com/reference/algorithm/make_heap/
		*/
		make_heap(HeapComTodosOsElementosDaCaixaZero.begin(),HeapComTodosOsElementosDaCaixaZero.end());
		//make_heap(ListaRestritaDeCandidatos.begin(),ListaRestritaDeCandidatos.end());

		/*
		O for abaixo pecorre a lista de todos os candidatos representado pela HeapComTodosOsElementosDaCaixaZero
		Retirando da heap o seu topo e colocando na lista de candidatos restritos.

		A função pop_heap rearranja os elementos na heap colocando o elemento a ser "retirado" na ultima posição
		do vector. 

		http://www.cplusplus.com/reference/algorithm/push_heap/
		http://www.cplusplus.com/reference/algorithm/pop_heap/
		*/
		for(unsigned int i = 0;i < TamanhoDaListaDeMelhoresCandidatos ; i++){

			pop_heap(HeapComTodosOsElementosDaCaixaZero.begin(),HeapComTodosOsElementosDaCaixaZero.end());
			ListaRestritaDeCandidatos.at(i) = HeapComTodosOsElementosDaCaixaZero.back();
			HeapComTodosOsElementosDaCaixaZero.pop_back();

		}
		//make_heap(ListaRestritaDeCandidatos.begin(), ListaRestritaDeCandidatos.end());
		/*
		a variavel chamada aleatorio será uma variavel de controle para aleatoriedade da solução gerada,
		caso o parametro passado for verdadeiro irá utilizar a função advance em conjunto com a função
		rand para "pular" para o candidato escolhido.

		Os parametros passados para advance serão a ListaDeCandidatosRestrita e a posição de casas a serem "avançadas"

		http://www.cplusplus.com/reference/iterator/advance/

		Após isso será feita uma busca utilizando um segundo iterador para localizar a posição escolhida
		para localizar o ponteiro para o vertice correto.
		*/

		vector<Dados>::iterator SelecionarAleatorioNaListaDeCandidatosRestrita = ListaRestritaDeCandidatos.begin();
		if(aleatorio == true)
			advance(SelecionarAleatorioNaListaDeCandidatosRestrita,rand() % ListaRestritaDeCandidatos.size());

		list<Vertice*>::iterator itLocalizador;
		Vertice * verticeGrauZero;
		for(itLocalizador = caixas->at(0).begin(); itLocalizador != caixas->at(0).end(); itLocalizador++)
			if((*itLocalizador)->getIndice() == (*SelecionarAleatorioNaListaDeCandidatosRestrita).getIndice()){
				verticeGrauZero = (*itLocalizador);
				break;
			}

		/*
		Atualiza as dependencias do vertice escolhido diminuindo o valor da caixa onde 
		suas dependencias estão
		*/
		list<Arco*>::iterator it = verticeGrauZero->getListaDeEntrada()->begin();
		for (; it != verticeGrauZero->getListaDeEntrada()->end(); it++) {

			int ondeInicioEsta = NaCaixa[(*it)->getInicio()->getIndice()];
			caixas->at(ondeInicioEsta).erase(CaixaIterator[(*it)->getInicio()->getIndice()]);
			caixas->at(ondeInicioEsta-1).push_front((*it)->getInicio());
			NaCaixa[(*it)->getInicio()->getIndice()] = ondeInicioEsta-1;
			CaixaIterator[(*it)->getInicio()->getIndice()] = caixas->at(ondeInicioEsta-1).begin();
	
		}
		/*
		Atualiza a extensão e retira o vertice de grau zero
		*/
		ExtLinear->push_front(verticeGrauZero);
		//caixas->at(0).remove(verticeGrauZero);
		caixas->at(0).erase(itLocalizador); // O(1)!
		verticeGrauZero->posicao = p.getNumeroDeVertices() + 1 - ExtLinear->size();
	}//while(caixas->at(0).size() > 0)

	/*
	Seguindo a regra desreferenciar para NULL apos de usar o delete
	*/
	delete[] CaixaIterator;
	delete[] NaCaixa;
	delete caixas;
	NaCaixa = NULL;
	caixas = NULL;
	CaixaIterator = NULL;
}

/*
A busca local está implementada na forma de uma lista circular parando apenas quando a última
posição atulizada coincidir com a posição atual
*/
void BuscaLocal(Poset p,list<Vertice*>* ExtensaoLinear){

			int ultimaMudanca = 0;
			int indiceUp;
			int indiceDown;
			int t;
			int n = p.getNumeroDeVertices();
			bool parada = true;
			while(parada){
				for(t = 0; t < n; t++){
					int posicao = (ultimaMudanca+t)%n;
					p.IntervaloDownUpSet(posicao,indiceDown,indiceUp);		
					if(p.melhorMovimento(ExtensaoLinear,posicao,indiceDown,indiceUp)){
						if(ultimaMudanca != posicao){
							ultimaMudanca = posicao;
							//cout << posicao << endl;
						}else{
							parada = false;
						}
						break;
					}
				}
				if(t == n)
					break;
			}
			if(p.auditarExtLinear( ExtensaoLinear ) == false)
				cout << "ERRO Busca local gerou solução inválida"<< endl;
}

double Grasp(Poset p,float alfa,int qtdRodadas,bool buscaLocal,bool aleatorio, int* solucao){
	
	double custoTotal = DBL_MAX;
	list<Vertice*> *ExtensaoLinear = new list<Vertice*>;

	for(int i = 0 ; i < qtdRodadas ; i++ ){
		//Chamada a ordenação Topologica(Fase construtiva)
		ConstruirSolucao( p, alfa, aleatorio, ExtensaoLinear);

		if( p.auditarExtLinear( ExtensaoLinear ) == false )
			cout << "ERRO geração de solução inválida"<< endl;

		if( buscaLocal == true )
			BuscaLocal( p, ExtensaoLinear );
		
		double custo = p.ContabilizarExtLinear( *ExtensaoLinear );
		if( custoTotal > custo ){
			custoTotal = custo;

			int indice = 0;
			for(auto  iterator = ExtensaoLinear->begin(); iterator !=  ExtensaoLinear->end(); iterator++)
				solucao[indice++] =  (*iterator)->getIndice()+1;
		}

		//Limpa a extensão para uma nova interação
		ExtensaoLinear->clear();
	}
	return custoTotal;
}

Poset* construirPoset(char* arq){
		int NumeroDeVertices, NumeroDeArcosDiretos, NumeroDeArcosComCustos;

		ifstream fin;
		fin.open(arq);
		fin >> NumeroDeVertices;
		fin >> NumeroDeArcosDiretos;
		fin >> NumeroDeArcosComCustos;

		Poset *p = new Poset(NumeroDeVertices);

		int aux1,aux2,aux3;
		for (int i = 0; i < NumeroDeArcosDiretos; i++) {
			fin >> aux1;
			fin >> aux2;
			p->adicionarArco( aux1 , aux2 );
		}

		for (int i = 0; i < NumeroDeArcosComCustos; i++) {
			fin >> aux1;
			fin >> aux2;
			fin >> aux3;
			p->adicionarArcoDeCusto( aux1 , aux2 , aux3 );
		}

		fin.close();
		return p;
	}