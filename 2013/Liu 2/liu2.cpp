/**********************************************************************************
// Liu2
//
// Criação:		22 Abr 2013
// Atualização:	24 Abr 2013
// Compilador:	Visual C++ 2010
//
// Descrição:	Define métodos para o algoritmo de Liu 2
//
//				Os parametros a serem passados para o executável é
//				o nome do arquivo
//
//				O arquivo texto deve estar no seguinte formato:
//				Quantidade de Vertice
//				Quantidade de Arcos SEM custo custo(Relações do tipo X Precede Y)
//				Quantidade de Arcos COM custo custo(Relações do tipo X incomparavél Y)
//				Arcos SEM custo de X para y
//				Arcos com Custo de X para Y e Y para X
//
//
**********************************************************************************/

// ---------------------------------------------------------------------------------
// Inclusões
#include <iostream>
#include <time.h>
#include <algorithm>
#include "Poset.h"

// ---------------------------------------------------------------------------------

using namespace std;


list<Vertice*>* Step3(Poset &p,list<Vertice*> *extensao){

		list<Vertice*> verticesNaoIncluidos;
		list<Vertice*>::iterator iterVertice;
		list<Vertice*>::iterator posicaoDeInsercao;
		double custoTotal = 0;
		bool buscaInterrompida;

		// Popular lista de vertices que ainda nao pretencem aa extensao
		for (int i = 0 ; i < p.getNumeroDeVertices() ; i++)
			verticesNaoIncluidos.push_back(p.getVertice(i));
		for (iterVertice = extensao->begin() ; iterVertice != extensao->end() ; iterVertice++)
			verticesNaoIncluidos.remove(*iterVertice);

		// Incluir cada vertice na extensao, seguindo a ordem da lista
		while (verticesNaoIncluidos.size() > 0) {
			Vertice *novo = verticesNaoIncluidos.front();
			verticesNaoIncluidos.pop_front();

			double custoNovoVertice = DBL_MAX, custoNaPosicao;
			buscaInterrompida = false;
			for (iterVertice = extensao->begin() ; iterVertice != extensao->end() ; iterVertice++) {

				// Verificar insercao do novo vertice no inicio da extensao parcial
				if (iterVertice == extensao->begin()) {
					if (novo->obterCustoParaVertice((*iterVertice)->getIndice(), custoNaPosicao)) {
						custoNovoVertice = custoNaPosicao;
						posicaoDeInsercao = extensao->begin();
					}
				}

				// Verificar insercao do novo vertice no "meio" da extensao parcial
				else {
					// Inclui logica de "desistir", caso nao seja possivel inserir "novo" na posicao atual ou subsequente
					double custoComPosterior, custoComAnterior;
					if (novo->obterCustoParaVertice((*iterVertice)->getIndice(), custoComPosterior)) {
						// Obter referencia para vertice anterior na extensao parcial
						bool posteriorNoDownset = novo->estaNoDownset((*iterVertice)->getIndice());
						list<Vertice *>::iterator anterior = iterVertice;
						anterior--;
						if ((*anterior)->obterCustoParaVertice(novo->getIndice(), custoComAnterior)) {
							custoNaPosicao = custoComAnterior + custoComPosterior;
							if (custoNaPosicao < custoNovoVertice){
								custoNovoVertice = custoNaPosicao;
								posicaoDeInsercao = iterVertice;
							}
							if (posteriorNoDownset) { // "novo" nao pode ficar abaixo de "posterior"
								// Precisamos interromper a busca
								buscaInterrompida = true;
								break;
							}
						}
						else { // "novo" nao pode ficar abaixo de "anterior".
							// Precisamos interromper a busca
							buscaInterrompida = true;
							break;
						}
					}
					else { // "novo" deve ficar abaixo de "*iterVertice"
						// Resetar melhor custo visto ate agora (as posicoes anteriores eram todas invalidas)
						custoNovoVertice = DBL_MAX;
					}
				}
			}//for

			// Verificar insercao do novo vertice no final da extensao parcial
			if ((!buscaInterrompida) && (extensao->back()->obterCustoParaVertice(novo->getIndice(), custoNaPosicao) && (custoNaPosicao < custoNovoVertice))) {
				custoNovoVertice = custoNaPosicao;
				posicaoDeInsercao = extensao->end();
			}

			extensao->insert(posicaoDeInsercao, novo);
		}
		return extensao;
	}

