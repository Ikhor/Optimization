/**********************************************************************************
// Poset (Arquivo de Cabeçalho)
// 
// Criação:		22 Abr 2013
// Atualização: 06 Mai 2013
// Compilador:	Visual C++ 2010
//
// Descrição:	Define uma classe para gerenciar o conteudo de um grafo.
//				
//				Um poset possui 3 metodos para gerenciar 
//				ExibirCadeia: Mostra o conteudo da cadeia
//              Contabilizar: Soma os custos entre os vertices dando como resultado o valor da extensão
//				Auditoria: Confere se todas as relações de precedências são obedecidas
//				IntervalorUpDownSet:  Retorna para variaveis passadas por referencia o valor do intervalo de upset e downset(texto explicativo antes da função)
//
**********************************************************************************/
#define _POSET_H_

// ---------------------------------------------------------------------------------
// Inclusões

#include <fstream>
#include <queue>
#include <iostream>
#include "Grafo.h"
using namespace std;

// ---------------------------------------------------------------------------------

class Poset : public GrafoDirecionado {

public:
	Poset(int numVertices)
		: GrafoDirecionado(numVertices) {}

	// Exibe o conteudo de uma cadeia, representada como uma lista
	void exibirCadeia(list<Vertice*> extlinear){
		for(list<Vertice*>::iterator it = extlinear.begin(); it != extlinear.end(); it++)
			cout << (*it)->getIndice() << " ";
		cout << endl;
	}

	double ContabilizarExtLinear(list<Vertice*> ext){
		double custo = 0, aux;
		list<Vertice*>::iterator it = ext.begin();
		list<Vertice*>::iterator iter = ext.begin();
		for( ; iter != --ext.end(); it++){
			iter ++;
			(*it)->obterCustoParaVertice((*iter)->getIndice(),aux);
			custo += aux;
		}
		return custo;
	}

	bool auditarExtLinear(list<Vertice*> *ext) {
		
		vector<bool> JaInserido(ext->size());
		for(unsigned int i = 0;i < ext->size();i++)
			JaInserido[i] = false;

		list<Vertice*>::iterator itV = ext->begin();
		while (itV!= ext->end()) {
			JaInserido[(*itV)->getIndice()] = true;
			//Para cada um da lista de chegada verificar se ja foi inserido caso não retorna falso
			if ((*itV)->getListaDeEntrada()->size() != 0){
				list<Arco*>::iterator itArco = (*itV)->getListaDeEntrada()->begin();
				while (itArco != (*itV)->getListaDeEntrada()->end()) {
					if (JaInserido[(*itArco)->getInicio()->getIndice()] == false) {
						cout << "ERRO: " << (*itV)->getIndice() << " aparece antes de " << (*itArco)->getInicio()->getIndice() << endl;
						return false;
					}
					itArco++;
				}
			}
			itV++;
		}
		return true;
	}

	/*
	(Upset e downset):

	Os valores dos indices são armazenados com o min/max visto que:

		1 - O downset for zero significa que ele é um elemento maximal de alguma cadeia;

		2 - O upset for o número total de vertices isto significa que ele é o
		elemento minimal de alguma cadeia.

	Caso isso não aconteça como o vertice agora possui um novo campo chamado POSIÇÃO (que 
	foi incluido para exportação dos dados para gerar o gráfico de soluções) ele vai ser útil
	para capturar o intervalo pois irá necessitar apenas verificar a lista de entrada e saida
	de cada vertice para determinar o intervalo.

	*/
	void IntervaloDownUpSet(int indiceVertice,int &indiceDownSet,int &indiceUpSet){
		
		if(getVertice(indiceVertice)->getGrauDeSaida() == 0)
			indiceDownSet = getNumeroDeVertices();
		else{
			indiceDownSet = getNumeroDeVertices()+2;
			for(auto itListaSaida = getVertice(indiceVertice)->getListaDeSaida()->begin();
				itListaSaida != getVertice(indiceVertice)->getListaDeSaida()->end();
				itListaSaida++){
					if((*itListaSaida)->getFinal()->posicao < indiceDownSet)
						indiceDownSet = (*itListaSaida)->getFinal()->posicao;
			}
		}

		if(getVertice(indiceVertice)->getGrauDeEntrada() == 0)
			indiceUpSet = 1;
		else{
			indiceUpSet = -1;
			for(auto itListaEntrada = getVertice(indiceVertice)->getListaDeEntrada()->begin();
				itListaEntrada != getVertice(indiceVertice)->getListaDeEntrada()->end();
				itListaEntrada++){
					if((*itListaEntrada)->getInicio()->posicao > indiceUpSet)
						indiceUpSet = (*itListaEntrada)->getInicio()->posicao; 
			}
		}
	}

