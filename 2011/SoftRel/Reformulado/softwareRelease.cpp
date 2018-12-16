#include <iostream>
#include <list>
#include <vector>
#include <fstream>
#include <stack>
#include <float.h>
#include <time.h>

using namespace std;

class Vertice;
bool compare_nocase (Vertice *first, Vertice *second);


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
	int GrauDeSaida;
	list<Arco*> *ListaDeEntrada;
	int Custo;
	int Rank;

public:
    Vertice(int i){
		Indice = i;
		GrauDeSaida = 0;
		ListaDeEntrada = new list<Arco*>;
		Custo = 0;
	}

	int getRank(){
		return Rank;
	}

	void setRank(int rank){
		Rank = rank;
	}

	int  getGrauDeSaida() {
		return GrauDeSaida;
	}
	void adicionarArcoDeEntrada(Arco *a) {
         ListaDeEntrada->push_back(a);
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

	list<Arco*>* getListaDeEntrada() {
		return ListaDeEntrada;
	}

	void setCusto(int custo){
		Custo = custo;
	}

	int getCusto(){
		return Custo;
	}
};

class GrafoDirecionado {
private:
	vector<Vertice*> *Vertices;

public:
	GrafoDirecionado(int numVertices){
		Vertices = new vector<Vertice*>(numVertices);
		for (int i = 0; i < numVertices; i++) {
			Vertices->at(i) = new Vertice(i);
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
				for(list<Arco*>::iterator it = verticeOrigem->getListaDeEntrada()->begin(); it != verticeOrigem->getListaDeEntrada()->end();it++)
					if((*it)->getFinal() == verticeDestino)
						return NULL;
				Arco *arco = new Arco(verticeOrigem , verticeDestino);
				verticeOrigem->incrementarGrauDeSaida();
				verticeDestino->adicionarArcoDeEntrada(arco);
				return arco;
			}
		}
		return NULL;
	}

	void removerArco(Arco *arco) {
		Vertice *fim = arco->getFinal();
		Vertice *inicio = arco->getInicio();
		fim->decrementarGrauDeSaida();
	}

	vector<Vertice*>* getVertices(){
		return Vertices;
	}

};

class Poset : public GrafoDirecionado {

	int budgetRelease;
	vector<int> *custoPorRelease;

public:
	Poset(int numVertices,int budget,int *custo)
		: GrafoDirecionado(numVertices) {
			budgetRelease = budget;
			custoPorRelease = new vector<int>(budget);
			for(int i = 0;i < budget ; i++)
				custoPorRelease->at(i) = custo[i];
	}

	// Exibe o conteudo de uma cadeia, representada como uma lista
	void exibirCadeia(vector<list<Vertice*>> *extlinear){
		for(int i = 0; i < budgetRelease ; i++){
			cout << "Release " << i+1 << ": ";
			for(list<Vertice*>::iterator it = extlinear->at(i).begin(); it != extlinear->at(i).end(); it++)
				cout << (*it)->getIndice() << " ";
		cout << endl;
		}
	}

	vector<list<Vertice*>>* AlgoritmoOrdenacaoTopologica(){

		   vector<list<Vertice*>> *ExtLinear = new vector<list<Vertice*>>(budgetRelease);

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
		   int custo;

		   for(int i = 0; i < budgetRelease; i++){
			   custo = 0;
			   bool naoConseguiuSelecionar = false;
			   while(naoConseguiuSelecionar == false){
				   caixas->at(0).sort(compare_nocase);
				   list<Vertice*>::iterator requerimentoSelecionado = caixas->at(0).begin();
				   Vertice * verticeGrauZero;
				   bool selecaoFeita = false;
				   while((selecaoFeita == false) && (requerimentoSelecionado != caixas->at(0).end())){
					   verticeGrauZero = (*requerimentoSelecionado);
					   int aux = custo;//Aux será o custo atual
					   aux += verticeGrauZero->getCusto();// Aux receberá o custo atual + o valor caso haja a inserção do novo requerimento selecionado
					   if(aux <= custoPorRelease->at(i))
						   selecaoFeita = true;
					   else
					   requerimentoSelecionado++;
				   }
				   if(selecaoFeita == true){
					   list<Arco*>::iterator it = verticeGrauZero->getListaDeEntrada()->begin();
					   for ( ;it != verticeGrauZero->getListaDeEntrada()->end(); it++) {
						   int ondeInicioEsta = NaCaixa[(*it)->getInicio()->getIndice()];
						   caixas->at(ondeInicioEsta).remove((*it)->getInicio());
						   caixas->at(ondeInicioEsta-1).push_front((*it)->getInicio());
						   NaCaixa[(*it)->getInicio()->getIndice()] = ondeInicioEsta-1;
					   }
					   ExtLinear->at(i).push_front(verticeGrauZero);
					   caixas->at(0).remove(verticeGrauZero);
					   custo +=  verticeGrauZero->getCusto();
				   }
				   else
					    naoConseguiuSelecionar = true;
			   }
		   }
			   delete NaCaixa;
			   delete caixas;
			   return ExtLinear;
	}


