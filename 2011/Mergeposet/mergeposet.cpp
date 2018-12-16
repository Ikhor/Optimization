#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <fstream>

using namespace std;

struct custo{
	bool Inf;
	double Custo;
};

struct Arco{
    
	struct Vertice *U;
    struct Vertice *V;

	void inicializarArco(Vertice *u, Vertice *v) {
		U = u;
		V = v;
    }
    void inicializarArco(Arco *a) {
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

struct Vertice {

	int Indice;
	int GrauDeEntrada;
	list<Arco*> *ListaDeAdjacencia;
	map<int,double> *MapaDeCustos;

    void inicializarVertice(int i){
		Indice = i;
		GrauDeEntrada = 0;
		ListaDeAdjacencia = new list<Arco*>;
		MapaDeCustos = new map<int,double>;
	}
	void adicionarCusto(Vertice* vertice,double custo){
		MapaDeCustos->insert(pair<int,double>(vertice->getIndice(),custo));
	}
	void adicionarCusto(Vertice* vertice){
		MapaDeCustos->insert(pair<int,double>(vertice->getIndice(),0));
	}
	void adicionarArcoDeSaida(Arco *a) {
         Arco *A = new Arco();
         A->inicializarArco(a);
         ListaDeAdjacencia->push_back(A);
		 adicionarCusto(a->getFinal());
	}
	void adicionarArcoDeSaida(Arco *a, double custo) {
         Arco *A = new Arco();
         A->inicializarArco(a);
		 adicionarCusto(a->getFinal(),custo);
	}
	void removerArcoDeSaida(Arco *a) {
		ListaDeAdjacencia->remove(a);
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
	int  getIndice() {
		return Indice;
	}
	list<Arco*>* getListaDeAdjacencia() {
		return ListaDeAdjacencia;
	}
};

struct GrafoDirecionado {

       vector<Vertice*> *Vertices;
       list<Arco*> *Arcos;

       void inicializarGrafoDirecionado(int numVertices){
            Vertices = new vector<Vertice*>(numVertices);
            for (int i = 0; i < numVertices; i++) {
                Vertices->at(i) = new Vertice;
                Vertices->at(i)->inicializarVertice(i);
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
					for(list<Arco*>::iterator it = verticeDestino->getListaDeAdjacencia()->begin(); it != verticeDestino->getListaDeAdjacencia()->end();it++)
						if((*it)->getInicio() == verticeOrigem)
							return NULL;
					Arco *arco = new Arco();
					arco->inicializarArco(verticeOrigem , verticeDestino);
					verticeOrigem->adicionarArcoDeSaida(arco);
					verticeDestino->incrementarGrauDeEntrada();
					Arcos->push_back(arco);
					return arco;
                }
             }
		     return NULL;
	   }
	   //Função Nova Adiciona apenas os arcos de custos(Serão invisiveis ao Grafo original)
	   Arco* adicionarArcoDeCusto(int origem, int destino, double custo) {
             int tamanho = getNumeroDeVertices();
             if ((origem < tamanho) && (destino < tamanho)) {
                Vertice *verticeOrigem = getVertice(origem);
                Vertice *verticeDestino = getVertice(destino);
				if ((verticeOrigem != NULL) && (verticeDestino != NULL)) {
					for(list<Arco*>::iterator it = verticeDestino->getListaDeAdjacencia()->begin(); it != verticeDestino->getListaDeAdjacencia()->end();it++)
						if((*it)->getInicio() == verticeOrigem)
							return NULL;
					Arco *arco = new Arco();
					arco->inicializarArco(verticeOrigem , verticeDestino);
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
		     Arcos->remove(arco);
       }
	   list<Arco*>* getArcos(){
            return Arcos;
	   }
	   vector<Vertice*>* getVertices(){
			return Vertices;
	   }

	   void showExtLinear(list<Vertice*> *extlinear){
		   for(list<Vertice*>::iterator it = extlinear->begin(); it != extlinear->end(); it++)
			   cout << (*it)->getIndice() << " ";
		   cout << endl;
	   }	   
	   
	   //Funções Novas
	   double custoArcoA(int i, int j,vector<Vertice*> *aux,vector<Vertice*> *aux2){
		   if(aux2->at(j)->MapaDeCustos[0].find(aux->at(i)->getIndice()) != aux2->at(j)->MapaDeCustos[0].end())
			   return aux2->at(j)->MapaDeCustos[0].at(aux->at(i)->getIndice());
		   return 999999;
	   }

	   double custoArcoB(int i, int j,vector<Vertice*> *aux,vector<Vertice*> *aux2){
		   if(aux->at(i)->MapaDeCustos[0].find(aux2->at(j)->getIndice()) != aux->at(i)->MapaDeCustos[0].end())
			   return aux->at(i)->MapaDeCustos[0].at(aux2->at(j)->getIndice());
		    return 999999;
	   }
	   double calcularCustoA(int i, int j,vector<Vertice*> *aux,vector<Vertice*> *aux2){
		   if(i < 1)
			   return 999999;
		   else if(j == 0)
			   return 0;
		   else
			   return min(calcularCustoA(i-1,j,aux,aux2),(calcularCustoB(i-1,j,aux,aux2) + custoArcoA(i,j,aux,aux2)));			  
	   }

	   double calcularCustoB(int i, int j,vector<Vertice*> *aux,vector<Vertice*> *aux2){
		   if(j < 1)
			   return 999999;
		   else if(i == 0)
			   return 0; 		
		   else
			   return min(calcularCustoA(i,j-1,aux,aux2) + custoArcoB(i,j,aux,aux2),calcularCustoB(i,j-1,aux,aux2));
	   }

	   double min(double a, double b){
		   if(a < b)
			   return a;
		   return b;
	   }
};



int main(){

    GrafoDirecionado g;

	ifstream fin;
	fin.open("Grafo.txt");
	int NumeroDeVertices;
	fin >> NumeroDeVertices;
	g.inicializarGrafoDirecionado(NumeroDeVertices);
	int NumeroDeArcosDiretos;
	fin >> NumeroDeArcosDiretos;
	int NumeroDeArcosComCustos;
	fin >> NumeroDeArcosComCustos;
	int aux1,aux2,aux3;
	for(int i = 0;i < NumeroDeArcosDiretos; i++){
		fin >> aux1;
		fin >> aux2;
		g.adicionarArco( aux1 , aux2 );
	}
	for(int i = 0;i < NumeroDeArcosComCustos; i++){
		fin >> aux1;
		fin >> aux2;
		fin >> aux3;
		g.adicionarArcoDeCusto( aux1 , aux2 , aux3 );
	}

	fin.close();

	vector<Vertice*> *aux = new vector<Vertice*>;
	
	aux->push_back(g.getVertice(5));
	aux->push_back(g.getVertice(1));
	aux->push_back(g.getVertice(2));
	aux->push_back(g.getVertice(4));

	vector<Vertice*> *auxi = new vector<Vertice*>;
	
	auxi->push_back(g.getVertice(5));
	auxi->push_back(g.getVertice(0));
	auxi->push_back(g.getVertice(3));

	double a = g.calcularCustoA(3,2,aux,auxi);	
	cout << a << endl;

	double b = g.calcularCustoB(3,2,aux,auxi);
	cout << b << endl;
	getchar();
}