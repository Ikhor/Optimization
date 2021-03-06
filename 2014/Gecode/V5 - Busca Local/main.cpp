#include <fstream>
#include "Poset.h"
#include "MCLEModel.h"
#include "RegistroOT.h"

using namespace std;

//Prototipos das fun��es GTS
void BuscaLocal(Poset p,list<Vertice*>* ExtensaoLinear);
double Grasp(Poset p,float alfa,int qtdRodadas,bool buscaLocal,bool aleatorio, int* solucao);
void ConstruirSolucao(Poset &p,float valorAlfa,bool aleatorio, list<Vertice*>* ExtLinear);


Poset* construirPoset(char* arq, int ** &custos, int &incomparabilidades) {
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

	incomparabilidades = 0;
	custos = new int*[ NumeroDeArcosComCustos ];
	for (int i = 0; i < NumeroDeArcosComCustos; i++) {
		fin >> aux1;
		fin >> aux2;
		fin >> aux3;
		p->adicionarArcoDeCusto( aux1 , aux2 , aux3 );
		++ incomparabilidades;
		custos[i] = new int[3];
		custos[i][0] = aux1;
		custos[i][1] = aux2;
		custos[i][2] = aux3;
	}

	fin.close();

	return p;
}

bool getASolution(Poset *poset, int **costs, int incomparabilities, int *&sol, int &objValue, int Tvizinhaca, int upperBound, bool disregardSolution=false) {
	bool returnValue = true;

	MCLE * model;
	if (disregardSolution)
		model = new MCLE(poset, costs, incomparabilities, NULL, Tvizinhaca, upperBound);
	else
		model = new MCLE(poset, costs, incomparabilities, sol, Tvizinhaca, upperBound);

	BAB<MCLE> bab(model);

	MCLE *s;
	while (s = bab.next ()) { break; } // Obtem s� uma solu��o.

	if (s != NULL) {
		s->getSolution( sol );
		objValue = s->getOjbFunctionValue();
	}
	else
		returnValue = false;

	delete s;
	delete model;
	
	return returnValue;
}

int main(){

	//InicioGTS
	float alfa = 1.0f;

	int qtdRodadas = 1;

	bool buscaLocal = false;
	
	bool aleatorio = false;
	
	srand((unsigned)time(NULL));
	//Fim GTS

	int larguraVizinhaca = 6;
	int ** custos;
	int incomparabilidades;
	char nome[50];
	strcpy(nome, "poset7.txt");

	Poset *poset = construirPoset(nome, custos, incomparabilidades);

	

	int *solucao = new int[ poset->getNumeroDeVertices() ];


	double SolucaoGRASP = Grasp(*poset,alfa,qtdRodadas,buscaLocal,aleatorio, solucao);

	int fobj = SolucaoGRASP;

	cout << "Solucao GTS: "<< fobj << endl;

	bool ViewSolution = false;//Depurador Visual

	if(ViewSolution){
		for(int i =0 ;i < poset->getNumeroDeVertices(); i++)
			cout << solucao[i] << " ";
		cout<< endl;
	}

	bool PrimeiraSolGecode =  true;
	{
		while (getASolution(poset, custos, incomparabilidades, solucao, fobj, TamanhoVizinha, fobj-1)) {
			cout << "Gecode:" << fobj << endl;
			PrimeiraSolGecode = false;
			if(ViewSolution){
				for(int i =0 ;i < poset->getNumeroDeVertices(); i++)
					cout << solucao[i] << " ";
				cout<< endl;
			}
		}
	}
	
	cout << "Final:" << fobj << endl;



	system("pause");
}