	void ContabilizarExtLinear(vector<list<Vertice*>> ext){
		int custo, aux;
		for(int i = 0 ; i < budgetRelease ; i++){
			custo = 0;
			list<Vertice*>::iterator it = ext.at(i).begin();
			for( ; it != ext.at(i).end(); it++){
				aux = (*it)->getCusto();
				custo += aux;
			}
			cout << "Custo da Release " << i+1 << " eh " << custo << " O máximo seria " << custoPorRelease->at(i) << endl;
		}
	}
};

Poset* construirPoset(char* arq){

	int NumeroDeVertices, NumeroDeArcosDiretos,budget,aux1,aux2,M;
	int *budgetCustos,*risco,**importanciaRequisito,*importanciaCliente;
	ifstream fin;
	fin.open(arq);

	fin >> budget;
	budgetCustos = new int[budget];
	for(int i = 0; i < budget ; i++){
		fin >> aux1;
		budgetCustos[i] = aux1;
	}
	fin >> NumeroDeVertices;
    Poset *p = new Poset(NumeroDeVertices,budget,budgetCustos);
	risco = new int[NumeroDeVertices];

	for (int i = 0; i < NumeroDeVertices; i++) {
		fin >> risco[i];//Risco
		fin >> aux1;//Custo do requisito
		p->getVertice(i)->setCusto(aux1);
	}

	fin >> M;//Numero de Clientes
	importanciaCliente = new int[M];
	for (int i=0; i<M; i++)
		fin >> importanciaCliente[i];

	importanciaRequisito = new int*[M];
	for (int i=0; i < M; i++) {
		importanciaRequisito[i] = new int[NumeroDeVertices];
	}

	for (int i=0; i<M; i++)
		for (int j=0; j<NumeroDeVertices; j++){
			fin >> aux2;
			importanciaRequisito[i][j] = aux2;
		}

	for (int i = 0; i < NumeroDeVertices; i++) {
		int somatorioRequisito = 0;
		for(int j = 0; j < M ; j++)
			somatorioRequisito =+ importanciaCliente[j]*importanciaRequisito[j][i];
		p->getVertice(i)->setRank(somatorioRequisito + risco[i]);
	}

	fin >> NumeroDeArcosDiretos;
	for (int i = 0; i < NumeroDeArcosDiretos; i++) {
		fin >> aux1;//Origem
		fin >> aux2;//Destino
		p->adicionarArco( aux1 , aux2 );
	}

	fin.close();

	delete budgetCustos;
	delete risco;
	for (int i=0; i<M; i++)
		delete importanciaRequisito[i];
	delete importanciaRequisito;
	delete importanciaCliente;

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

void OrdenacaoTopologica(Poset p){
	vector<list<Vertice*>> *ext = p.AlgoritmoOrdenacaoTopologica();
	p.ContabilizarExtLinear(*ext);
	p.exibirCadeia(ext);
}
bool compare_nocase (Vertice *first, Vertice *second)
{
	if (first->getRank() < second->getRank())
		return true;
    return false;
}


int main(int argc, char **args) {
	char* nomeArquivo = new char[1+strlen(args[1])];
	strcpy(nomeArquivo, args[1]);
	Poset *p = construirPoset(nomeArquivo);

	clock_t ini = clock();
    OrdenacaoTopologica(*p);
	clock_t fim = clock();
	double tempo = double(fim-ini)/CLK_TCK;
//	cout << custoOT << "\n";
//	cout << nomeArquivo << "," << custoOT << "," << tempo;

	getchar();
}
