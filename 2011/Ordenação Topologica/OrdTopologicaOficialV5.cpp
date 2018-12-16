#include <iostream>
#include <list>
#include <vector>
#include <time.h>

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
	int GrauDeSaida;
	list<Arco*> *ListaDeChegada;
	vector<double> *ListaDeCustos;

	vector<double>* getListaDeCusto(){
	return ListaDeCustos;
	}

    void inicializarVertice(int i){
		Indice = i;
		GrauDeSaida = 0;
		ListaDeChegada = new list<Arco*>;
		ListaDeCustos = new vector<double>;
	}

	void adicionarCusto(int numVertices,int seed){
		srand(seed);
		for(int i = 0; i < numVertices ; i++){
		ListaDeCustos->push_back(rand() / (double) RAND_MAX);
		}
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

	   Arco* adicionarArco(int origem, int destino) {
             int tamanho = getNumeroDeVertices();
             if ((origem < tamanho) && (destino < tamanho)) {
                Vertice *verticeOrigem = getVertice(origem);
                Vertice *verticeDestino = getVertice(destino);

                if ((verticeOrigem != NULL) && (verticeDestino != NULL)) {
					for(list<Arco*>::iterator it = verticeDestino->getListaDeChegada()->begin(); it != verticeDestino->getListaDeChegada()->end();it++)
						if((*it)->getInicio() == verticeOrigem)
							return NULL;
					Arco *arco = new Arco();
					arco->inicializarArco(verticeOrigem , verticeDestino);
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

	   vector<Vertice*>* getVertices(){
			return Vertices;
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
			   cout << "no " << (*it)->getIndice() << " na caixa " << indiceDaCaixa << endl;
		   }

		   cout << "[ ";
		   for (int i=0; i <= maiorGrau; i++)
			   cout << " " << caixas->at(i).size();
		   cout << " ]" << endl;

		   //Ordenação topologica por caixas
		   bool aux = true;
		   
		   while(caixas->at(0).size() > 0) {
			    
			   Vertice * verticeGrauZero = caixas->at(0).front();
			   
				if(aux == false){
				   double menorCusto = INT_MAX;//Definir com um valor MAXIMO
				   for(list<Vertice*>::iterator itV = caixas->at(0).begin();itV != caixas->at(0).end() ; itV++)
					   if((*itV)->ListaDeCustos->at(ExtLinear->front()) < menorCusto){
						   menorCusto = (*itV)->ListaDeCustos->at(ExtLinear->front());
						   verticeGrauZero = (*itV);
					   }
			   }

			   aux = false;
			   
			   cout << "    vgz " << verticeGrauZero->getIndice() << endl;
			   for (list<Arco*>::iterator it = verticeGrauZero->getListaDeChegada()->begin();
				   it != verticeGrauZero->getListaDeChegada()->end(); it++) {

					   cout << "    rebaixando " << (*it)->getInicio()->getIndice() << endl;
					   int ondeInicioEsta = NaCaixa[(*it)->getInicio()->getIndice()];
					   // Esta remocao (abaixo) poderia ser melhor
					   caixas->at(ondeInicioEsta).remove((*it)->getInicio());
					   caixas->at(ondeInicioEsta-1).push_front((*it)->getInicio());
					   NaCaixa[(*it)->getInicio()->getIndice()] = ondeInicioEsta-1;
							if(ondeInicioEsta-1 == 0){
								aux = true;
								cout << "Moveu o vertice " << (*it)->getInicio()->getIndice() << " para a caixa 0" << endl;
							}
			   
			   }

			   ExtLinear->push_front(verticeGrauZero->getIndice());
			   caixas->at(0).remove(verticeGrauZero);

			   cout << "[ ";
			   for (int i=0; i<=maiorGrau; i++) cout << " " << caixas->at(i).size();
			   cout << " ]" << endl;
		   }
		   return ExtLinear;
	   }

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
	for(vector<Vertice*>::iterator iter = g.getVertices()->begin() ; iter != g.getVertices()->end() ; iter++){
		(*iter)->adicionarCusto(g.getNumeroDeVertices(),rand());
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
		cout << "\tno" << pA->getInicio()->getIndice() << " -> no" << pA->getFinal()->getIndice() <<  " " << endl;
		itA ++;
	}
	cout << "}" << endl;
}

void GastoExtLinear(list<int> Poset, GrafoDirecionado g){
	
	double custo = 0;

	vector<Vertice*> *vertices = g.getVertices();

	int i = 0;
	
	for(list<int>::iterator iter = Poset.begin();i < Poset.size()-1; i++){

		int vertice1 = vertices->at((*iter))->getIndice();
		
		iter++;
		
		int vertice2 = vertices->at((*iter))->getIndice();

		cout << vertice1 << " -> " << vertice2 << " ";

		bool aux = false;
		
		for(list<Arco*>::iterator it = vertices->at(vertice2)->getListaDeChegada()->begin();
			it != vertices->at(vertice2)->getListaDeChegada()->end() ; it++)
			if((*it)->getInicio()->getIndice() == vertice1){
			cout << "Custo 0";
			aux = true;
			}
		if(!aux){
			custo += vertices->at(vertice1)->getListaDeCusto()->at(vertice2);
			cout << vertices->at(vertice1)->getListaDeCusto()->at(vertice2);
		}
	cout << endl;
	}
	cout << "O custo da extensao linear e: " << custo << endl;
	system("pause");
}

int main(){

	system ("title Ordenação Topologica");

    GrafoDirecionado g;
	
	gerar(g, 10, 15);

	estrutura(&g);
	
	list<int> *aux = g.GerarExtensaoLinear();
	g.imprimirPoset(*aux);
	
	cout << endl << endl << endl;
	
	//for(vector<Vertice*>::iterator iter = g.getVertices()->begin() ; iter != g.getVertices()->end() ; iter++)
		//for(int i = 0; i < 10; i++)
			//cout << "Do vertice " << (*iter)->getIndice() <<  " para o vertice "   << i << " tem custo "  << (*iter)->ListaDeCustos->at(i) << endl;
	
	system("pause");
	system("cls");
	GastoExtLinear(*aux,g);
	return 0;
}