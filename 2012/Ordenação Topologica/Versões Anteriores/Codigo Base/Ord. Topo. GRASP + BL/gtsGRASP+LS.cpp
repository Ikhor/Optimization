#include <iostream>
#include <list>
#include <vector>
#include <fstream>
#include <stack>
#include <float.h>
#include <time.h>
#include <algorithm>

using namespace std;

/*
* Copiada Estrutura de Wu2V13 da Pasta Codigo\Novos 2012\OrdTopEwu2
*/


class Infos{

private:
	int indiceVertice;
	double CustoCRAUltimoElemento;

public:
	Infos(){
	}
	Infos(int i,int c){
		indiceVertice = i;
		CustoCRAUltimoElemento = c;
	}
	double getCustoCRAUltimoElemento(){
		return CustoCRAUltimoElemento;
	}
	int getindiceVertice(){
		return indiceVertice;
	}
	void setCustoCRAUltimoElemento(double c){
		CustoCRAUltimoElemento = c;
	}
	void setIndiceVertice(int i){
		indiceVertice = i;
	}
};

bool compare(Infos A,Infos B){
	if(A.getCustoCRAUltimoElemento() < B.getCustoCRAUltimoElemento())
		return true;
	return false;
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
			Custo->at(i) = DBL_MAX;
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
			if (custo < DBL_MAX - 1.0)
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

	// Exibe o conteudo de uma cadeia, representada como uma lista
	void exibirCadeia(list<Vertice*> *extlinear){
		for(list<Vertice*>::iterator it = extlinear->begin(); it != extlinear->end(); it++)
			cout << (*it)->getIndice() << " ";
		cout << endl;
	}

	list<Vertice*>* gtsRandom(){

		   list<Vertice*> *ExtLinear = new list<Vertice*>();

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

		   //Ordenação topologica por caixas
		   bool PrimeiraSelecao = true;
		   while(caixas->at(0).size() > 0) {   
			   //(Novo)Parte Seleção dos melhores
			   Infos *myInfos = new Infos[caixas->at(0).size()];
			   list<Vertice*>::iterator itV = caixas->at(0).begin();
			   for(int i = 0; i < caixas->at(0).size(); i++){
				   myInfos[i].setIndiceVertice((*itV)->getIndice());
				   if(PrimeiraSelecao == true){
					   myInfos[i].setCustoCRAUltimoElemento(0);
				   }else{
					   double custo;
					   (*itV)->obterCustoParaVertice(ExtLinear->front()->getIndice(),custo);
					   myInfos[i].setCustoCRAUltimoElemento(custo);
				   }
				   itV++;
			   }
			   PrimeiraSelecao = false;
			   vector<Infos> HeapCompleta(myInfos,myInfos+caixas->at(0).size());

			   int Melhores40Per = HeapCompleta.size()*0.2;
			   Melhores40Per++;//Adicionado +1 para garantir que haverá pelo menos 1 elemento na heap dos melhores
			   Infos *Melhores = new Infos[Melhores40Per];

			   vector<Infos> Escolha(Melhores,Melhores);
			   
			   make_heap(HeapCompleta.begin(),HeapCompleta.end(),compare);
			   make_heap(Escolha.begin(),Escolha.end(),compare);

			   int i = 0;
			   while(i < Melhores40Per){
				   //cout << "initial max heap   : " << HeapCompleta.front().getCustoCRAUltimoElemento() << endl;
				   pop_heap (HeapCompleta.begin(),HeapCompleta.end(),compare); 
				   Escolha.push_back(HeapCompleta.back());
				   push_heap (Escolha.begin(),Escolha.end(),compare);
				   HeapCompleta.pop_back();
				   i++;
			   }
			   //cout << HeapCompleta.front().getCustoCRAUltimoElemento() << endl;
			   while(HeapCompleta.size() > 0){
				  
				   if(HeapCompleta.front().getCustoCRAUltimoElemento() < Escolha.front().getCustoCRAUltimoElemento()){
					   
					   pop_heap (HeapCompleta.begin(),HeapCompleta.end(),compare);
					   Escolha.push_back(HeapCompleta.back());
					   push_heap (Escolha.begin(),Escolha.end(),compare);	  
					   HeapCompleta.pop_back();
					   pop_heap (Escolha.begin(),Escolha.end(),compare);
					   Escolha.pop_back();
				   }		  
				   else{
					   pop_heap(HeapCompleta.begin(),HeapCompleta.end(),compare); 
					   HeapCompleta.pop_back();
				   }
			   }
			   //cout << "ERRO \n";
			   //Parte Aleatorio
			   srand((unsigned)time(NULL));	

			   vector<Infos>::iterator SelecionarAleatorioMelhores = Escolha.begin();
			   advance(SelecionarAleatorioMelhores,rand()%Escolha.size());
			   
			   list<Vertice*>::iterator it2;//BuscaVertice selecionado aleatorio
			   Vertice * verticeGrauZero;
			   for(it2 = caixas->at(0).begin(); it2 != caixas->at(0).end(); it2++)
				   if((*it2)->getIndice() == (*SelecionarAleatorioMelhores).getindiceVertice()){
					   verticeGrauZero = (*it2);
					   break;
				   }
			   

			   //Nao mexer abaixo
			   list<Arco*>::iterator it = verticeGrauZero->getListaDeEntrada()->begin();
			   for ( ;it != verticeGrauZero->getListaDeEntrada()->end(); it++) {
				   int ondeInicioEsta = NaCaixa[(*it)->getInicio()->getIndice()];
				   caixas->at(ondeInicioEsta).remove((*it)->getInicio());
				   caixas->at(ondeInicioEsta-1).push_front((*it)->getInicio());
				   NaCaixa[(*it)->getInicio()->getIndice()] = ondeInicioEsta-1;
			   }
			   ExtLinear->push_front(verticeGrauZero);
			   caixas->at(0).remove(verticeGrauZero);
		   }
		   delete NaCaixa;
		   delete caixas;
		   return ExtLinear;
	}

	double Custo(Vertice *A,Vertice *B){
		double retorno;
		if(A->obterCustoParaVertice(B->getIndice(),retorno)){
			//cout << A->getIndice() << " " << B->getIndice() << " " << retorno << endl;
			return retorno;
		}
		else
			//cout << A->getIndice() << " " << B->getIndice() << " " << DBL_MAX<< endl;
			return DBL_MAX;
	}
	double Custo(Vertice *X,Vertice *A,Vertice *B,Vertice *Y){
		double XA,AB,BY;
		if(X->obterCustoParaVertice(A->getIndice(),XA) &&
		A->obterCustoParaVertice(B->getIndice(),AB) &&
		B->obterCustoParaVertice(Y->getIndice(),BY) ){
		//	cout << X->getIndice() << " " << A->getIndice() << " " << B->getIndice() << " " << Y->getIndice() << " " << XA+AB+BY << endl;
			return XA+AB+BY;
		}
		else
		//	cout << X->getIndice() << " " << A->getIndice() << " " << B->getIndice() << " " << Y->getIndice() << " "  << DBL_MAX << endl;
			return DBL_MAX;
	}

	void LocalSearch(list<Vertice*> *extlinear){
		bool houveTroca;
		do{
			houveTroca = false;
			vector<Vertice*> LS(extlinear->size());
			int j = 0;
			for(list<Vertice*>::iterator itVertice = extlinear->begin();itVertice != extlinear->end(); itVertice++,j++)
				LS.at(j) = (*itVertice);

			if(Custo(LS.at(0),LS.at(1)) > Custo(LS.at(1),LS.at(0))){
				Vertice *Aux = LS.at(0);
				LS.at(0) = LS.at(1);
				LS.at(1) = Aux;
				houveTroca = true;	
			}
			int i = 1;
			for(;i < LS.size()-2;i++)
				if(Custo(LS.at(i-1),LS.at(i),LS.at(i+1),LS.at(i+2)) > 
					Custo(LS.at(i-1),LS.at(i+1),LS.at(i),LS.at(i+2))){
					Vertice *Aux = LS.at(i);
					LS.at(i) = LS.at(i+1);
					LS.at(i+1) = Aux;
					houveTroca = true;	
				}
				if(Custo(LS.at(i),LS.at(i+1)) > Custo(LS.at(i+1),LS.at(i))){
					Vertice *Aux = LS.at(i);
					LS.at(i) = LS.at(i+1);
					LS.at(i+1) = Aux;
					houveTroca = true;	
				}
				extlinear->clear();
				for(int i = 0;i < LS.size();i++)
					extlinear->push_back(LS.at(i));
		}while(houveTroca != false);
	}

};

