#include <iostream>
#include <list>
#include <vector>
#include <fstream>
#include <stack>
#include <float.h>
#include <time.h>

using namespace std;

/*
* - 16/04/2012: correcao em Steps1and2().
*				+ if (maiorNivel > nivelMaisProfundo) { // "maiorNivel" era "indiceMaiorNivel"
*					// Atualizar nivel mais profundo ja visto
*					nivelMaisProfundo = maiorNivel; // "maiorNivel" era "indiceMaiorNivel"
*               + Precisava desta inicializacao tambem: int indiceMaiorNivel = i;
*
* - 17/04/2012: correcao em Step3().
*   Ao verificar insercao do novo vertice no "meio" da extensao parcial, podemos decidir por
*   desistir da busca, caso nao seja possivel inserir "novo" na posicao atual ou subsequente, ou
*   apenas resetar o melhor custo encontrado ate agora, caso nos deparemos com um elemento que
*   precisar ficar antes de "novo" (neste caso, nenhuma das posicoes anteriores era valida).
*/

class Vertice;

class Arco {
private:
	class Vertice *U;
    class Vertice *V;

public:
	Arco(Vertice *u, Vertice *v) {
		U = u;
		V = v;
    }

    Arco(Arco *a) {
		U = a->getInicio();
		V = a->getFinal();
    }

	Vertice * getInicio() {
		return U;
	}

	Vertice * getFinal() {
		return V;
	}
};

