#include <iostream>
#include <list>
#include <vector>
#include <fstream>
#include <stack>
#include <float.h>
#include <time.h>

using namespace std;

/*
* Copiada Estrutura de Wu2V13 da Pasta Codigo\Novos 2012\OrdTopEwu2
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

	vector<vector<Vertice*>>* CapturarCadeias(){

		   vector<vector<Vertice*>> *ExtLinear = new vector<vector<Vertice*>>();

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

		   bool NovaCadeia = false;
		   int indiceAtual = 0;
		   ExtLinear->resize(indiceAtual+1);
		   //Ordenação topologica por caixas			   
		   while(caixas->at(0).size() > 0) {   
			   
			   Vertice * verticeGrauZero = caixas->at(0).back();
			   //   cout << verticeGrauZero->getIndice();
			   if(NovaCadeia){
				   indiceAtual++;
				   ExtLinear->resize(indiceAtual+1);
			   }

			   NovaCadeia = false;
			   list<Arco*>::iterator it = verticeGrauZero->getListaDeEntrada()->begin();
			   for ( ;it != verticeGrauZero->getListaDeEntrada()->end(); it++) {
				   int ondeInicioEsta = NaCaixa[(*it)->getInicio()->getIndice()];
				   caixas->at(ondeInicioEsta).remove((*it)->getInicio());//Lembrete para trocar remoção
				   caixas->at(ondeInicioEsta-1).push_back((*it)->getInicio());
				   NaCaixa[(*it)->getInicio()->getIndice()] = ondeInicioEsta-1;
				   if(ondeInicioEsta-1 == 0)
					   NovaCadeia = true;
			   }
			   ExtLinear->at(indiceAtual).push_back(verticeGrauZero);
			   caixas->at(0).remove(verticeGrauZero);
		   }
		   delete NaCaixa;
		   delete caixas;
		   return ExtLinear;
	}

};

void Imprime(vector<vector<Vertice*>> *Cadeias){
	for(auto itCadeia = Cadeias->begin(); itCadeia != Cadeias->end(); itCadeia++){
		for(auto itElementoCadeia = (*itCadeia).begin(); itElementoCadeia != (*itCadeia).end() ; itElementoCadeia++)
			cout << (*itElementoCadeia)->getIndice() << " ";
	cout << endl;
	}
}
void GTS(Poset p){
	vector<vector<Vertice*>> *ext = p.CapturarCadeias();
	Imprime(ext);
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



int main(int argc, char **args) {
	char* nomeArquivo = new char[1+strlen(args[1])];
	strcpy(nomeArquivo, args[1]);
	Poset *p = construirPoset(nomeArquivo);


	clock_t ini = clock();
	GTS(*p);
	clock_t fim = clock();
	double tempo = double(fim-ini)/CLK_TCK;

	cout << nomeArquivo << "," << endl; //<< "," << tempo;
	system("pause");
}
