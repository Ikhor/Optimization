/**********************************************************************************
// GRASP
// 
// Criação:	22 Abr 2013
// Atualização:	23 Set 2013
// Compilador:	Visual C++ 2010
//
// Descrição:	Define métodos para GRASP E busca local
//				
//				Os parametros a serem passados para o executável são
//				o nome do arquivo,o valor Alfa(% dos melhores - 0 para uma solução gulosa
//				1 para soluções aleatórias), quantidade de vezes rodadas o GRASP
//
//				O arquivo texto deve estar no seguinte formato:
//				Quantidade de Vertice
//				Quantidade de Arcos SEM custo custo(Relações do tipo X Precede Y)
//				Quantidade de Arcos COM custo custo(Relações do tipo X incomparavél Y)
//				Arcos SEM custo de X para y 
//				Arcos com Custo de X para Y e Y para X
//
//				Atualização 24 Abr 2013
//					Adicionada linha para salvar posição do vertice na extensão linear
//
//				Atualização 23 Set 2013
//					Função de comparação é utililizada apartir de sobrecarga do operador <
//
**********************************************************************************/

// ---------------------------------------------------------------------------------
// Inclusões
#include <iostream>
#include <time.h>
#include <algorithm>
#include "Poset.h"
#include "RegistroOT.h"

// ---------------------------------------------------------------------------------

using namespace std;

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
		PrimeiraSelecao = false;
		vector<Dados> HeapComTodosOsElementosDaCaixaZero(caixas->at(0).size());
		
		for(int i = 0; i < caixas->at(0).size(); i++) 
			HeapComTodosOsElementosDaCaixaZero.at(i) = VetorDeCustosParaCaixa0[i];
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

		vector<Dados> ListaDeCandidatosRestrita(TamanhoDaListaDeMelhoresCandidatos);
		for(int i = 0 ; i < TamanhoDaListaDeMelhoresCandidatos ; i++) 
			ListaDeCandidatosRestrita.at(i) = VetorDeCustosParaCaixa0[i];

		/*
		As funções make_heap estão com o construtor no formato begin e end de cada vector para torna-lo
		uma heap.
		http://www.cplusplus.com/reference/algorithm/make_heap/
		*/
		make_heap(HeapComTodosOsElementosDaCaixaZero.begin(),HeapComTodosOsElementosDaCaixaZero.end());
		make_heap(ListaDeCandidatosRestrita.begin(),ListaDeCandidatosRestrita.end());

		/*
		O for abaixo pecorre a lista de todos os candidatos representado pela HeapComTodosOsElementosDaCaixaZero
		Retirando da heap o seu topo e colocando na lista de candidatos restritos.

		A função pop_heap rearranja os elementos na heap colocando o elemento a ser "retirado" na ultima posição
		do vector. 

		É necessário usa novamente make_heap após a mudança seja com pop_heap ou push_heap para reorganizar 
		os valores

		http://www.cplusplus.com/reference/algorithm/push_heap/
		http://www.cplusplus.com/reference/algorithm/pop_heap/
		*/

		for(unsigned int i = 0;i < caixas->at(0).size() ; i++){

			pop_heap(HeapComTodosOsElementosDaCaixaZero.begin(),HeapComTodosOsElementosDaCaixaZero.end());
			ListaDeCandidatosRestrita.push_back(HeapComTodosOsElementosDaCaixaZero.back());
			HeapComTodosOsElementosDaCaixaZero.pop_back();
			make_heap(HeapComTodosOsElementosDaCaixaZero.begin(),HeapComTodosOsElementosDaCaixaZero.end());
			make_heap(ListaDeCandidatosRestrita.begin(),ListaDeCandidatosRestrita.end());

		}
		/*
		a variavel chamada aleatorio será uma variavel de controle para aleatoriedade da solução gerada,
		caso o parametro passado for verdadeiro irá utilizar a função advance em conjunto com a função
		rand para "pular" para o candidato escolhido.

		Os parametros passados para advance serão a ListaDeCandidatosRestrita e a posição de casas a serem "avançadas"

		http://www.cplusplus.com/reference/iterator/advance/

		Após isso será feita uma busca utilizando um segundo iterador para localizar a posição escolhida
		para localizar o ponteiro para o vertice correto.
		*/

		vector<Dados>::iterator SelecionarAleatorioNaListaDeCandidatosRestrita = ListaDeCandidatosRestrita.begin();
		if(aleatorio == true)
			advance(SelecionarAleatorioNaListaDeCandidatosRestrita,rand()%ListaDeCandidatosRestrita.size());

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
		for (list<Arco*>::iterator it = verticeGrauZero->getListaDeEntrada()->begin();it != verticeGrauZero->getListaDeEntrada()->end(); it++) {

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
		caixas->at(0).remove(verticeGrauZero);
		verticeGrauZero->posicao = p.getNumeroDeVertices() + 1 - ExtLinear->size();
	}

	/*
	Seguindo a regra desreferenciar para NULL antes de usar o delete
	*/
	NaCaixa = NULL;
	caixas = NULL;
	CaixaIterator = NULL;
	delete CaixaIterator;
	delete NaCaixa;
	delete caixas;
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

double Grasp(Poset p,float alfa,int qtdRodadas,bool buscaLocal,bool aleatorio){
	
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
		if( custoTotal > custo )
			custoTotal = custo;

		//Limpa a extensão para uma nova interação
		ExtensaoLinear->clear();
	}
	return custoTotal;
}


int main(int argc, char* argv[]) {

//	char* nomeArquivo = new char[1+strlen(argv[1])];
//	strcpy(nomeArquivo, argv[1]);

	char* nomeArquivo = "RandomPoset_n200_f5.00_s24.txt";

	srand((unsigned)time(NULL));

	Poset *p = construirPoset(nomeArquivo);

	float alfa = 0.1f;//atof(argv[2]);

	int qtdRodadas = 50;//atoi(argv[3]);

	bool buscaLocal = false;
	
	bool aleatorio = false;

	clock_t ini = clock();
	double custos = Grasp(*p,alfa,qtdRodadas,buscaLocal,aleatorio);
	clock_t fim = clock();
	double tempo = double(fim-ini)/CLK_TCK;
	
	cout << endl << custos << endl;
	//cout << nomeArquivo  << "," << custos << "," << tempo << endl;
	system("pause");
}