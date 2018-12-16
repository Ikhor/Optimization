#include <iostream>
#include <list>
#include <vector>
#include <time.h>

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

struct Vertice {
	int Indice;
	int GrauDeSaida;
	list<Arco*> *ListaDeChegada;

    void inicializarVertice(int i){
		Indice = i;
		GrauDeSaida = 0;
		ListaDeChegada = new list<Arco*>;
	}

	void adicionarArcoDeChegada(Arco *a) {
         Arco *A = new Arco();
         A->inicializarArco(a);
         ListaDeChegada->push_back(A);
	}

	void removerArcoDeChegada(Arco *a) {
		ListaDeChegada->remove(a);
	}

	int getGrauDeSaida() {
		return GrauDeSaida;
	}

	void incrementarGrauDeSaida() {
		GrauDeSaida ++;
	}

	void decrementarGrauDeSaida() {
		GrauDeSaida --;
	}

	int getIndice() {
		return Indice;
	}

	list<Arco*>* getListaDeChegada() {
		return ListaDeChegada;
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

	   Arco* adicionarArco(int origem, int destino, double custo) {
             int tamanho = getNumeroDeVertices();
             if ((origem < tamanho) && (destino < tamanho)) {
                Vertice *verticeOrigem = getVertice(origem);
                Vertice *verticeDestino = getVertice(destino);

                if ((verticeOrigem != NULL) && (verticeDestino != NULL)) {
					Arco *arco = new Arco();
					arco->inicializarArco(verticeOrigem , verticeDestino, custo);
					verticeDestino->adicionarArcoDeChegada(arco);
					verticeOrigem->incrementarGrauDeSaida();
					Arcos->push_back(arco);
					return arco;
                }
             }
		     return NULL;
	   }

       void removerArco(Arco *arco) {
             Vertice *fim = arco->getFinal();
             fim->removerArcoDeChegada(arco);
			 Vertice *inicio = arco->getInicio();
			 inicio->decrementarGrauDeSaida();
		     Arcos->remove(arco);
       }

       list<Arco*>* getArcos(){
            return Arcos;
	   }

	   vector<Vertice*>* getVertices(){//Nova Função
			return Vertices;
	   }

	   list<int>* GerarExtensaoLinearV(){

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
			   cout << "no " << (*it)->getIndice() << " na caixa " << indiceDaCaixa << endl;
		   }

		   cout << "[ ";
		   for (int i=0; i <= maiorGrau; i++)
			   cout << " " << caixas->at(i).size();
		   cout << " ]" << endl;

		   //Ordenação topologica por caixas
		   while(caixas->at(0).size() > 0) {

			   Vertice * verticeGrauZero =  verticeGrauZero = caixas->at(0).front();

			   cout << "    vgz " << verticeGrauZero->getIndice() << endl;
			   for (list<Arco*>::iterator it = verticeGrauZero->getListaDeChegada()->begin();
				   it != verticeGrauZero->getListaDeChegada()->end(); it++) {

					   cout << "    rebaixando " << (*it)->getInicio()->getIndice() << endl;
					   int ondeInicioEsta = NaCaixa[(*it)->getInicio()->getIndice()];
					   // Esta remocao (abaixo) poderia ser melhor
					   caixas->at(ondeInicioEsta).remove((*it)->getInicio());
					   caixas->at(ondeInicioEsta-1).push_front((*it)->getInicio());
					   NaCaixa[(*it)->getInicio()->getIndice()] = ondeInicioEsta-1;
			   }

			   ExtLinear->push_front(verticeGrauZero->getIndice());
			   caixas->at(0).remove(verticeGrauZero);

			   cout << "[ ";
			   for (int i=0; i<=maiorGrau; i++) cout << " " << caixas->at(i).size();
			   cout << " ]" << endl;
		   }
		   return ExtLinear;
	   }


	/*
	Recebe uma lista com a extensão linear gerada pelo algoritmo no método POSET
	*/

	void imprimirPoset(list<int> Poset){
			for(int i = Poset.size();i > 0;i--){
				cout << Poset.front() << " ";
				Poset.pop_front();
			}
	cout << endl;
	}
};

void troca(int &a, int &b) {
	int t = a;
	a = b;
	b = t;
}

void gerar(GrafoDirecionado &g, int numNos, int numArestas) {

	g.inicializarGrafoDirecionado(numNos);

	srand( time(NULL) );
	//srand( 21 );

	int numArcos=0;
	while (numArcos < numArestas) {
		int i = rand() % numNos;
		int j = rand() % numNos;

		if (i != j) {
			if (i > j) troca(i, j);
			g.adicionarArco(i, j, rand() / (double) RAND_MAX);
			numArcos ++;
		}
	}

}

void estrutura(GrafoDirecionado *g) {
	cout << "digraph G {" << endl;

	list<Arco*> *lista = g->getArcos();
	list<Arco*>::iterator itA = lista->begin();

	for (int i=0; i<g->getNumeroDeVertices(); i++) {
		cout << "\tno" << g->getVertice(i)->getIndice();
		cout << " [label=\"" << g->getVertice(i)->getIndice() << "\"]" << endl;
	}
	while (itA != lista->end()) {
		Arco *pA = *itA;

		cout << "\tno" << pA->getInicio()->getIndice() << " -> no" << 
			pA->getFinal()->getIndice() << endl;

		itA ++;
	}

	cout << "}" << endl;
}


int main(){

    GrafoDirecionado g;

	gerar(g, 10, 15);

	list<int> *Poset = g.GerarExtensaoLinearV();
	g.imprimirPoset(*Poset);

	estrutura(&g);
	system("pause");

	return 0;
}