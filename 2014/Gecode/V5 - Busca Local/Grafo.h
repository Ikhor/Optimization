/**********************************************************************************
// Grafo (Arquivo de Cabe�alho)
// 
// Cria��o:		22 Abr 2013
// Atualiza��o:	24 Abr 2013
// Compilador:	Visual C++ 2010
//
// Descri��o:	Define uma classe para gerenciar vertices e arcos.
//				
//				Um Grafo � respons�vel por gerenciar e organizar dados da estrutura
//				como depend�ncias(Arcos) e dados(Vertices).
//              
//				Atualiza��o 24 Abr 2013
//					Adicionada fun��o para salvar em arquivo as posi��es de cada vertice na extens�o
//
**********************************************************************************/
#define _GRAFO_H_

// ---------------------------------------------------------------------------------
// Inclus�es
#include "Vertice.h"
#include <fstream>

using namespace std;

// ---------------------------------------------------------------------------------

class GrafoDirecionado {

private:

	vector<Vertice*> *Vertices;

public:
	GrafoDirecionado(int numVertices){
		Vertices = new vector<Vertice*>(numVertices);
		for (int i = 0; i < numVertices; i++) {
			Vertices->at(i) = new Vertice(i, numVertices);
		}
	}

	int getNumeroDeVertices() {
		return Vertices->size();
	}

	Vertice* getVertice(int i) {
		if (i < getNumeroDeVertices())
			return Vertices->at(i);
		return NULL;
	}

	Arco* adicionarArco(int origem, int destino) {
		int tamanho = getNumeroDeVertices();
		if ((origem < tamanho) && (destino < tamanho)) {
			Vertice *verticeOrigem = getVertice(origem);
			Vertice *verticeDestino = getVertice(destino);
			if ((verticeOrigem != NULL) && (verticeDestino != NULL)) {
				for(list<Arco*>::iterator it = verticeDestino->getListaDeSaida()->begin(); it != verticeDestino->getListaDeSaida()->end();it++)
					if((*it)->getInicio() == verticeOrigem)
						return NULL;
				Arco *arco = new Arco(verticeOrigem , verticeDestino);
				verticeOrigem->adicionarArcoDeSaida(arco);
				verticeOrigem->incrementarGrauDeSaida();
				verticeDestino->adicionarArcoDeEntrada(arco);
				verticeDestino->incrementarGrauDeEntrada();
				return arco;
			}
		}
		return NULL;
	}

	// Adiciona apenas os arcos de custos (serao invisiveis ao Grafo original)
	Arco* adicionarArcoDeCusto(int origem, int destino, double custo) {
		int tamanho = getNumeroDeVertices();
		if ((origem < tamanho) && (destino < tamanho)) {
			Vertice *verticeOrigem = getVertice(origem);
			Vertice *verticeDestino = getVertice(destino);
			if ((verticeOrigem != NULL) && (verticeDestino != NULL)) {
				for(list<Arco*>::iterator it = verticeDestino->getListaDeSaida()->begin(); it != verticeDestino->getListaDeSaida()->end();it++)
					if((*it)->getInicio() == verticeOrigem)
						return NULL;
				Arco *arco = new Arco(verticeOrigem , verticeDestino);
				verticeOrigem->adicionarArcoDeSaida(arco, custo);
				return arco;
			}
		}
		return NULL;
	}

	void removerArco(Arco *arco) {
		Vertice *fim = arco->getFinal();
		fim->removerArcoDeSaida(arco);
		Vertice *inicio = arco->getInicio();
		inicio->decrementarGrauDeEntrada();
		fim->decrementarGrauDeSaida();
	}

	vector<Vertice*>* getVertices(){
		return Vertices;
	}

	void ArquivoDeSaidaPosicoes(){
		ofstream ArqSaida;
		ArqSaida.open("Posicao.txt", ios_base::out | ios_base::app);
		for(auto iter = getVertices()->begin(); iter != getVertices()->end(); iter++)
			ArqSaida << (*iter)->posicao << "\t";
		ArqSaida << endl;
		ArqSaida.close();
	}
};


