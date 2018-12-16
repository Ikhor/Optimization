#include <iostream>
#include <list>
#include <vector>

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
	int GrauDeEntrada;
	list<Arco*> *ListaDeAdjacencia;
	vector<double> *ListaDeCustos;

	vector<double>* getListaDeCusto(){
	return ListaDeCustos;
	}

    void inicializarVertice(int i){
		Indice = i;
		GrauDeEntrada = 0;
		ListaDeAdjacencia = new list<Arco*>;
		ListaDeCustos = new vector<double>;
	}

	void adicionarCusto(int numVertices,int seed){
		srand(seed);
		for(int i = 0; i < numVertices ; i++){
		ListaDeCustos->push_back(rand() / (double) RAND_MAX);
		}
	}

	void adicionarArcoDeSaida(Arco *a) {
         Arco *A = new Arco();
         A->inicializarArco(a);
         ListaDeAdjacencia->push_back(A);
	}

	void removerArcoDeSaida(Arco *a) {
		ListaDeAdjacencia->remove(a);
	}

	int getGrauDeEntrada() {
		return GrauDeEntrada;
	}

	void incrementarGrauDeEntrada() {
		GrauDeEntrada ++;
	}

	void decrementarGrauDeEntrada() {
		GrauDeEntrada --;
	}

	int getIndice() {
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
	   
	   vector<list<Vertice*>>* step1(){

			//Descobre o maior grau
		   int maiorGrau = -1;
		   
		   for(vector<Vertice*>::iterator it = getVertices()->begin()++;it != getVertices()->end();it++){
			   if((*it)->getGrauDeEntrada() > maiorGrau)
				   maiorGrau = (*it)->getGrauDeEntrada();
		   }

		   //Preenche as "caixas"
		   int *NaCaixa = new int[getNumeroDeVertices()];
		   vector<list<Vertice*>> *caixas = new vector<list<Vertice*>>(maiorGrau+1);
		   for(vector<Vertice*>::iterator it = getVertices()->begin()++;it != getVertices()->end();it++){
			   int indiceDaCaixa = (*it)->getGrauDeEntrada();
			   caixas->at(indiceDaCaixa).push_back((*it));
			   NaCaixa[(*it)->getIndice()] = indiceDaCaixa;
			//   cout << "no " << (*it)->getIndice() << " na caixa " << indiceDaCaixa << endl;
		   }
		   
		   //layer construction
		   
		   int indice = 0;
		   vector<list<Vertice*>> *layer = new vector<list<Vertice*>>;
		   layer->resize(1);

		   while(caixas->at(0).size() > 0) {

			   int iteracoes = caixas->at(0).size();
			   
			   for(int i = 0; i < iteracoes ; i ++){
				   Vertice * verticeGrauZero = caixas->at(0).front();
				   for(list<Arco*>::iterator it = verticeGrauZero->getListaDeAdjacencia()->begin();
					   it != verticeGrauZero->getListaDeAdjacencia()->end();it++){

						   int ondeInicioEsta = NaCaixa[(*it)->getFinal()->getIndice()];
						   caixas->at(ondeInicioEsta).remove((*it)->getFinal());
						   caixas->at(ondeInicioEsta-1).push_back((*it)->getFinal());
						   NaCaixa[(*it)->getFinal()->getIndice()] = ondeInicioEsta-1;
					   }
					   layer->at(indice).push_back(verticeGrauZero);
					   caixas->at(0).remove(verticeGrauZero);
				   }
			   indice++;
			   layer->resize(indice+1);
			}


	   //Mostra as layers
		   cout << "Step 1\n";
		   for(int i = 0; i < layer->size()-1;i++){
			   cout << "H"<< i << endl;
			   for(list<Vertice*>::iterator iter =  layer->at(i).begin(); iter != layer->at(i).end();iter++)
				   cout << (*iter)->getIndice() << " ";
			   cout << endl;
		   }
		   return layer;
	   }

	int calcAltura(Vertice *no){
		int a,b;
		a = 0;
		for(list<Arco*>::iterator iter = no->getListaDeAdjacencia()->begin(); iter !=no->getListaDeAdjacencia()->end();iter++){
			b = calcAltura((*iter)->getFinal());
			if(b > a)
				a = b;
		}
		return a+1;
	}
	
	void step2(vector<list<Vertice*>> *layer){

		int ValorDaMaiorAltura = -1;
		int NumeroDoVertice;

		for(list<Vertice*>::iterator iter =  layer->at(0).begin(); iter != layer->at(0).end();iter++){
			int b = calcAltura((*iter));
			if(b > ValorDaMaiorAltura){
				ValorDaMaiorAltura = b;
				NumeroDoVertice = (*iter)->getIndice();
			}
		}
		cout << "O indice com a maior cadeia e: " << NumeroDoVertice << " com altura de " << ValorDaMaiorAltura << endl;

		list<Vertice*> *extlinear = new list<Vertice*>;

		Vertice *Aux = getVertices()->at(NumeroDoVertice);
		extlinear->push_back(Aux);
		
		while(extlinear->size() < ValorDaMaiorAltura){

		int ValorDaMaiorAlturaAux = -1;
		NumeroDoVertice = -1;

			for(list<Arco*>::iterator iter = Aux->getListaDeAdjacencia()->begin();iter != Aux->getListaDeAdjacencia()->end();iter++){
				int b = calcAltura((*iter)->getFinal());
				if(b > ValorDaMaiorAlturaAux){
				ValorDaMaiorAlturaAux = b;
				NumeroDoVertice = (*iter)->getFinal()->getIndice();
				}
			}
		
			Aux = getVertices()->at(NumeroDoVertice);
			extlinear->push_back(Aux);
		}
		
		for(list<Vertice*>::iterator it = extlinear->begin(); it != extlinear->end(); it++)
			cout << (*it)->getIndice() << " ";
			
	}

};

int main(){

    GrafoDirecionado g;

	g.inicializarGrafoDirecionado(10);

    g.adicionarArco( 0, 1 );
    g.adicionarArco( 0, 3 );
	g.adicionarArco( 0, 5 );
	g.adicionarArco( 0, 7 );
    g.adicionarArco( 2, 5 );
	g.adicionarArco( 2, 6 );
	g.adicionarArco( 2, 8 );
	g.adicionarArco( 3, 4 );
	g.adicionarArco( 3, 6 );
	g.adicionarArco( 4, 5 );
	g.adicionarArco( 4, 6 );
	g.adicionarArco( 5, 6 );
	g.adicionarArco( 5, 8 );
	g.adicionarArco( 5, 9 );\
	g.adicionarArco( 6, 8 );
	
	vector<list<Vertice*>> *layer = g.step1();
	
	cout << "\n\nStep2\n";
	//cout << g.calcAltura(g.getVertices()->at(6));
	g.step2(layer);

	getchar();
}