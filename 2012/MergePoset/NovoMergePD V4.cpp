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
			Custo->at(i) = INT_MAX;
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
			if (custo < INT_MAX - 1.0)
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


	vector<vector<Vertice*>>* CapturarCadeias(){

		   vector<vector<Vertice*>> *ExtLinear = new vector<vector<Vertice*>>();

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
		   }

		   list<Vertice*> CadeiaInversa;
		   bool NovaCadeia = false;
		   int indiceAtual = 0;
		   ExtLinear->resize(indiceAtual+1);
		   ExtLinear->at(indiceAtual).push_back(getVertice(getNumeroDeVertices()));

		   //Ordenação topologica por caixas			   
		   while(caixas->at(0).size() > 0) {   
			   
			   Vertice * verticeGrauZero = caixas->at(0).back();
			   //   cout << verticeGrauZero->getIndice();
			   if(NovaCadeia){  
				   for(auto Cadeia = CadeiaInversa.begin(); Cadeia != CadeiaInversa.end(); Cadeia++)
					   ExtLinear->at(indiceAtual).push_back((*Cadeia));
				   CadeiaInversa.clear();
				   indiceAtual++;
				   ExtLinear->resize(indiceAtual+1);
				   ExtLinear->at(indiceAtual).push_back(getVertice(getNumeroDeVertices()));
			   }

			   NovaCadeia = true;
			   auto it = verticeGrauZero->getListaDeEntrada()->begin();
			   for ( ;it != verticeGrauZero->getListaDeEntrada()->end() ; it++) {
				   int ondeInicioEsta = NaCaixa[(*it)->getInicio()->getIndice()];
				   caixas->at(ondeInicioEsta).remove((*it)->getInicio());//Lembrete para trocar remoção
				   caixas->at(ondeInicioEsta-1).push_back((*it)->getInicio());
				   NaCaixa[(*it)->getInicio()->getIndice()] = ondeInicioEsta-1;
				   if(ondeInicioEsta-1 == 0)
					   NovaCadeia = false;
			   }
			   CadeiaInversa.push_front(verticeGrauZero);
			   caixas->at(0).remove(verticeGrauZero);//Lembrete para trocar remoção
		   }

		   //Ultima Cadeia
		   for(auto Cadeia = CadeiaInversa.begin(); Cadeia != CadeiaInversa.end(); Cadeia++)
			   ExtLinear->at(indiceAtual).push_back((*Cadeia));

		   delete NaCaixa;
		   delete caixas;
		   return ExtLinear;
	}
	
	void ImprimeCadeias(vector<vector<Vertice*>> *Cadeias){
		for(auto itCadeia = Cadeias->begin(); itCadeia != Cadeias->end(); itCadeia++){
			auto itElementoCadeia = (*itCadeia).begin();
			itElementoCadeia++;//Tirando o vertice de ajuste
			for(; itElementoCadeia != (*itCadeia).end() ; itElementoCadeia++)
				cout << (*itElementoCadeia)->getIndice() << " ";
			cout << endl;
		}
	}

