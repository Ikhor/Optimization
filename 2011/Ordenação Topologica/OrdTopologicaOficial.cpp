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
		   for (int i=0; i<=maiorGrau; i++) cout << " " << caixas->at(i).size();
		   cout << " ]" << endl;

		   //Ordenação topologica por caixas
		   while(caixas->at(0).size() > 0) {
			   Vertice * verticeGrauZero = caixas->at(0).front();
			   cout << "    vgz " << verticeGrauZero->getIndice() << endl;
			   for (list<Arco*>::iterator it = verticeGrauZero->getListaDeChegada()->begin();
				   it != verticeGrauZero->getListaDeChegada()->end(); it++) {

					   cout << "    rebaixando " << (*it)->getInicio()->getIndice() << endl;
					   int ondeInicioEsta = NaCaixa[(*it)->getInicio()->getIndice()];
					   // Esta remocao (abaixo) poderia ser melhor
					   caixas->at(ondeInicioEsta).remove((*it)->getInicio());
					   caixas->at(ondeInicioEsta-1).push_back((*it)->getInicio());
					   NaCaixa[(*it)->getInicio()->getIndice()] = ondeInicioEsta-1;
			   }

			   ExtLinear->push_back(verticeGrauZero->getIndice());
			   caixas->at(0).pop_front();

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

int main(){

    GrafoDirecionado g;

	g.inicializarGrafoDirecionado(12);

	g.adicionarArco( 0,   1, 1.0);
    g.adicionarArco( 0,   2, 1.0);
    g.adicionarArco( 0,   3, 1.0);

	g.adicionarArco( 1,   4, 1.0);
	g.adicionarArco( 1,   5, 1.0);
	g.adicionarArco( 1,   6, 1.0);

	g.adicionarArco( 2,   5, 1.0);
    g.adicionarArco( 2,   7, 1.0);

	g.adicionarArco( 3,   6, 1.0);
    g.adicionarArco( 3,   7, 1.0);

    g.adicionarArco( 4,   8, 1.0);
	g.adicionarArco( 4,   9, 1.0);

	g.adicionarArco( 5,   8, 1.0);
	g.adicionarArco( 5,  10, 1.0);

	g.adicionarArco( 6,   9, 1.0);
    g.adicionarArco( 6,  10, 1.0);

	g.adicionarArco( 7,  10, 1.0);

	g.adicionarArco( 8,  11, 1.0);

    g.adicionarArco( 9,  11, 1.0);

	g.adicionarArco(10,  11, 1.0);

	//g.GerarExtensaoLinearV2();

	//g.imprimirPoset(*g.GerarExtensaoLinear());

	GrafoDirecionado g2;

    g2.inicializarGrafoDirecionado(12);

	g2.adicionarArco( 3, 7, 1.0);//60 30
    g2.adicionarArco( 3, 6, 1.0);//60 20
    g2.adicionarArco( 3, 11, 1.0);//60 12

    g2.adicionarArco( 7,  8, 1.0);//30 15
	g2.adicionarArco( 7,  9, 1.0);//30 6

    g2.adicionarArco( 6, 10, 1.0);//20 10
    g2.adicionarArco( 6,  5, 1.0);//20 4

	g2.adicionarArco(11,  9, 1.0);//12 6
 	g2.adicionarArco(11,  5, 1.0);//12 4

	g2.adicionarArco( 8,  4, 1.0);//15 3
    g2.adicionarArco( 8,  1, 1.0);//15 5

	g2.adicionarArco(10,  1, 1.0);//10 5
    g2.adicionarArco(10,  2, 1.0);//10 2

	g2.adicionarArco( 9,  4, 1.0);//6  3
    g2.adicionarArco( 9,  2, 1.0);//6  2

	g2.adicionarArco( 5,  2, 1.0);//4  2

	g2.adicionarArco( 4,  0, 1.0);//3  1

	g2.adicionarArco( 1,  0, 1.0);//5  1

	g2.adicionarArco( 2,  0, 1.0);//2  1

	/*
	Valor Indice
	60		 3
	30		 7
	20		 6
	12		11
	15		 8
	10		10
	 4		 5
	 6		 9
	 3		 4
	 5		 1
	 2		 2
	 1		 0
	*/

	//g2.imprimirPoset(*g2.GerarExtensaoLinear());

	//for(vector<Vertice*>::iterator it = g2.Vertices->begin();it != g2.Vertices->end();it++)
	//	cout << (*it)->getIndice() << " " << (*it)->getListaDeAdjacencia()->size() << endl;
	//g.imprimirPoset(
	list<int> *Poset = g.GerarExtensaoLinearV();
	g.imprimirPoset(*Poset);
	system("pause");

	return 0;
}