	bool melhorMovimento(list<Vertice*> *extlinear,int indiceVertice,int indiceDownSet,int indiceUpSet){
		
		if(indiceDownSet-indiceUpSet > 2){

			auto IteratorVertices = new list<Vertice*>::iterator[getNumeroDeVertices()];

			for(auto it = extlinear->begin(); it != extlinear->end(); it++)
				IteratorVertices[(*it)->getIndice()] = it;

			/*
			A variavel indiceMelhora salva o indice do vertice seguinte da extensão onde houve
			melhora por exemplo temos a cadeia abaixo:
				A -> X -> B
			a variavel indiceMelhora irá salvar o indice correspondente ao vertice B
			*/
			int IndiceMelhora = -1;
			double CustoMelhora = 0.0;

			auto iteratorVertice = extlinear->begin();

			while((*iteratorVertice)->getIndice() != indiceVertice){
				iteratorVertice++;
			}

			double CustoOriginal = 0;
			double CustoModificado = 0;
			double custotemp = 0;
			auto iteratorAuxiliar = iteratorVertice;

			/*
				Custo Original

				No fragmento abaixo desejamos calcular os valores do custo atual da extensão para o elemnento
				que está sendo avaliado para isso obteremos os custos abaixos onde C(A,B) representa o custo
				do arco de A para B

				- C(A,X) - C(X,B) + C(A,B)

				Com a formular acima obtemos o custo de por B após A na extensão e retirar o elemento X da extensão

				excerto pelos casos em que o elemento em questão está no ínicio ou no fim da extensão
				no qual a descrição será:

				-C(A,X) no caso em que o elemento está no fim da extensão
				-C(X,A) no caso em que o elemento está no inicio da extensão
			*/
			if(getVertice(indiceVertice)->posicao != 1 && getVertice(indiceVertice)->posicao != getNumeroDeVertices()){
				//Custo A->X
				iteratorAuxiliar--;//iteratorAuxiliar passa a ser o predecessor na extensão do vertice avalidado
				(*iteratorAuxiliar)->obterCustoParaVertice((*iteratorVertice)->getIndice(),custotemp);
				CustoOriginal -= custotemp;
				iteratorAuxiliar++;//iteratorAuxiliar volta a ser iteratorVertice

				//Custo X->B
				iteratorAuxiliar++;//iteratorAuxiliar passa a ser o sucessor na extensão do vertice avalidado
				(*iteratorVertice)->obterCustoParaVertice((*iteratorAuxiliar)->getIndice(),custotemp);
				CustoOriginal -= custotemp;
				iteratorVertice--;//Decrementa o a posicao do vertice avaliado para pegar o seu predecessor

				//Custo A->B
				(*iteratorVertice)->obterCustoParaVertice((*iteratorAuxiliar)->getIndice(),custotemp);
				CustoOriginal += custotemp;
			}
			else if(getVertice(indiceVertice)->posicao == 1){
				iteratorAuxiliar++;//Vai para o vertice na segunda posição
				(*iteratorVertice)->obterCustoParaVertice((*iteratorAuxiliar)->getIndice(),custotemp);
				CustoOriginal -= custotemp;
			}
			else{
				iteratorAuxiliar--;//Vai para o vertice na penúltima posição
				(*iteratorAuxiliar)->obterCustoParaVertice(indiceVertice,custotemp);
				CustoOriginal -= custotemp;
			}

			/*
			Custos Modificados 
			No fragmento abaixo desejamos calcular os valores "de onde o vertice ele vai parar"
			descrito por:
				- C(E,F) + C(E,X) + C(X,F)
			*/
		
			iteratorVertice = extlinear->begin();
			for(int i = 1;i < indiceUpSet; i++)
				iteratorVertice++;

			iteratorAuxiliar = iteratorVertice;
			iteratorAuxiliar++;

			if(indiceUpSet == 1){
				CustoModificado = 0;
				getVertice(indiceVertice)->obterCustoParaVertice(extlinear->front()->getIndice(),custotemp);
				CustoModificado += custotemp;

				//cout << indiceVertice << " " << extlinear->front()->getIndice() << endl;

				//cout << "Custo original: " << CustoOriginal << " Custo Modificado: " << CustoModificado << endl;

				if(CustoOriginal + CustoModificado < 0) {
					IndiceMelhora = extlinear->front()->getIndice();
					CustoMelhora = CustoOriginal + CustoModificado;			
				}
			}

			for(int i = indiceUpSet;i < indiceDownSet; i++){

				CustoModificado = 0;

				if((*iteratorVertice)->getIndice() !=  indiceVertice && (*iteratorAuxiliar)->getIndice() !=  indiceVertice)
				{
					//-C(E,F)
					(*iteratorVertice)->obterCustoParaVertice((*iteratorAuxiliar)->getIndice(),custotemp);
					CustoModificado -= custotemp;

					//C(E,X)
					(*iteratorVertice)->obterCustoParaVertice(indiceVertice,custotemp);
					CustoModificado += custotemp;

					//C(X,F)
					getVertice(indiceVertice)->obterCustoParaVertice((*iteratorAuxiliar)->getIndice(),custotemp);
					CustoModificado += custotemp;

					//cout << (*iteratorVertice)->getIndice() << " " << (*iteratorAuxiliar)->getIndice() << endl;
					//cout << "Custo original: " << CustoOriginal << " Custo Modificado: " << CustoModificado << endl;

					if(CustoOriginal + CustoModificado < 0) {
						if (CustoOriginal + CustoModificado < CustoMelhora) {
							IndiceMelhora = (*iteratorAuxiliar)->getIndice();
							CustoMelhora = CustoOriginal + CustoModificado;
						}
					}
				}
			
				iteratorVertice++;
				iteratorAuxiliar++;
			}

			if(indiceDownSet == getNumeroDeVertices()){
				CustoModificado = 0;
				extlinear->back()->obterCustoParaVertice(indiceVertice,custotemp);
				CustoModificado += custotemp;

				//cout << extlinear->back()->getIndice() << " " << indiceVertice << endl;

				//cout << "Custo original: " << CustoOriginal << " Custo Modificado: " << CustoModificado << endl;

				if(CustoOriginal + CustoModificado < 0) {
					if (CustoOriginal + CustoModificado < CustoMelhora) {
						IndiceMelhora = extlinear->back()->getIndice();
						CustoMelhora = CustoOriginal + CustoModificado;
					}
				}
			}

			if(IndiceMelhora != -1){
				extlinear->erase(IteratorVertices[indiceVertice]);
				extlinear->insert(IteratorVertices[IndiceMelhora],getVertice(indiceVertice));
				auto it = extlinear->begin();
				for(int i = 0; i < getNumeroDeVertices(); i++){
					(*it)->posicao = i+1;
					it++;
				}
				return true;
			}
		}
		return false;
	}

