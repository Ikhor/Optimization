#include <iostream>
#include <list>
#include <vector>
#include <stack>
#include <time.h>
#include <fstream>

using namespace std;


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
	vector<bool> *Transitivo;
	vector<bool> *Incomparavel;
	vector<bool> *Infinito;

    void inicializarVertice(int i, int numNos){
		Indice = i;
		Transitivo = new vector<bool>(numNos);
		Incomparavel = new vector<bool>(numNos);
		Infinito = new vector<bool>(numNos);
	}

	void adicionarTransitividade(int i) {
		 Transitivo->at(i) = true;
	}

	void adicionarIncomparabilidade(int i) {
		 Incomparavel->at(i) = true;
	}

	void adicionarInfinito(int i) {
		 Infinito->at(i) = true;
	}

	int getIndice() {
		return Indice;
	}

};

struct GrafoDirecionado {

       vector<Vertice*> *Vertices;
       list<Arco*> *Arcos;

       void inicializarGrafoDirecionado(int numVertices){
            Vertices = new vector<Vertice*>(numVertices);
            for (int i = 0; i < numVertices; i++) {
                Vertices->at(i) = new Vertice;
				Vertices->at(i)->inicializarVertice(i,numVertices);
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
				if(verticeOrigem->Infinito->at(verticeDestino->getIndice()) == true)
					return NULL;
				if(verticeOrigem->Transitivo->at(verticeDestino->getIndice()) == true)
					return NULL;
				Arco *arco = new Arco();
				arco->inicializarArco(verticeOrigem , verticeDestino);
				verticeOrigem->adicionarTransitividade(verticeDestino->getIndice());
				verticeDestino->adicionarInfinito(verticeOrigem->getIndice());
				Arcos->push_back(arco);
				return arco;
			 }
			 return NULL;
	   }

       list<Arco*>* getArcos(){
            return Arcos;
	   }

	   vector<Vertice*>* getVertices(){
			return Vertices;
	   }

};

void troca(int &a, int &b) {
	int t = a;
	a = b;
	b = t;
}

void gerar(GrafoDirecionado &g, int numNos, int numArestas) {

	g.inicializarGrafoDirecionado(numNos);

	//srand( time(NULL) );
	srand( 2623 );
				
	int numArcos=0;
	while (numArcos < numArestas) {
		int i = rand() % numNos;
		int j = rand() % numNos;

		if (i != j) {
			if (i > j)
				troca(i, j);		
			if(g.adicionarArco(i, j))
				numArcos ++;
		}
	}
}

void GerarArquivoPoset(GrafoDirecionado &g){
	ofstream fout;
	fout.open("poset.txt");
	
	//Grava as ligações imediatas de adjacencia
	for(int i = 0; i < g.getNumeroDeVertices() ; i++){
		for(int j = 0; j < g.getNumeroDeVertices(); j++)
			if(g.getVertice(i)->Transitivo->at(j) == true){
				fout << g.getVertice(i)->getIndice() << " " << g.getVertice(j)->getIndice() << endl;
			}
	}

	//Repassa a transitividade aos vertices
	for(int i = 0; i < g.getNumeroDeVertices() ; i++)
		for(int j = 0; j < g.getNumeroDeVertices(); j++)
			if(g.getVertice(i)->Infinito->at(j) == true)
				for(int k = 0; k < g.getNumeroDeVertices(); k++)
					if(g.getVertice(i)->Transitivo->at(k) == true){
						g.getVertice(j)->adicionarTransitividade(k);
						g.getVertice(k)->adicionarInfinito(j);
					}

	//Apenas Imprime a nova lista de adjacencia
	for(int i = 0; i < g.getNumeroDeVertices() ; i++){
		cout << "Lista de Adjacencia  do Vertice " << i << endl;
		for(int j = 0; j < g.getNumeroDeVertices(); j++)
			if(g.getVertice(i)->Transitivo->at(j) == true){
				cout << g.getVertice(j)->getIndice() << " ";
			}
		cout << endl;
	}

	//Adiciona as relações de incomparabilidaades
	for(int i = 0; i < g.getNumeroDeVertices() ; i++){
		for(int j = 0; j < g.getNumeroDeVertices(); j++)
			if(g.getVertice(i)->Transitivo->at(j) == false && g.getVertice(i)->Infinito->at(j) == false && i != j ){
				g.getVertice(i)->adicionarIncomparabilidade(j);
			}
	}

	//Imprime e grava no arquivo os custos incomparaveis
	for(int i = 0; i < g.getNumeroDeVertices() ; i++){
		cout << "Lista de incomparaveis  do Vertice " << i << endl;
		for(int j = 0; j < g.getNumeroDeVertices(); j++)
			if(g.getVertice(i)->Incomparavel->at(j) == true){
				fout << g.getVertice(i)->getIndice() << " " << g.getVertice(j)->getIndice() << " 1" <<endl;
				cout << g.getVertice(j)->getIndice() << " ";
			}
		cout << endl;
	}

}

int main(){
    GrafoDirecionado g;
	//Função gerar especificiar uma instancia de um grafo direcionado,o numero de vertices e o numero de arestas
	//OBS: função gerar esta com seed fixa parar gerar outros resultados modificar a seed no corpo da função
	gerar(g, 10, 15);
	GerarArquivoPoset(g);
	system("pause");
	return 0;
}