/* 
Inicio C�digo GTS 
*/
void ConstruirSolucao(Poset &p,float valorAlfa,bool aleatorio, list<Vertice*>* ExtLinear){		

	//Descobre o maior grau de saida presente no poset para montar as "caixas" da ordena��o topologica
	int maiorGrau = -1;
	for(vector<Vertice*>::iterator it = p.getVertices()->begin()++;it != p.getVertices()->end();it++)
		if((*it)->getGrauDeSaida() > maiorGrau)
			maiorGrau = (*it)->getGrauDeSaida();

	int *NaCaixa = new int[p.getNumeroDeVertices()];//Vetor que indica em que "caixa" est� o vertice

	vector<list<Vertice*>> *caixas = new vector<list<Vertice*>>(maiorGrau+1);
	//Vector de iteradores ser� utilizado para realoca��o em tempo O(1) durante a ordena��o topol�gica por caixa
	list<Vertice*>::iterator *CaixaIterator = new list<Vertice*>::iterator[p.getNumeroDeVertices()];

	//Preenche as "caixas"
	for(vector<Vertice*>::iterator it = p.getVertices()->begin()++;it != p.getVertices()->end();it++) {
		int indiceDaCaixa = (*it)->getGrauDeSaida();
		caixas->at(indiceDaCaixa).push_back((*it));
		NaCaixa[(*it)->getIndice()] = indiceDaCaixa;
		auto itAuxiliarParaCapturarFinal = caixas->at(indiceDaCaixa).end();
		CaixaIterator[(*it)->getIndice()] = --itAuxiliarParaCapturarFinal;
	}		

	
	bool PrimeiraSelecao = true;//Variavel de controle para execu��o da primeira rodada da ordena��o topologica por caixa

	while(caixas->at(0).size() > 0) {
		Dados *VetorDeCustosParaCaixa0 = new Dados[caixas->at(0).size()];
		list<Vertice*>::iterator itV = caixas->at(0).begin();
		/*
		Percorrer a caixa de grau de saida 0 para armazenar os custos
		dos elementos atualmente na caixa com rela��o ao �ltimo
		posto na extens�o linear. Caso seja a primeira vez todos os custos ser�o zero
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
		O valor de alfa dever� estar entre 0.0 e 1.0. O if foi modificado para ser acima de 0.99
		devido a Tiberius lembrar algo sobre os tipos de dados e o 1 ser armazenado como quase 1, logo
		para valores maiores que 0.99 ser� assumido que o valor de aleatoriedade ser� 1.0, ou seja, o 
		algoritmo ser� totalmente aleatorio. 
		Devido ao arredondamento caso o TamanhoDaListaDeMelhoresCandidatos tenha como resultado o valor zero
		ser� adicionado um unidade ficando ent�o com o tamanho igual a 1.
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
		As fun��es make_heap est�o com o construtor no formato begin e end de cada vector para torna-lo
		uma heap.
		http://www.cplusplus.com/reference/algorithm/make_heap/
		*/
		make_heap(HeapComTodosOsElementosDaCaixaZero.begin(),HeapComTodosOsElementosDaCaixaZero.end());
		//make_heap(ListaRestritaDeCandidatos.begin(),ListaRestritaDeCandidatos.end());

		/*
		O for abaixo pecorre a lista de todos os candidatos representado pela HeapComTodosOsElementosDaCaixaZero
		Retirando da heap o seu topo e colocando na lista de candidatos restritos.

		A fun��o pop_heap rearranja os elementos na heap colocando o elemento a ser "retirado" na ultima posi��o
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
		a variavel chamada aleatorio ser� uma variavel de controle para aleatoriedade da solu��o gerada,
		caso o parametro passado for verdadeiro ir� utilizar a fun��o advance em conjunto com a fun��o
		rand para "pular" para o candidato escolhido.

		Os parametros passados para advance ser�o a ListaDeCandidatosRestrita e a posi��o de casas a serem "avan�adas"

		http://www.cplusplus.com/reference/iterator/advance/

		Ap�s isso ser� feita uma busca utilizando um segundo iterador para localizar a posi��o escolhida
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
		suas dependencias est�o
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
		Atualiza a extens�o e retira o vertice de grau zero
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
A busca local est� implementada na forma de uma lista circular parando apenas quando a �ltima
posi��o atulizada coincidir com a posi��o atual
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
				cout << "ERRO Busca local gerou solu��o inv�lida"<< endl;
}

double Grasp(Poset p,float alfa,int qtdRodadas,bool buscaLocal,bool aleatorio, int* solucao){
	
	double custoTotal = DBL_MAX;
	list<Vertice*> *ExtensaoLinear = new list<Vertice*>;

	for(int i = 0 ; i < qtdRodadas ; i++ ){
		//Chamada a ordena��o Topologica(Fase construtiva)
		ConstruirSolucao( p, alfa, aleatorio, ExtensaoLinear);

		if( p.auditarExtLinear( ExtensaoLinear ) == false )
			cout << "ERRO gera��o de solu��o inv�lida"<< endl;

		if( buscaLocal == true )
			BuscaLocal( p, ExtensaoLinear );
		
		double custo = p.ContabilizarExtLinear( *ExtensaoLinear );
		if( custoTotal > custo ){
			custoTotal = custo;

			int indice = 0;
			for(auto  iterator = ExtensaoLinear->begin(); iterator !=  ExtensaoLinear->end(); iterator++)
				solucao[indice++] =  (*iterator)->getIndice()+1;
		}

		//Limpa a extens�o para uma nova intera��o
		ExtensaoLinear->clear();
	}
	return custoTotal;
}