int ContabilizarExtLinear(list<Vertice*> ext){

	double custo = 0, aux;
	list<Vertice*>::iterator it = ext.begin();
	list<Vertice*>::iterator iter = ext.begin();

	int i = 0;
	for( ; iter != --ext.end(); it++){
		iter ++;
		(*it)->obterCustoParaVertice((*iter)->getIndice(),aux);
		custo += aux;
	}

	return (int)custo;
}
int GTS(Poset p){
	list<Vertice*> *ext = p.gtsRandom();
	//p.exibirCadeia(ext);
	int custo = ContabilizarExtLinear(*ext);
	//cout << "Custo antes da Local Search: " << custo << endl;
	cout << custo << ",";
	p.LocalSearch(ext);
	//p.exibirCadeia(ext);
	custo = ContabilizarExtLinear(*ext);
	return custo;
}

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

bool auditarExtLinear(list<Vertice*> *ext) {

	vector<bool> JaInserido(ext->size());

	list<Vertice*>::iterator itV = ext->begin();
	while (itV!= ext->end()) {
		JaInserido[(*itV)->getIndice()] = true;
		//Para cada um da lista de chegada verificar se ja foi inserido caso não retorna falso
		if ((*itV)->getListaDeEntrada()->size() != 0){
			list<Arco*>::iterator itArco = (*itV)->getListaDeEntrada()->begin();
			while (itArco != (*itV)->getListaDeEntrada()->end()) {
				if (JaInserido[(*itArco)->getInicio()->getIndice()] == false) {
					cout << "ERRO: " << (*itV)->getIndice() << " aparece antes de " << (*itArco)->getInicio()->getIndice() << endl;
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
	char* nomeArquivo = new char[1+strlen(args[1])];
	strcpy(nomeArquivo, args[1]);
	Poset *p = construirPoset(nomeArquivo);


	clock_t ini = clock();
	int custoOT = GTS(*p);
	clock_t fim = clock();
	double tempo = double(fim-ini)/CLK_TCK;

	cout << nomeArquivo << "," << custoOT << endl; //<< "," << tempo;
	//system("pause");
}
