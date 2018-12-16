#include <iostream>
#include <list>
#include <vector>

using namespace std;

struct Arco{
       struct Vertice *U;
       struct Vertice *V;
       double Custo;

       void inicializarArco(Vertice *u, Vertice *v, double custo) {
	        U = u;
	        V = v;
	        Custo = custo;
       }

       void inicializarArco(Arco *a) {
	        U = a->getInicio();
	        V = a->getFinal();
	        Custo = a->getCusto();
       }

       double getCusto() {
              return Custo;
       }

       Vertice * getInicio() {
              return U;
       }

       Vertice * getFinal() {
              return V;
       }
};


struct Vertice{
	int Indice;
	list<Arco*> *ListaAdjacencia;
	
    void inicializarVertice(int i){
		Indice = i;
		ListaAdjacencia = new list<Arco*>;
	}

	void adicionarArco(Arco *a) {
         Arco *A = new Arco();
         A->inicializarArco(a);
         ListaAdjacencia->push_back(A);
	}

	void removerArco(Arco *a) {
		ListaAdjacencia->remove(a);
	}

	int getIndice() {
		return Indice;
	}

	list<Arco*>* getListaDeAdjacencia() {
		return ListaAdjacencia;
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
           if (i < Vertices->size())
              return Vertices->at(i);
           return NULL;
	   }

	   Arco* adicionarArco(int origem, int destino, double custo) {
             int tamanho = Vertices->size();
             if ((origem < tamanho) && (destino < tamanho)) {
                Vertice *verticeOrigem = getVertice(origem);
                Vertice *verticeDestino = getVertice(destino);

                if ((verticeOrigem != NULL) && (verticeDestino != NULL)) {
                   Arco *arco = new Arco();
                   arco->inicializarArco(verticeOrigem , verticeDestino, custo);
                   verticeOrigem->adicionarArco(arco);
                   Arcos->push_back(arco);
                   return arco;
                }
             }
		     return NULL;
	   }

       void removerArco(Arco *arco) {
             Vertice *inicio = arco->getInicio();
             inicio->removerArco(arco);
		     Arcos->remove(arco);
       }

       list<Arco*>* getArcos() {
            return Arcos;
	   }
};
/*
ideia de algoritmo para ordenação topologica

da lista de vertice seleciono o primeiro elemento
verifoco se o mesmo encontra-se na lista de aresta como vertice de destino
se essa busca me dizer que sim vou para o proximo elemento
senao 
removo o vertice do meu grafo
adiciono numa lista de "extensão linear"
removo todas as arestas em que ele se encontra
repito estes passos até nao restar mais vertice no grafo

implementação a definir

poset(){
	list<Vertice*> *Poset = new list<Vertice*>;
	while( getNumeroDeVertices() > 0){
		for(int i = 0,bool condicao = true;i <  getNumeroDeVertices();i++,condicao = true{
			for(int j = 0;j < getNumeroDeArcos() && condicao == true;j++){
				if(getVertice(i) ==  //verticedestino{
					condicao = false
				}
			}
				if(condicao == true){
				//removerVertice
				Poset->push_back(V);
					for(int j = 0;j < getNumeroDeArcos();j++){
					if(V->origem == //VerticeOrigemDaLista
					removerArco(//VerticeOrigemDaLista)
					}
				}
		}
	}
}

*/

int main(){
    
    GrafoDirecionado g;
    g.inicializarGrafoDirecionado(21);
    g.adicionarArco(60, 30, 1.0);
    g.adicionarArco(60, 20, 1.0);
    g.adicionarArco(60, 12, 1.0);
    g.adicionarArco(30,  6, 1.0);
    g.adicionarArco(30, 15, 1.0);
    g.adicionarArco(20, 10, 1.0);
    g.adicionarArco(20,  4, 1.0);
    g.adicionarArco(12,  6, 1.0);
    g.adicionarArco(12,  4, 1.0);
    g.adicionarArco(12,  6, 1.0);
    g.adicionarArco(12,  4, 1.0);
    g.adicionarArco(15,  3, 1.0);
    g.adicionarArco(15,  5, 1.0);
    g.adicionarArco( 6,  3, 1.0);
    g.adicionarArco( 6,  2, 1.0);
    g.adicionarArco(10,  2, 1.0);
    g.adicionarArco(10,  5, 1.0);
    g.adicionarArco( 4,  2, 1.0);
    g.adicionarArco( 3,  1, 1.0);
    g.adicionarArco( 2,  1, 1.0);
    g.adicionarArco( 5,  1, 1.0);
    
    system("pause");
    return 0;
}
