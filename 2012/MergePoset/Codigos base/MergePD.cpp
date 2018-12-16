#include <iostream>
#include <list>
#include <vector>
#include <fstream>
#include <stack>
#include <float.h>
#include <time.h>

using namespace std;

double minimo(double a, double b){
	if(a < b)
		return a;
	return b;
}

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
	
	//Funções Novas
	double custoArcoMerge(int i, int j,vector<Vertice*> *lista1,vector<Vertice*> *lista2,char c)
	{
		double custo;
		if(c == 'A')
			lista2->at(j)->obterCustoParaVertice(lista1->at(i)->getIndice(),custo);
		else if(c == 'B')
			lista1->at(i)->obterCustoParaVertice(lista2->at(j)->getIndice(),custo);
		return custo;
	}


	//void Liu1(vector<Vertice*> *lista1,vector<Vertice*> *lista2){
	 vector<Vertice*>* Liu1(vector<Vertice*> *lista1,vector<Vertice*> *lista2){
		
		int NumeroEstado = (lista1->size())*(lista2->size());

		char  * anterior =  new char  [NumeroEstado*2];
		double* valorA   =  new double[NumeroEstado];
		double* valorB   =  new double[NumeroEstado];

		for(int i = 0; i < NumeroEstado; i++){
			valorA[i] = DBL_MAX;
			valorB[i] = DBL_MAX;
			anterior[i] = 'a';
			anterior[2*i] = 'b';
		}

		for(int i = 0; i < lista1->size(); i++){
			valorA[i*lista2->size()] = 0;
			anterior[i] = 'c';
		}

		for(int i = 0; i < lista2->size(); i++){
			valorB[i] = 0;
			anterior[i] = 'c';
		}

		for(int i = 1 ; i < lista1->size() ; i++)
			for(int j = 1 ; j < lista2->size() ; j++){

					if( valorA[(j-1)+ (i*lista2->size())] + custoArcoMerge(i,j,lista1,lista2,'B') < valorB[(j-1)+ (i*lista2->size())] ){

						valorB[j+ (i*lista2->size())] = valorA[(j-1)+ (i*lista2->size())] + custoArcoMerge(i,j,lista1,lista2,'B');
						anterior[2*(j+ (i*lista2->size()))] = 'a'; 
					}
					else{
						valorB[j+ (i*lista2->size())] = valorB[(j-1) + (i*lista2->size())];
						anterior[2*(j+ (i*lista2->size()))] = 'b'; 
					}
				
				   if(valorA[j+ ((i-1)*lista2->size())] < valorB[j+ ((i-1)*lista2->size())] + custoArcoMerge(i,j,lista1,lista2,'A')){
					   valorA[j + (i*lista2->size())] = valorA[j+ ((i-1)*lista2->size())];
					   anterior[j+ (i*lista2->size())] = 'a'; 
				   }
				   else{
					   valorA[j + (i*lista2->size())] = valorB[j+ ((i-1)*lista2->size())] + custoArcoMerge(i,j,lista1,lista2,'A');
					   anterior[j+ (i*lista2->size())] = 'b';
				   }

			}			
			
			list<Vertice*>  *Inverte = new list<Vertice*>;
			vector<Vertice*>  *extLinearResultante = new vector<Vertice*>;

			vector<Vertice*>::iterator A =  lista1->end();
			vector<Vertice*>::iterator B =  lista2->end();
			A--;B--;

			int indice = NumeroEstado-1;

			while(anterior[indice] != 'c'){
				if(valorA[indice] < valorB[indice]){
					Inverte->push_front((*A));
					A--;
					indice = indice- lista2->size();
				}else{
					Inverte->push_front((*B));
					B--;
					indice -= 1; 
				}
			}
			
			
			for(auto i = Inverte->begin();i != Inverte->end();i++){
				extLinearResultante->push_back(*i);
				cout << (*i)->getIndice() << " ";				
			}				
			cout << endl;
			return extLinearResultante;
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


int main(int argc, char **args) {
	char* nomeArquivo = new char[1+strlen(args[1])];
	strcpy(nomeArquivo, args[1]);
	Poset *g = construirPoset(nomeArquivo);
	
	vector<Vertice*> *auxi = new vector<Vertice*>;

	auxi->push_back(g->getVertice(5));
	auxi->push_back(g->getVertice(1));
	auxi->push_back(g->getVertice(2));
	auxi->push_back(g->getVertice(4));

	vector<Vertice*> *aux = new vector<Vertice*>;
	
	aux->push_back(g->getVertice(5));
	aux->push_back(g->getVertice(0));
	aux->push_back(g->getVertice(3));

	g->Liu1(aux,auxi);	

	
	system("pause");
}
