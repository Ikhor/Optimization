#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <fstream>
#include <stack>

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

	   list<Vertice*>* Step1And2(){

		   list<Vertice*> *cadeiaMaisLonga = new list<Vertice*>;
		   int maisProfundo = -1;

		   for ( int i = 0 ; i < getNumeroDeVertices() ; i++ )
			   if ( getVertice(i)->getGrauDeEntrada() == 0 ) {

				   int *pred = new int[getNumeroDeVertices()]; 
				   pred[i] = i;
				   stack<Vertice*> aux;
				   aux.push(getVertice(i));
				   Vertice *auxiliar;
				   vector<bool> visitados;
				   visitados.resize(getNumeroDeVertices());
				   visitados.at(i) = true;
				   
				   //Mudanças feitas na DFS Original
				   vector<int> nivel;
				   nivel.resize(getNumeroDeVertices());
				   nivel.at(i) = 0;
				   int maiorNivel = 0;
				   int indiceMaiorNivel;

					while(!aux.empty()){
					   auxiliar = aux.top();
					   aux.pop();
					   for(list<Arco*>::iterator it = auxiliar->getListaDeAdjacencia()->begin(); it != auxiliar->getListaDeAdjacencia()->end(); it++){
						   if( visitados.at((*it)->getFinal()->getIndice()) == false){
							   visitados.at((*it)->getFinal()->getIndice()) = true;
							   aux.push((*it)->getFinal());
							   pred[(*it)->getFinal()->getIndice()] = (*it)->getInicio()->getIndice();
							   nivel[(*it)->getFinal()->getIndice()] = nivel[(*it)->getInicio()->getIndice()]+1;
							   if(nivel[(*it)->getFinal()->getIndice()]  >= maiorNivel){
								   maiorNivel = nivel[(*it)->getFinal()->getIndice()];
								   indiceMaiorNivel = (*it)->getFinal()->getIndice();
							   }
						   }
					   }
				   }
					if(indiceMaiorNivel >= maisProfundo){
						cadeiaMaisLonga->clear();
						maisProfundo = indiceMaiorNivel;
						int indiceAux = indiceMaiorNivel;
						cadeiaMaisLonga->push_front(getVertice(indiceMaiorNivel));
						while(pred[indiceAux] !=  indiceAux){
							cadeiaMaisLonga->push_front(getVertice(pred[indiceAux]));
							indiceAux = pred[indiceAux];
						}
					}
			   }
		   return cadeiaMaisLonga;
	   }
	   
	   /*
	   Funções abaixo tem apenas a funcionalidade de exibir os resultados pela Step's do algoritmo
	   */
	   void showExtLinear(list<Vertice*> *extlinear){
		   for(list<Vertice*>::iterator it = extlinear->begin(); it != extlinear->end(); it++)
			   cout << (*it)->getIndice() << " ";
		   cout << endl;
	   }
	   
	   void Step3(list<Vertice*> *ext){

		   map<int,double>::iterator it;
		   list<Vertice*> auxiliar;
		   list<Vertice*>::iterator iterVertice;
		   list<Vertice*>::iterator save;
		   double custoTotal = 0;

		   for(int i = 0 ; i < getNumeroDeVertices() ; i++)
			   auxiliar.push_back(getVertice(i));

		   for(iterVertice = ext->begin() ; iterVertice != ext->end() ; iterVertice++)
			   auxiliar.remove(*iterVertice);		

		   do{
			   double custo = DBL_MAX;
			   
			   for( iterVertice = ext->begin() ; iterVertice != ext->end() ; iterVertice++){
				   it = auxiliar.front()->MapaDeCustos->find((*iterVertice)->getIndice());
				   if(it != auxiliar.front()->MapaDeCustos->end() && iterVertice !=  ext->begin()){
					   double custoTemp = it->second;
					   iterVertice--;
					   it = (*iterVertice)->MapaDeCustos->find(auxiliar.front()->getIndice());
					   if(it !=  (*iterVertice)->MapaDeCustos->end()){
						   custoTemp += it->second;
						   if(custoTemp <= custo){
						   custo = custoTemp;
						   save = ++iterVertice;
						   iterVertice--;
						   }
					   }
					   iterVertice++;  
				   } 
				   if(iterVertice == ext->begin() && it !=  auxiliar.front()->MapaDeCustos->end()){
					   custo = it->second;
					   save = iterVertice;   
				   }
				   if(ext->back() == (*iterVertice)){
					   it = (*iterVertice)->MapaDeCustos->find(auxiliar.front()->getIndice());
					   if(it !=  (*iterVertice)->MapaDeCustos->end() && custo >= it->second){
						   custo = it->second;
						   save = ext->end();
					   }
				   }      	
			   }
			   cout << "Menor custo: " << custo << endl;
			   ext->insert(save,auxiliar.front());
			   showExtLinear(ext);
			   cout << endl;
			   auxiliar.pop_front();
			   custoTotal += custo;
		   }while(auxiliar.size() > 0);
		   cout << "O Custo total da ext linear e: " << custoTotal << endl;
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

	list<Vertice*> *ext = g.Step1And2();
	g.showExtLinear(ext);
	g.Step3(ext);

	getchar();
}