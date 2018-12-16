#include <iostream>
#include <list>
#include <vector>
#include <fstream>
#include <time.h>

using namespace std;

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
		}
		return extensao;
	}

	list<Vertice*>* NovaSteps1And2(){

		list<Vertice*> *ExtLinear = new list<Vertice*>();

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

		int *Nivel = new int[getNumeroDeVertices()];
		
		int *Sucessor = new int[getNumeroDeVertices()];

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
					for(int i = 0; i < getNumeroDeVertices(); i++)
						if(verticeGrauZero->estaNoDownset(i))
							(*it)->getInicio()->adicionarRelacao(i);
			}
			caixas->at(0).pop_front();
		}

		int MaiorNivel = -2;		
		int indiceMaiorNivel;
		for(int i = 0; i <  getNumeroDeVertices(); i++){
			if(Nivel[i] > MaiorNivel){
				indiceMaiorNivel = i;
				MaiorNivel = Nivel[i];
			}
		}
		
		ExtLinear->push_back(getVertice(indiceMaiorNivel));
		while(Sucessor[indiceMaiorNivel] != -1){
			indiceMaiorNivel = Sucessor[indiceMaiorNivel];
			ExtLinear->push_back(getVertice(indiceMaiorNivel));
		}

		return ExtLinear;
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
	for(int i = 0;i < ext->size();i++)
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

int AlgoritmoWu2(Poset p){
	list<Vertice*> *ext = p.NovaSteps1And2(); //p.Steps1And2();
	p.Step3(ext);
	int custo = ContabilizarExtLinear(*ext);
	if (auditarExtLinear(ext) == false)
		cout << "ERRO: Extensao Wu2 invalida!" << endl;
	return custo;
}

int main(int argc, char* argv[]) {

	char* nomeArquivo = new char[1+strlen(argv[1])];
	strcpy(nomeArquivo, argv[1]);

	Poset *p = construirPoset(nomeArquivo);

	clock_t ini = clock();
	int custos = AlgoritmoWu2(*p);
	clock_t fim = clock();
	double tempo = double(fim-ini)/CLK_TCK;

	cout << nomeArquivo  << "," << custos << "," << tempo << endl; 
}