list<Vertice*>* Steps1And2(Poset &p){

		list<Vertice*> *ExtLinear = new list<Vertice*>();

		// Descobre o maior grau de saida
		int maiorGrau = -1;
		for(vector<Vertice*>::iterator it = p.getVertices()->begin()++;it != p.getVertices()->end();it++){
			if((*it)->getGrauDeSaida() > maiorGrau)
				maiorGrau = (*it)->getGrauDeSaida();
		}

		// Preenche as "caixas"
		int *NaCaixa = new int[p.getNumeroDeVertices()];
		vector<list<Vertice*>> *caixas = new vector<list<Vertice*>>(maiorGrau+1);
		for(vector<Vertice*>::iterator it = p.getVertices()->begin(); it != p.getVertices()->end(); it++) {
			int indiceDaCaixa = (*it)->getGrauDeSaida();
			caixas->at(indiceDaCaixa).push_back((*it));
			NaCaixa[(*it)->getIndice()] = indiceDaCaixa;
		}

		int *Nivel = new int[p.getNumeroDeVertices()];

		int *Sucessor = new int[p.getNumeroDeVertices()];

		for(list<Vertice*>::iterator VerticecomNivelZero = caixas->at(0).begin(); VerticecomNivelZero  != caixas->at(0).end() ; VerticecomNivelZero++)
		{
			Nivel[(*VerticecomNivelZero)->getIndice()] = 0;
			Sucessor[(*VerticecomNivelZero)->getIndice()] = -1;
		}


		// Ordenação topologica por caixas
		while(caixas->at(0).size() > 0) {
			Vertice * verticeGrauZero = caixas->at(0).front();
			for (list<Arco*>::iterator it = verticeGrauZero->getListaDeEntrada()->begin();it != verticeGrauZero->getListaDeEntrada()->end(); it++) {

					int ondeInicioEsta = NaCaixa[(*it)->getInicio()->getIndice()];
					caixas->at(ondeInicioEsta).remove((*it)->getInicio());
					caixas->at(ondeInicioEsta-1).push_back((*it)->getInicio());
					NaCaixa[(*it)->getInicio()->getIndice()] = ondeInicioEsta-1;

					Nivel[(*it)->getInicio()->getIndice()] = Nivel[caixas->at(0).front()->getIndice()]+1;

					Sucessor[(*it)->getInicio()->getIndice()] = caixas->at(0).front()->getIndice();

					//Copia os transitivos
					for(int i = 0; i < p.getNumeroDeVertices(); i++)
						if(verticeGrauZero->estaNoDownset(i))
							(*it)->getInicio()->adicionarRelacao(i);
			}
			caixas->at(0).pop_front();
		}

		int MaiorNivel = -2;
		int indiceMaiorNivel;
		for(int i = 0; i <  p.getNumeroDeVertices(); i++){
			if(Nivel[i] > MaiorNivel){
				indiceMaiorNivel = i;
				MaiorNivel = Nivel[i];
			}
		}

		ExtLinear->push_back(p.getVertice(indiceMaiorNivel));
		while(Sucessor[indiceMaiorNivel] != -1){
			indiceMaiorNivel = Sucessor[indiceMaiorNivel];
			ExtLinear->push_back(p.getVertice(indiceMaiorNivel));
		}

		return ExtLinear;
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

int AlgoritmoLiu2(Poset p){

	list<Vertice*> *ext = Steps1And2(p);
	Step3(p,ext);
	int	custo = p.ContabilizarExtLinear(*ext);

	if(p.auditarExtLinear(ext) == false)
		cout << "ERRO geração de solução inválida"<< endl;

	return custo;
}

int main(int argc, char* argv[]) {

	char* nomeArquivo = new char[1+strlen(argv[1])];
	strcpy(nomeArquivo, argv[1]);

	Poset *p = construirPoset(nomeArquivo);

	clock_t ini = clock();
	int custos = AlgoritmoLiu2(*p);
	clock_t fim = clock();
	double tempo = double(fim-ini)/CLK_TCK;

	cout << nomeArquivo  << "," << custos << "," << tempo << endl;
}
