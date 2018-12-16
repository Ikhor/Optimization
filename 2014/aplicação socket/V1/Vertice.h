/**********************************************************************************
// Vertice (Arquivo de Cabe�alho)
// 
// Cria��o:		22 Abr 2013
// Atualiza��o:	24 Abr 2013
// Compilador:	Visual C++ 2010
//
// Descri��o:	Define uma classe para gerenciar o conteudo do vertice.
//				
//				Um Vertice � respons�vel por armazenar dados da estrutura
//				como custos e depend�ncias.
//              
//				Atualiza��o 24 Abr 2013
//					Adicionado campo para salvar a posi��o do vertice na extens�o linear
//
**********************************************************************************/

#ifndef _VERTICE_H_
#define _VERTICE_H_

// ---------------------------------------------------------------------------------
// Inclus�es
#include "Arcos.h"

#include <list>
#include <vector>

using std::vector;							// acessa vector sem precisar do std::
using std::list;							// acessa list sem precisar do std::

// ---------------------------------------------------------------------------------

class Vertice {

private:
	int Indice;								//Identificador do vertice Este n�mero deve ser �nico
	int GrauDeEntrada;						//Quantidade de arcos de entrada no vertice
	int GrauDeSaida;						//Quantidade de arcos de saida no vertice
	list<Arco*> *ListaDeSaida;				//Lista de arcos de saida do vertice
	list<Arco*> *ListaDeEntrada;			//Lista de arcos de entrada do vertice
	vector<double> *Custo;					//Vetor de com os custos referentes ao demais vertices
											/*
											Indica��o dos valores e rela��es

											Caso seja comparavel existem duas sa�das 
											Caso 1: 
												O custo ser� 0 se ele precede o v�rtice de indice i ou algum dos seus sucessores
											(transitivade).
											
											Obs: Neste caso o vector NoDownset na mesma posi��o estar� como true para diferenciar do caso 3

											Caso 2:
												O custo ser� INT_MAX se o a rela��o for imposs�vel
												(antes o valor era DBL_MAX mas foi trocado com  o intuito de em c�digo que antes de verificar 
												somem o resutado o o valor n�o "resete" e acabe tendo um valor negativo

											Caso 3:
												Os dois elementos s�o incomparaveis logo existir� um custo entre eles.
												
											*/
	vector<bool> *NoDownset;				//Vetor de indica��o se existe uma rela��o de dependencia e transtividade entre dois v�rtices.

public:

	int posicao;							//Armazena a posi��o do vertice na extens�o linear

    Vertice(int i, int totalDeNos){
		Indice = i;
		GrauDeEntrada = 0;
		GrauDeSaida = 0;
		posicao = -1;
		ListaDeSaida = new list<Arco*>;
		ListaDeEntrada = new list<Arco*>;
		Custo = new vector<double>(totalDeNos);
		NoDownset = new vector<bool>(totalDeNos);
		for (int i=0; i<totalDeNos; i++) {
			Custo->at(i) = INT_MAX;
			NoDownset->at(i) = false;
		}
	}

	void adicionarCusto(Vertice* vertice, double custo){
		Custo->at(vertice->getIndice()) = custo;
		NoDownset->at(vertice->getIndice()) = false;
	}

	inline void adicionarRelacao(Vertice* vertice){
		NoDownset->at(vertice->getIndice()) = true;
		Custo->at(vertice->getIndice()) = 0.0;
	}

	inline void adicionarRelacao(int v){
		NoDownset->at(v) = true;
		Custo->at(v) = 0.0;
	}

	/*
	* Se o parametro v for comparavel com o vertice "this" esta funcao retorna true".
	* O retorno "false" so acontece se "this" estiver no downset de v.
	*/
	bool obterCustoParaVertice(int v, double &custo) {
		if (NoDownset->at(v)) {
			custo = 0.0;
			return true;
		}
		else {
			custo = Custo->at(v);
			if (custo < INT_MAX - 1.0)
				return true;
			else // "this" esta' no downset de "v"
				return false;
		}
	}

	/* 
		As fun��es abaixo s�o apenas de Get and Set
	*/

	inline bool estaNoDownset(int v) {
		return NoDownset->at(v);
	}

	void adicionarArcoDeSaida(Arco *a) {
         ListaDeSaida->push_back(a);
		 adicionarRelacao(a->getFinal());
	}

	void adicionarArcoDeSaida(Arco *a, double custo) {
		 adicionarCusto(a->getFinal(),custo);
	}

	void adicionarArcoDeEntrada(Arco *a) {
         ListaDeEntrada->push_back(a);
	}

	void removerArcoDeSaida(Arco *a) {
		ListaDeSaida->remove(a);
	}

	int  getGrauDeEntrada() {
		return GrauDeEntrada;
	}

	void incrementarGrauDeEntrada() {
		GrauDeEntrada ++;
	}

	void decrementarGrauDeEntrada() {
		GrauDeEntrada --;
	}

	int  getGrauDeSaida() {
		return GrauDeSaida;
	}

	void incrementarGrauDeSaida() {
		GrauDeSaida ++;
	}

	void decrementarGrauDeSaida() {
		GrauDeSaida --;
	}

	int  getIndice() {
		return Indice;
	}

	list<Arco*>* getListaDeSaida() {
		return ListaDeSaida;
	}

	list<Arco*>* getListaDeEntrada() {
		return ListaDeEntrada;
	}
};

// ---------------------------------------------------------------------------------

#endif