	// Retorna tamanho do downset de um elemento
	// (Observacao: o elemento propriamente dito e' contabilizado.)
	int tamanhoDownset(int vi) {
		int retorno = 0;
		Vertice * v = getVertice( vi );

		bool *alcancado = new bool[ getNumeroDeVertices() ];
		for (int c=0; c<getNumeroDeVertices(); c++)
			alcancado[c] = false;

		queue<Vertice*> F;
		F.push( v );
		while (F.size() > 0) {
			Vertice *w = F.front(); F.pop();
			++ retorno;

			for (auto arco = w->getListaDeSaida()->begin(); arco != w->getListaDeSaida()->end(); arco++) {
				Vertice *viz = (*arco)->getFinal();
				if (! alcancado[ viz->getIndice() ]) {
					alcancado[ viz->getIndice() ] = true;
					F.push( viz );
				}
			}
		}

		delete alcancado;
		return retorno;
	}

	// Retorna tamanho do upset de um elemento
	// (Observacao: o elemento propriamente dito e' contabilizado.)
	int tamanhoUpset(int vi) {
		int retorno = 0;
		Vertice * v = getVertice( vi );

		bool *alcancado = new bool[ getNumeroDeVertices() ];
		for (int c=0; c<getNumeroDeVertices(); c++)
			alcancado[c] = false;

		queue<Vertice*> F;
		F.push( v );
		while (F.size() > 0) {
			Vertice *w = F.front(); F.pop();
			++ retorno;

			for (auto arco = w->getListaDeEntrada()->begin(); arco != w->getListaDeEntrada()->end(); arco++) {
				Vertice *viz = (*arco)->getInicio();
				if (! alcancado[ viz->getIndice() ]) {
					alcancado[ viz->getIndice() ] = true;
					F.push( viz );
				}
			}
		}

		delete alcancado;
		return retorno;
	}

};