/**********************************************************************************
// GRASP
// 
// Cria��o:	22 Abr 2013
// Atualiza��o:	28 Mai 2013
// Compilador:	Visual C++ 2010
//
// Descri��o:	Define m�todos para GRASP E busca local
//				
//				Os parametros a serem passados para o execut�vel s�o
//				o nome do arquivo,o valor Alfa(% dos melhores - 0 para uma solu��o gulosa
//				1 para solu��es aleat�rias), quantidade de vezes rodadas o GRASP
//
//				O arquivo texto deve estar no seguinte formato:
//				Quantidade de Vertice
//				Quantidade de Arcos SEM custo custo(Rela��es do tipo X Precede Y)
//				Quantidade de Arcos COM custo custo(Rela��es do tipo X incomparav�l Y)
//				Arcos SEM custo de X para y 
//				Arcos com Custo de X para Y e Y para X
//
//				Atualiza��o 24 Abr 2013
//					Adicionada linha para salvar posi��o do vertice na extens�o linear
//
**********************************************************************************/

// ---------------------------------------------------------------------------------
// Inclus�es
#include <iostream>
#include <time.h>
#include <algorithm>
#include "Poset.h"
#include "RegistroOT.h"
#include "BuscaLocal.h"
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

	bool compare(Infos A,Infos B){
		if(A.getCustoCRAUltimoElemento() < B.getCustoCRAUltimoElemento())
			return true;
		return false;
	}

	list<Vertice*>* GRASP(Poset &p,float valorAlfa){

		list<Vertice*> *ExtLinear = new list<Vertice*>();

		//Descobre o maior grau
		int maiorGrau = -1;

		
		for(vector<Vertice*>::iterator it = p.getVertices()->begin()++;it != p.getVertices()->end();it++){
			if((*it)->getGrauDeSaida() > maiorGrau)
				maiorGrau = (*it)->getGrauDeSaida();
		}
		

		//Preenche as "caixas"
		int *NaCaixa = new int[p.getNumeroDeVertices()];
		vector<list<Vertice*>> *caixas = new vector<list<Vertice*>>(maiorGrau+1);

		//Vector de iteradores
	    list<Vertice*>::iterator *CaixaIterator = new list<Vertice*>::iterator[p.getNumeroDeVertices()];

		for(vector<Vertice*>::iterator it = p.getVertices()->begin()++;it != p.getVertices()->end();it++) {
			int indiceDaCaixa = (*it)->getGrauDeSaida();
			caixas->at(indiceDaCaixa).push_back((*it));
			NaCaixa[(*it)->getIndice()] = indiceDaCaixa;

			//Parte Iterator
			auto itAuxiliarParaCapturarFinal = caixas->at(indiceDaCaixa).end();
			CaixaIterator[(*it)->getIndice()] = --itAuxiliarParaCapturarFinal;
		}

		

		//Ordena��o topologica por caixas
		bool PrimeiraSelecao = true;
		while(caixas->at(0).size() > 0) {
			//Sele��o dos melhores
			Infos *myInfos = new Infos[caixas->at(0).size()];
			list<Vertice*>::iterator itV = caixas->at(0).begin();
			for(unsigned int i = 0; i < caixas->at(0).size(); i++){
				myInfos[i].setIndiceVertice((*itV)->getIndice());
				if(PrimeiraSelecao == true){
					myInfos[i].setCustoCRAUltimoElemento(0);
				}else{
					double custo;
					(*itV)->obterCustoParaVertice(ExtLinear->front()->getIndice(),custo);
					myInfos[i].setCustoCRAUltimoElemento(custo);
				}
				itV++;
			}
			PrimeiraSelecao = false;
			vector<Infos> HeapCompleta(myInfos,myInfos+caixas->at(0).size());
			int Alfa;
			if(valorAlfa == 1)
				Alfa = HeapCompleta.size();
			else{
				Alfa = int(HeapCompleta.size()*valorAlfa);
				Alfa++;//Adicionado +1 para garantir que haver� pelo menos 1 elemento na heap dos melhores
			}

			Infos *Melhores = new Infos[Alfa];
			vector<Infos> Escolha(Melhores,Melhores);

			make_heap(HeapCompleta.begin(),HeapCompleta.end(),compare);
			make_heap(Escolha.begin(),Escolha.end(),compare);
			int i = 0;
			while(i < Alfa){
				pop_heap (HeapCompleta.begin(),HeapCompleta.end(),compare);
				Escolha.push_back(HeapCompleta.back());
				push_heap (Escolha.begin(),Escolha.end(),compare);
				HeapCompleta.pop_back();
				i++;
			}

			while(HeapCompleta.size() > 0){
				if(HeapCompleta.front().getCustoCRAUltimoElemento() < Escolha.front().getCustoCRAUltimoElemento()){
					pop_heap (HeapCompleta.begin(),HeapCompleta.end(),compare);
					Escolha.push_back(HeapCompleta.back());
					push_heap (Escolha.begin(),Escolha.end(),compare);
					HeapCompleta.pop_back();
					pop_heap (Escolha.begin(),Escolha.end(),compare);
					Escolha.pop_back();
				}
				else{
					pop_heap(HeapCompleta.begin(),HeapCompleta.end(),compare);
					HeapCompleta.pop_back();
				}
			}

			//Parte Aleatorio
			vector<Infos>::iterator SelecionarAleatorioMelhores = Escolha.begin();
			advance(SelecionarAleatorioMelhores,rand()%Escolha.size());
			list<Vertice*>::iterator it2;//BuscaVertice selecionado aleatorio
			Vertice * verticeGrauZero;
			for(it2 = caixas->at(0).begin(); it2 != caixas->at(0).end(); it2++)
				if((*it2)->getIndice() == (*SelecionarAleatorioMelhores).getindiceVertice()){
					verticeGrauZero = (*it2);
					break;
				}

				list<Arco*>::iterator it = verticeGrauZero->getListaDeEntrada()->begin();
				for ( ;it != verticeGrauZero->getListaDeEntrada()->end(); it++) {
					int ondeInicioEsta = NaCaixa[(*it)->getInicio()->getIndice()];
					
					//remo��o
					caixas->at(ondeInicioEsta).erase(CaixaIterator[(*it)->getInicio()->getIndice()]);
					
					caixas->at(ondeInicioEsta-1).push_front((*it)->getInicio());
					NaCaixa[(*it)->getInicio()->getIndice()] = ondeInicioEsta-1;

					//Atualiza��o do Iterador
					CaixaIterator[(*it)->getInicio()->getIndice()] = caixas->at(ondeInicioEsta-1).begin();
				}

				ExtLinear->push_front(verticeGrauZero);
				caixas->at(0).remove(verticeGrauZero);
				verticeGrauZero->posicao = p.getNumeroDeVertices() + 1 - ExtLinear->size();
				delete myInfos;
				delete Melhores;
		}

		delete NaCaixa;
		delete caixas;
		return ExtLinear;
	}


