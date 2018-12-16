#include <iostream>
#include <list>
#include <vector>
#include <map>
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
	   //Função abaixo responsável pela busca em profundidade
	   int calcAltura(Vertice *no){
		    int retorno = -1;
			for( list<Arco*>::iterator iter = no->getListaDeAdjacencia()->begin(); iter !=no->getListaDeAdjacencia()->end() ; iter++ ){
				int aux = calcAltura((*iter)->getFinal());
				if(aux > retorno)
					retorno = aux;					
				}
		return ++retorno;
	  }
	   vector<list<Vertice*>>* Step1(){
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
		   return layer;
	   }
	   /*
		Busca em profundidade pela maior altura do grafo entre os vertices pertencentes ao
		layer denominado "HO"
		
					//-------------------Melhorar a captura da extlinear----------------\\
	
		A captura abaixo está fazendo em O(h*m) 
		Onde h é a maior altura do grafo e m são arestas necessárias a pecorrer para achar a maior altura
		*/
	   list<Vertice*>* Step2(vector<list<Vertice*>> *layer){
		
		int ValorDaMaiorAltura = 0;
		int NumeroDoVertice;

		for( list<Vertice*>::iterator iter =  layer->at(0).begin() ; iter != layer->at(0).end() ; iter++ ){
			int aux = calcAltura((*iter));
			if( aux > ValorDaMaiorAltura ){
				ValorDaMaiorAltura = aux;
				NumeroDoVertice = (*iter)->getIndice();
			}
		}

		list<Vertice*> *extlinear = new list<Vertice*>;
		Vertice *Aux = getVertices()->at(NumeroDoVertice);
		extlinear->push_back(Aux);
		
		while(extlinear->size() <= ValorDaMaiorAltura){

		int ValorDaMaiorAlturaAux = -1;
		NumeroDoVertice;

			for( list<Arco*>::iterator iter = Aux->getListaDeAdjacencia()->begin() ; iter != Aux->getListaDeAdjacencia()->end() ; iter++ ){
				int b = calcAltura((*iter)->getFinal());
				if(b > ValorDaMaiorAlturaAux){
				ValorDaMaiorAlturaAux = b;
				NumeroDoVertice = (*iter)->getFinal()->getIndice();
				}
			}
			Aux = getVertices()->at(NumeroDoVertice);
			extlinear->push_back(Aux);
		}
		return extlinear;
	   }
	   /*
	   Funções abaixo tem apenas a funcionalidade de exibir os resultados pela Step's do algoritmo
	   */
	   void showExtLinear(list<Vertice*> *extlinear){
		for(list<Vertice*>::iterator it = extlinear->begin(); it != extlinear->end(); it++)
			cout << (*it)->getIndice() << " ";
		cout << endl;
	}
	   void showLayers(vector<list<Vertice*>>* layer){
		   cout << "Step 1\n";
		   for(int i = 0; i < layer->size()-1;i++){
			   cout << "H"<< i << endl;
			   for(list<Vertice*>::iterator iter =  layer->at(i).begin(); iter != layer->at(i).end();iter++)
				   cout << (*iter)->getIndice() << " ";
			   cout << endl;
		   }
	   }
	  
	   void Step3(list<Vertice*> *ext){
	   
		   map<int,double>::iterator it;
		   list<Vertice*> auxiliar;
		   list<Vertice*>::iterator ite;
		   list<Vertice*>::iterator save1;
		   double custoTotal = 0;

		   for(int i = 0 ; i < getNumeroDeVertices() ; i++)
			   auxiliar.push_back(getVertice(i));

		   for(ite = ext->begin() ; ite != ext->end() ; ite++)
			   auxiliar.remove(*ite);		


		   do{
				  double custo = INT_MAX;

				  for( ite = ext->begin() ; ite != ext->end() ; ite++){
				   it = auxiliar.front()->MapaDeCustos[0].find((*ite)->getIndice());
				   if(ite == ext->begin() && it !=  auxiliar.front()->MapaDeCustos[0].end()){
					   	  custo = it->second;
						  save1 = ite;   
				   }

				   if(it != auxiliar.front()->MapaDeCustos[0].end() && ite !=  ext->begin()){ 	   
					   double custoTemp = it->second;
					 	   ite--;
						   it = (*ite)->MapaDeCustos[0].find(auxiliar.front()->getIndice());
						   if(it !=  (*ite)->MapaDeCustos[0].end()){
							   custoTemp += it->second;
							   if(custoTemp < custo){
								   custo = custoTemp;
								   save1 = ++ite;
								   ite--;
							   }
						   }
						   ite++;  
					}
				   if(--ext->end() == ite){
						it = (*ite)->MapaDeCustos[0].find(auxiliar.front()->getIndice());
						if(it !=  (*ite)->MapaDeCustos[0].end() && custo > it->second){
							custo = it->second;
							save1 = ext->end();
						}
					}      	
			   
			   }
			   cout << "Menor custo: " << custo << endl;
			   ext->insert(save1,auxiliar.front());
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

	vector<list<Vertice*>> *layer = g.Step1();
	g.showLayers(layer);

	list<Vertice*> *ext = g.Step2(layer);
	g.showExtLinear(ext);

	g.Step3(ext);

	getchar();
}