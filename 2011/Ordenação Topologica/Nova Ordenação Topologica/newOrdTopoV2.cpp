#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <fstream>
#include <stack>
#include <float.h>

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
         Arco *A = new Arco(a);
         ListaDeSaida->push_back(A);
		 adicionarRelacao(a->getFinal());
	}

	void adicionarArcoDeSaida(Arco *a, double custo) {
         Arco *A = new Arco(a);
		 adicionarCusto(a->getFinal(),custo);
	}

	void adicionarArcoDeEntrada(Arco *a) {
         Arco *A = new Arco(a);
         ListaDeEntrada->push_back(A);
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


	list<int>* GerarExtensaoLinear(){

		   list<int> *ExtLinear = new list<int>();

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
				   double menorCusto = INT_MAX;//Definir com um valor MAXIMO
				   for(list<Vertice*>::iterator itV = caixas->at(0).begin();itV != caixas->at(0).end() ; itV++)
					   if((*itV)->obterCustoParaVertice(ExtLinear->front(),custo) && custo < menorCusto ){
						   menorCusto = custo;
						   verticeGrauZero = (*itV);
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
			   ExtLinear->push_front(verticeGrauZero->getIndice());
			   caixas->at(0).remove(verticeGrauZero);
		   }
		   return ExtLinear;
	}


};

int Lucas(char* arq){
	int NumeroDeVertices, NumeroDeArcosDiretos, NumeroDeArcosComCustos;

	ifstream fin;
	fin.open(arq);
	fin >> NumeroDeVertices;
	fin >> NumeroDeArcosDiretos;
	fin >> NumeroDeArcosComCustos;

    Poset p(NumeroDeVertices);

	int aux1,aux2,aux3;
	for (int i = 0; i < NumeroDeArcosDiretos; i++) {
		fin >> aux1;
		fin >> aux2;
		p.adicionarArco( aux1 , aux2 );
	}

	for (int i = 0; i < NumeroDeArcosComCustos; i++) {
		fin >> aux1;
		fin >> aux2;
		fin >> aux3;
		p.adicionarArcoDeCusto( aux1 , aux2 , aux3 );
	}

	fin.close();

	list<int> *ext = p.GerarExtensaoLinear();
	double custo = 0;

	double aux;
	list<int>::iterator it = ext->begin();
	list<int>::iterator iter = ext->begin();

	
	for( ; iter != --ext->end(); it++){
		iter++;
		p.getVertice(*it)->obterCustoParaVertice(*iter,aux);
		custo += aux;
	}
	

	return custo;
}



int main() {
	char* nomeArquivo = "poset_n100_d4_s567.txt";
	cout << Lucas(nomeArquivo) << endl;

	getchar();
}