double GRASPBL(Poset p,float alfa,int qtdRodadas){
	int i = 0;
	double custoTotal = DBL_MAX;
	while(i < qtdRodadas){
		//Chamada ao GRASP
		list<Vertice*> *ext = GRASP(p,alfa);
		double	custo = p.ContabilizarExtLinear(*ext);

		if(p.auditarExtLinear(ext) == false)
			cout << "ERRO gera��o de solu��o inv�lida"<< endl;

		//cout << "\nCusto Antes: " << custo << endl;
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
				if(p.melhorMovimento(ext,posicao,indiceDown,indiceUp)){
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

			
		if(p.auditarExtLinear(ext) == false){
			cout << "ERRO Busca local gerou solu��o inv�lida"<< endl;
		}else{
			if(custoTotal > p.ContabilizarExtLinear(*ext))
				custoTotal = p.ContabilizarExtLinear(*ext);
			//cout << " % de melhoria =  "<< custoTotal/custo << endl;
		}	
		
		i++;
		//cout << "Rodada I: " << i << endl;
	}

	return custoTotal;
}


int main(int argc, char* argv[]) {

	char* nomeArquivo = new char[1+strlen(argv[1])];
	strcpy(nomeArquivo, argv[1]);

	srand((unsigned)time(NULL));

	Poset *p = construirPoset(nomeArquivo);

	float alfa = atof(argv[2]);

	int qtdRodadas = atoi(argv[3]);

	clock_t ini = clock();
	double custos = GRASPBL(*p,alfa,qtdRodadas);
	clock_t fim = clock();
	double tempo = double(fim-ini)/CLK_TCK;
	
	//cout << endl << custos << endl;
	cout << nomeArquivo  << "," << custos << "," << tempo << endl;
	//system("pause");
}