// Exibe o conteudo de uma cadeia, representada como uma lista
	void exibirCadeia(vector<Vertice*> *extlinear){
		auto it = extlinear->begin();
		it++;
		for(; it != extlinear->end(); it++)
			cout << (*it)->getIndice() << " ";
		cout << endl;
	}
	
	//Funções Novas
	double custoArcoMerge(int i, int j,const vector<Vertice*> *lista1,const vector<Vertice*> *lista2,char c)
	{
		double custo;
		if(c == 'A')
			lista2->at(j)->obterCustoParaVertice(lista1->at(i)->getIndice(),custo);
		else if(c == 'B')
			lista1->at(i)->obterCustoParaVertice(lista2->at(j)->getIndice(),custo);
		return custo;
	}
	
	//Algoritmo Liu1
	vector<Vertice*>* Liu1(vector<Vertice*> *lista1,vector<Vertice*> *lista2){

		int NumeroEstado = (lista1->size())*(lista2->size());

		vector<double> valorA(NumeroEstado);
		vector<double> valorB(NumeroEstado);		

		//Inicializações
		for(int i = 0; i < NumeroEstado; i++){
			valorA[i] = INT_MAX;
			valorB[i] = INT_MAX;
		}

		int custoLista2 = ContabilizarExtLinear(*lista2);
		for(int i = 0; i < lista1->size(); i++)
			valorA[i*lista2->size()] = custoLista2;
		
		int custoLista1 = ContabilizarExtLinear(*lista1);
		for(int i = 0; i < lista2->size(); i++)
			valorB[i] = custoLista1;

		
		valorA[0] = 0;
		for(int i = 1; i < lista1->size()-1; i++){
			double custo;
			lista1->at(i)->obterCustoParaVertice(lista1->at(i+1)->getIndice(),custo);
			valorA[i*lista2->size()] = valorA[(i-1)*lista2->size()] + custo;
		}
		
		valorB[0] = 0;
		for(int i = 1; i < lista2->size()-1; i++){
			double custo;
			lista2->at(i)->obterCustoParaVertice(lista2->at(i+1)->getIndice(),custo);
			valorB[i] = valorB[i-1] + custo;
		}

		for(int i = 1 ; i < lista1->size() ; i++)
			for(int j = 1 ; j < lista2->size() ; j++){
				if(valorA[j+ ((i-1)*lista2->size())] < valorB[j+ ((i-1)*lista2->size())] + custoArcoMerge(i,j,lista1,lista2,'A'))
					valorA[j + (i*lista2->size())] = valorA[j+ ((i-1)*lista2->size())];
				else
					valorA[j + (i*lista2->size())] = valorB[j+ ((i-1)*lista2->size())] + custoArcoMerge(i,j,lista1,lista2,'A');

				if(valorA[(j-1)+ (i*lista2->size())] + custoArcoMerge(i,j,lista1,lista2,'B') < valorB[(j-1)+ (i*lista2->size())])
					valorB[j+ (i*lista2->size())] = valorA[(j-1)+ (i*lista2->size())] + custoArcoMerge(i,j,lista1,lista2,'B');
				else
					valorB[j+ (i*lista2->size())] = valorB[(j-1) + (i*lista2->size())];
			}			
			
			vector<Vertice*>  *extLinearResultante = new vector<Vertice*>;	
			extLinearResultante->push_back(getVertice(getNumeroDeVertices()));

			list<Vertice*> ExtInversa;

			auto A =  lista1->end();
			auto B =  lista2->end();
			A--; B--;

			//Função para montar cadeia está errada
			for(int i = NumeroEstado-1; i > 0 ;){
			//	cout << "valor de i: " << i << endl;
				bool Escolhido;//False para A e Verdadeiro para B
				if(valorA[i] < valorB[i]){			
						ExtInversa.push_front(*A);
						A--;
						Escolhido = false;
				}else{
						ExtInversa.push_front(*B);
						B--;
						Escolhido = true;
				}				
				if(Escolhido)
						i = i - 1;
				else
						i = i - lista2->size();				
			}
			
			for(auto it = ExtInversa.begin();it != ExtInversa.end(); it++)
				extLinearResultante->push_back(*it);

			return extLinearResultante;
	} 

	int ContabilizarExtLinear(vector<Vertice*> ext){
		double custo = 0, aux;
		auto it = ext.begin();
		auto iter = ext.begin();
		it++;
		iter++;

		int i = 0;
		for( ; iter != --ext.end(); it++){
			iter ++;
			(*it)->obterCustoParaVertice((*iter)->getIndice(),aux);
			custo += aux;
		}
		return (int)custo;
	}
	
	void Matlab(){
		ofstream fout;
		fout.open("Grafo.m");
		fout << "C =[" << endl;
		for(int i = 0; i < getNumeroDeVertices(); i++){
			for(int j = 0 ; j < getNumeroDeVertices(); j++){
				double custo;
				if(getVertice(i)->obterCustoParaVertice(j,custo))
					fout << custo << " ";
				else
					fout << "Inf ";
			}
			fout << endl;
		}
		fout << "];";
		fout.close();
	}
};

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
bool auditarExtLinear(vector<Vertice*> *ext, char* nomeArq) {

	vector<bool> JaInserido(ext->size());
	for(int i = 0;i < ext->size();i++)
		JaInserido[i] = false;
	
	auto itV = ext->begin();
	itV++;
	while (itV != ext->end()) {
		JaInserido[(*itV)->getIndice()] = true;
		//Para cada um da lista de chegada verificar se ja foi inserido caso não retorna falso
		if ((*itV)->getListaDeEntrada()->size() != 0){
			auto itArco = (*itV)->getListaDeEntrada()->begin();
			while (itArco != (*itV)->getListaDeEntrada()->end()) {
				if (JaInserido[(*itArco)->getInicio()->getIndice()] == false) {
					cout << "ERRO: " << (*itV)->getIndice() << " aparece antes de " << (*itArco)->getInicio()->getIndice() << endl;
					cout << "No arquivo: " << nomeArq << endl;
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
	
	//char* nomeArquivo = new char[1+strlen(args[1])];
	//strcpy(nomeArquivo, args[1]);
	
	//char* nomeArquivo = "Grafo.txt";
	char* nomeArquivo = "rp_n15_p0.80_s1.txt";
	Poset *g = construirPoset(nomeArquivo);


	clock_t ini = clock();
	vector<vector<Vertice*>> *Cadeias = g->CapturarCadeias();
	//cout << "Inicio das cadeias\n\n";

	g->ImprimeCadeias(Cadeias);

	//cout << "\nFim das cadeias\n\nInicio Do Merge\n";

	srand(12789453);

	int *perm = new int[ Cadeias->size() ];
	for (int i=0; i<Cadeias->size(); i++) perm[i] = Cadeias->size() - 1 - i;
	/*
	int t, iaux;
	for (int i=0; i<(Cadeias->size()-1); i++) {
		t = (int) ((rand()/(double)RAND_MAX) * (Cadeias->size() - 1 - i));
		//cout << t ;
		iaux = perm[i+t]; perm[i+t] = perm[i]; perm[i] = iaux;
	}*/

	vector<Vertice*> *aux = &Cadeias->at(perm[0]);
	for(int i = 1; i < Cadeias->size();i++){
		vector<Vertice*> auxi = Cadeias->at(perm[i]);
		vector<Vertice*> *Cadeia = g->Liu1(aux,&auxi);
		aux = Cadeia;
		g->exibirCadeia(aux);
		//cout << g->ContabilizarExtLinear(*aux) << endl;
	}
	clock_t fim = clock();

	auditarExtLinear(aux,nomeArquivo);
	int custo = g->ContabilizarExtLinear(*aux);	
	double tempo = double(fim-ini)/CLK_TCK;
	cout << nomeArquivo  << "," << custo << "," << tempo << endl; 

	g->Matlab();
	system("pause");
}