class Vertice {
private:
	int Indice;
	int GrauDeEntrada;
	int GrauDeSaida;
	list<Arco*> *ListaDeSaida;
	list<Arco*> *ListaDeEntrada;
	vector<double> *Custo;
	vector<bool> *NoDownset;

public:
    Vertice(int i, int totalDeNos){
		Indice = i;
		GrauDeEntrada = 0;
		GrauDeSaida = 0;
		ListaDeSaida = new list<Arco*>;
		ListaDeEntrada = new list<Arco*>;
		Custo = new vector<double>(totalDeNos);
		NoDownset = new vector<bool>(totalDeNos);
		for (int i=0; i<totalDeNos; i++) {
			Custo->at(i) = DBL_MAX;
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
	* Agora esta funcao retorna true se o parametro v for comparavel com o vertice "this".
	* O retorno "false" so acontece se "this" estiver no downset de v.
	*/
	bool obterCustoParaVertice(int v, double &custo) {
		if (NoDownset->at(v)) {
			custo = 0.0;
			return true;
		}
		else {
			custo = Custo->at(v);
			if (custo < DBL_MAX - 1.0)
				return true;
			else // "this" esta' no downset de "v"
				return false;
		}
	}

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

class GrafoDirecionado {
private:
	vector<Vertice*> *Vertices;
	list<Arco*> *Arcos;

public:
	GrafoDirecionado(int numVertices){
		Vertices = new vector<Vertice*>(numVertices);
		for (int i = 0; i < numVertices; i++) {
			Vertices->at(i) = new Vertice(i, numVertices);
		}
		Arcos = new list<Arco*>();
	}

	int getNumeroDeVertices() {
		return Vertices->size();
	}

	int getNumeroDeArcos() {
		return Arcos->size();
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
				Arcos->push_back(arco);
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
				Arcos->push_back(arco);
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
		Arcos->remove(arco);
	}

	list<Arco*>* getArcos(){
		return Arcos;
	}

	vector<Vertice*>* getVertices(){
		return Vertices;
	}

};

class Poset : public GrafoDirecionado {

public:
	Poset(int numVertices)
		: GrafoDirecionado(numVertices) {}

	// Exibe o conteudo de uma cadeia, representada como uma lista
	void exibirCadeia(list<Vertice*> *extlinear){
		for(list<Vertice*>::iterator it = extlinear->begin(); it != extlinear->end(); it++)
			cout << (*it)->getIndice() << " ";
		cout << endl;
	}

	list<Vertice*>* AlgoritmoOrdenacaoTopologica(){

		   list<Vertice*> *ExtLinear = new list<Vertice*>();

		   //Descobre o maior grau
		   int maiorGrau = -1;

		   for(vector<Vertice*>::iterator it = getVertices()->begin()++;it != getVertices()->end();it++){
			   if((*it)->getGrauDeSaida() > maiorGrau)
				   maiorGrau = (*it)->getGrauDeSaida();
		   }

		   //Preenche as "caixas"
		   int *NaCaixa = new int[getNumeroDeVertices()];

		   vector<list<Vertice*>> *caixas = new vector<list<Vertice*>>(maiorGrau+1);

		   for(vector<Vertice*>::iterator it = getVertices()->begin()++;it != getVertices()->end();it++) {
			   int indiceDaCaixa = (*it)->getGrauDeSaida();
			   caixas->at(indiceDaCaixa).push_back((*it));
			   NaCaixa[(*it)->getIndice()] = indiceDaCaixa;
		   }

		   //Ordenação topologica por caixas
		   bool aux = true;

		   while(caixas->at(0).size() > 0) {

			   Vertice * verticeGrauZero = caixas->at(0).front();
			   double custo;
				if(aux == false){
				   double menorCusto = DBL_MAX;//Definir com um valor MAXIMO
				   for(list<Vertice*>::iterator itV = caixas->at(0).begin();itV != caixas->at(0).end() ; itV++)
					   if((*itV)->obterCustoParaVertice(ExtLinear->front()->getIndice(),custo)) {
						   // Verificar o custo da proxima iteracao
						   double menorAdicional = DBL_MAX;
						   // Pode haver vertice na caixa[1] que passar para a caixa[0]
						   list<Arco*>::iterator itViz = (*itV)->getListaDeEntrada()->begin();
						   while (itViz != (*itV)->getListaDeEntrada()->end()) {
							   if (NaCaixa[(*itViz)->getInicio()->getIndice()]==1) {
								   menorAdicional = 0.0;
								   break;
							   }
							   itViz ++;
						   }
						   // Mas, pode ser que o proximo venha da Caixa[0]
						   if (menorAdicional > 0.0) {
							   list<Vertice*>::iterator itC0 = caixas->at(0).begin();
							   double custoProximo = DBL_MAX;
							   while (itC0 != caixas->at(0).end()) {
								   if ((*itC0)->obterCustoParaVertice((*itV)->getIndice(),custoProximo) && (custoProximo < menorAdicional)) {
									   menorAdicional = custoProximo;
								   }
								   itC0 ++;
							   }
						   }
						   custo += menorAdicional;
						   if (custo < menorCusto) {
							   menorCusto = custo;
							   verticeGrauZero = (*itV);
						   }
					   }
			   }

			   aux = false;

			   list<Arco*>::iterator it = verticeGrauZero->getListaDeEntrada()->begin();
			   for ( ;it != verticeGrauZero->getListaDeEntrada()->end(); it++) {

					   int ondeInicioEsta = NaCaixa[(*it)->getInicio()->getIndice()];
					   caixas->at(ondeInicioEsta).remove((*it)->getInicio());
					   caixas->at(ondeInicioEsta-1).push_front((*it)->getInicio());
					   NaCaixa[(*it)->getInicio()->getIndice()] = ondeInicioEsta-1;
					   if(ondeInicioEsta-1 == 0)
						   aux = true;
			   }
			   ExtLinear->push_front(verticeGrauZero);
			   caixas->at(0).remove(verticeGrauZero);
		   }

		   delete NaCaixa;
		   delete caixas;
		   return ExtLinear;
	}


	list<Vertice*>* maiorCadeia(){

		   int *altura = new int[getNumeroDeVertices()];
		   Vertice **sucessor = new Vertice*[getNumeroDeVertices()];

		   //Descobre o maior grau
		   int maiorGrau = -1;

		   for(vector<Vertice*>::iterator it = getVertices()->begin()++;it != getVertices()->end();it++){
			   if((*it)->getGrauDeSaida() > maiorGrau)
				   maiorGrau = (*it)->getGrauDeSaida();
			   altura[(*it)->getIndice()] = 0;
			   sucessor[(*it)->getIndice()] = NULL;
		   }

		   //Preenche as "caixas"

		   int *NaCaixa = new int[getNumeroDeVertices()]; // NaCaixa[i] informa a caixa do vertice i
		   vector<list<Vertice*>> *caixas = new vector<list<Vertice*>>(maiorGrau+1);

		   for(vector<Vertice*>::iterator it = getVertices()->begin()++;it != getVertices()->end();it++) {
			   int indiceDaCaixa = (*it)->getGrauDeSaida();
			   caixas->at(indiceDaCaixa).push_back((*it));
			   NaCaixa[(*it)->getIndice()] = indiceDaCaixa;
		   }

		   //Ordenação topologica por caixas
		   Vertice *inicio;
		   while(caixas->at(0).size() > 0) {
			   Vertice * verticeGrauZero = caixas->at(0).front();
			   list<Arco*>::iterator it = verticeGrauZero->getListaDeEntrada()->begin();
			   for (; it != verticeGrauZero->getListaDeEntrada()->end(); it++) {
				   inicio = (*it)->getInicio();
				   int ondeInicioEsta = NaCaixa[inicio->getIndice()];
				   caixas->at(ondeInicioEsta).remove(inicio);
				   caixas->at(ondeInicioEsta-1).push_front(inicio);
				   NaCaixa[inicio->getIndice()] = ondeInicioEsta-1;
				   if (altura[verticeGrauZero->getIndice()] + 1 > altura[inicio->getIndice()]) {
					   altura[inicio->getIndice()] = altura[verticeGrauZero->getIndice()] + 1;
					   sucessor[inicio->getIndice()] = verticeGrauZero;
				   }
			   }
			   caixas->at(0).remove(verticeGrauZero);
		   }

		   // Encontrar um vertice de maior altura
		   int indiceMaiorAltura = -1;
		   for (vector<Vertice*>::iterator it = getVertices()->begin()++;it != getVertices()->end();it++) {
			   if ((indiceMaiorAltura == -1) || (altura[(*it)->getIndice()] > altura[indiceMaiorAltura]))
				   indiceMaiorAltura = (*it)->getIndice();
		   }

		   // Montar lista contendo cadeia de maior comprimento
		   list<Vertice*> *maiorC = new list<Vertice*>();
		   Vertice *v = getVertice(indiceMaiorAltura);
		   do {
			   maiorC->push_back(v);
			   v = sucessor[v->getIndice()];
		   } while (v != NULL);

		   delete altura;
		   delete sucessor;
		   delete caixas;
		   delete NaCaixa;

		   //Armazenar explicitamente todas as relações transitivas
		   RelacoesTransitivas();

		   return maiorC;
	}

	// Passo 1 e 2 do algoritmo 2 de Wu et al.
	list<Vertice*>* Steps1And2() {

		int n = getNumeroDeVertices();
		list<Vertice*> *cadeiaMaisLonga = new list<Vertice*>;
		int nivelMaisProfundo = -1;

		// Para cada vertice com grau de entrada zero, executar DFS
		for (int i = 0 ; i < getNumeroDeVertices() ; i++ ) {
			if (getVertice(i)->getGrauDeEntrada() == 0) {

				int *pred = new int[n];
				pred[i] = i;
				stack<Vertice*> pilha;
				pilha.push(getVertice(i));
				Vertice *vertice;
				vector<bool> visitados;
				visitados.resize(n);
				visitados.at(i) = true;

				// Efetua DFS sobre um vertice de grau zero
				vector<int> nivel;
				nivel.resize(n);
				nivel.at(i) = 0;
				int maiorNivel = 0;
				int indiceMaiorNivel = i;

				while (!pilha.empty()) {
					vertice = pilha.top();
					pilha.pop();
					// Percorre cada vizinho nao visitado (NAO!! Percorremos os visitados de novo!!)
					for (list<Arco*>::iterator it = vertice->getListaDeSaida()->begin(); it != vertice->getListaDeSaida()->end(); it++) {
						//if (visitados.at((*it)->getFinal()->getIndice()) == false) {
							pilha.push((*it)->getFinal());
							int indiceFinal = (*it)->getFinal()->getIndice();
							visitados.at(indiceFinal) = true;
							// Atualiza informacoes de nivel e predecessor
							pred[indiceFinal] = (*it)->getInicio()->getIndice();
							nivel[indiceFinal] = nivel[(*it)->getInicio()->getIndice()]+1;
							// Se nivel do vertice final for maior que o maior nivel ja visto, atualizar
							if (nivel[indiceFinal]  >= maiorNivel) {
								maiorNivel = nivel[indiceFinal];
								indiceMaiorNivel = indiceFinal;
							}
						//}
					}
				}//while

				if (maiorNivel > nivelMaisProfundo) {
					// Atualizar nivel mais profundo ja visto
					nivelMaisProfundo = maiorNivel;
					// Percorrer vetor pred 'backwards', reconstruindo a cadeia mais longa
					int indiceAux = indiceMaiorNivel;
					cadeiaMaisLonga->clear();
					cadeiaMaisLonga->push_front(getVertice(indiceMaiorNivel));
					while (pred[indiceAux] !=  indiceAux) {
						indiceAux = pred[indiceAux];
						cadeiaMaisLonga->push_front(getVertice(indiceAux));
					}
				}
			}//if
		}//for

		//Armazenar explicitamente todas as relações transitivas
		RelacoesTransitivas();

		return cadeiaMaisLonga;
	}
	// Passo 3 do algoritmo 2 de Wu et al.
	list<Vertice*>* Step3(list<Vertice*> *extensao){

		list<Vertice*> verticesNaoIncluidos;
		list<Vertice*>::iterator iterVertice;
		list<Vertice*>::iterator posicaoDeInsercao;
		double custoTotal = 0;
		bool buscaInterrompida;

		// Popular lista de vertices que ainda nao pretencem aa extensao
		for (int i = 0 ; i < getNumeroDeVertices() ; i++)
			verticesNaoIncluidos.push_back(getVertice(i));
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
				// Verificar insercao do novo vertice no final da extensao parcial
				/*else if ((*iterVertice) == extensao->back()) {
					if ((*iterVertice)->obterCustoParaVertice(novo->getIndice(), custoNaPosicao) && (custoNaPosicao < custoNovoVertice)) {
						custoNovoVertice = custoNaPosicao;
						posicaoDeInsercao = extensao->end();
					}
				}*/
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
			//exibirCadeia(extensao);
			//cout << endl;
		}
		return extensao;
	}
	//Adiciona As relações transitivas para a Step3 do algoritmo de Wu et al
	void RelacoesTransitivas(){

		// Descobre o maior grau de saida
		int maiorGrau = -1;
		for(vector<Vertice*>::iterator it = getVertices()->begin()++;it != getVertices()->end();it++){
			if((*it)->getGrauDeSaida() > maiorGrau)
				maiorGrau = (*it)->getGrauDeSaida();
		}

		// Preenche as "caixas"
		int *NaCaixa = new int[getNumeroDeVertices()];
		vector<list<Vertice*>> *caixas = new vector<list<Vertice*>>(maiorGrau+1);
		for(vector<Vertice*>::iterator it = getVertices()->begin(); it != getVertices()->end(); it++) {
			int indiceDaCaixa = (*it)->getGrauDeSaida();
			caixas->at(indiceDaCaixa).push_back((*it));
			NaCaixa[(*it)->getIndice()] = indiceDaCaixa;
		}

		// Ordenação topologica por caixas
		while(caixas->at(0).size() > 0) {
			Vertice * verticeGrauZero = caixas->at(0).front();
			caixas->at(0).pop_front();
			for (list<Arco*>::iterator it = verticeGrauZero->getListaDeEntrada()->begin();
				it != verticeGrauZero->getListaDeEntrada()->end(); it++) {

					// Copia os transitivos
					list<Arco*>::iterator iterador = verticeGrauZero->getListaDeSaida()->begin();
					while(iterador != (*it)->getFinal()->getListaDeSaida()->end()){
						(*it)->getInicio()->adicionarRelacao((*iterador)->getFinal()->getIndice());
						Arco *novoArco = new Arco((*it)->getInicio(), (*iterador)->getFinal());
						(*it)->getInicio()->adicionarArcoDeSaida(novoArco);
						iterador++;
					}

					int ondeInicioEsta = NaCaixa[(*it)->getInicio()->getIndice()];
					caixas->at(ondeInicioEsta).remove((*it)->getInicio());
					caixas->at(ondeInicioEsta-1).push_back((*it)->getInicio());
					NaCaixa[(*it)->getInicio()->getIndice()] = ondeInicioEsta-1;

			}
		}
		delete NaCaixa;
		delete caixas;
	}

};

int ContabilizarExtLinear(list<Vertice*> ext){

	double custo = 0, aux;
	list<Vertice*>::iterator it = ext.begin();
	list<Vertice*>::iterator iter = ext.begin();

	int i = 0;
	for( ; iter != --ext.end(); it++){
		iter ++;
		(*it)->obterCustoParaVertice((*iter)->getIndice(),aux);
		custo += aux;
	}

	return (int)custo;
}

int OrdenacaoTopologica(Poset p){
	list<Vertice*> *ext = p.AlgoritmoOrdenacaoTopologica();
	//p.exibirCadeia(ext);
	//cout << "Ordenacao Topologica" << endl;
	int custo = ContabilizarExtLinear(*ext);
	//cout << custo << endl;
	return custo;
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

bool auditarExtLinear(list<Vertice*> *ext) {

	vector<bool> JaInserido(ext->size());

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


int AlgoritmoWu2(Poset p){

	list<Vertice*> *ext = p.maiorCadeia(); //p.Steps1And2();
	p.Step3(ext);

	//cout << "Algoritmo Wu2" << endl;
	int custo = ContabilizarExtLinear(*ext);
	//cout << custo << endl;
	if (!auditarExtLinear(ext))
		cout << "ERRO: Extensao Wu2 invalida!" << endl;
	return custo;
}

int main(int argc, char **args) {
	char* nomeArquivo = new char[1+strlen(args[1])];
	strcpy(nomeArquivo, args[1]);
	Poset *p = construirPoset(nomeArquivo);

	clock_t ini = clock();
	int custoOT = OrdenacaoTopologica(*p);
	clock_t fim = clock();
	double tempo = double(fim-ini)/CLK_TCK;

	cout << nomeArquivo << "," << custoOT << "," << tempo;

	ini = clock();
	int custoLiu2 = AlgoritmoWu2(*p);
	fim = clock();
	tempo = double(fim-ini)/CLK_TCK;

	cout << "," << custoLiu2 << "," << tempo << endl;

	getchar();
}
