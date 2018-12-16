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

	int numReleases;
	vector<int> *custoPorRelease;

public:
	Poset(int numVertices,int releases)
		: GrafoDirecionado(numVertices) {
			numReleases = releases;
			custoPorRelease = new vector<int>(numReleases);
			for(int i = 0;i < numReleases ; i++)
				custoPorRelease->at(i) = 0;
	}

	// Exibe o conteudo de uma cadeia, representada como uma lista
	void exibirCadeia(vector<list<Vertice*>> *extlinear){
		for(int i = 0; i < numReleases ; i++){
			//cout << "Release " << i+1 << ": ";
			//for(list<Vertice*>::iterator it = extlinear->at(i).begin(); it != extlinear->at(i).end(); it++)
				//cout << (*it)->getIndice() << " ";
		//cout << endl;
		}
	}

	bool auditarPreecedencia(vector<list<Vertice*>> *ext) {

	bool *JaInserido = new bool[getNumeroDeVertices()];
	for(int i = 0; i < getNumeroDeVertices();i++)
		JaInserido[i] = false;

	for(int i = 0; i < numReleases ; i++)	{
		for(auto itV = ext->at(i).begin(); itV != ext->at(i).end() ; itV++) {
			JaInserido[(*itV)->getIndice()] =  true;
			//Para cada um da lista de saida verificar se ja foi inserido caso não retorna falso
			if ((*itV)->getListaDeEntrada()->size() != 0){
				auto itArco = (*itV)->getListaDeEntrada()->begin();
				while (itArco != (*itV)->getListaDeEntrada()->end()) {
					if (JaInserido[(*itArco)->getFinal()->getIndice()] == false) {
						cout << "\nERRO NA RELEASE: " << i << endl;
						cout << "ERRO: " << (*itV)->getIndice() << " aparece antes de " << (*itArco)->getInicio()->getIndice() << endl;
						return false;
					}
					itArco++;
				}
			}
		}
		system("pause");
	}
	return true;
	}

	bool auditarSomatorio(vector<list<Vertice*>> *ext) {
		for(int i = 0; i < numReleases ; i++){	
			int somatorio = 0;
			for(list<Vertice*>::iterator itV = ext->at(i).begin(); itV != ext->at(i).end() ; itV++) 
				somatorio = somatorio + (*itV)->getCusto();
			if(somatorio > getBudget(i))
				return false;
		}
		return true;
	}

	void setBudget(int release, int b) {
		custoPorRelease->at(release) = b;
	}

	int getBudget(int release) {
		return custoPorRelease->at(release);
	}

	vector<list<Vertice*>>* AlgoritmoOrdenacaoTopologica(){

		   vector<list<Vertice*>> *ExtLinear = new vector<list<Vertice*>>(numReleases);

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

		   for(int i = 0; i < numReleases; i++){
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


	void ContabilizarExtLinear(vector<list<Vertice*>> ext,int *risco,int *score){
		int custo, aux, FObj, *x;

		x = new int[getNumeroDeVertices()];
		for (int i=0; i<getNumeroDeVertices(); i++)
			x[i] = -1;

		double scoreMedio;
		//cout << "," << numReleases;
		for(int i = 0 ; i < numReleases ; i++){
			custo = 0;
			scoreMedio = 0.0;
			list<Vertice*> releaseAtual = ext.at(i);
			list<Vertice*>::iterator it = releaseAtual.begin();
			for( ; it != releaseAtual.end(); it++){
				aux = (*it)->getCusto();
				custo += aux;
				x[(*it)->getIndice()] = i + 1;
				scoreMedio += score[(*it)->getIndice()];
			}
			scoreMedio /= (double)releaseAtual.size();
			//cout << "," << scoreMedio;
			//cout << "Custo da Release " << i+1 << " eh " << custo << " O máximo seria " << custoPorRelease->at(i) << endl;
		}

		FObj = 0;
		for(int i = 0 ; i < getNumeroDeVertices() ; i++) {
			if (x[i] != -1)
				FObj += score[i]*(numReleases - x[i] + 1) - risco[i]*x[i];
			//cout << score[i] << "  ";
		}
		//cout << endl;

		cout << "," << FObj;
		delete x;
	}
};

Poset* construirPoset(char* arq, int *&risco, int *&score){

	int NumeroDeRequisitos,NumReleases,aux1,aux2,NumClientes;
	int **importanciaRequisito,*importanciaCliente;
	ifstream fin;
	fin.open(arq);

	cout << arq;
	fin >> NumReleases;
	fin >> NumeroDeRequisitos;
	fin >> NumClientes;//Numero de Clientes

	Poset *p = new Poset(NumeroDeRequisitos,NumReleases);

	// Importancia de cada cliente
	importanciaCliente = new int[NumClientes];
	for (int i=0; i< NumClientes; i++)
		fin >> importanciaCliente[i];

	// Matriz de precedencia
	for (int i = 0; i < NumeroDeRequisitos; i++) {
		for (int j = 0; j < NumeroDeRequisitos; j++) {
			fin >> aux1; // Se arco existe ou nao
			if (aux1 == 1)
				p->adicionarArco( i , j );
		}
	}

	// Risco e importancia de cada requisito
	risco = new int[NumeroDeRequisitos];
	score = new int[NumeroDeRequisitos];
	for (int i = 0; i < NumeroDeRequisitos; i++) {
		fin >> aux1; //Custo do requisito
		fin >> risco[i]; //Risco
		p->getVertice(i)->setCusto(aux1);
	}

	// Importancia de cada requisito para cada cliente
	importanciaRequisito = new int*[NumClientes];
	for (int i=0; i < NumClientes; i++)
		importanciaRequisito[i] = new int[NumeroDeRequisitos];

	for (int i=0; i<NumClientes; i++) {
		for (int j=0; j<NumeroDeRequisitos; j++){
			fin >> aux2;
			importanciaRequisito[i][j] = aux2;
		}
	}

	// Ler budget de cada release
	for(int i = 0; i < NumReleases ; i++){
		fin >> aux1;
		p->setBudget(i, aux1);
	}

	// Calcular rank de cada requisito
	for (int i = 0; i < NumeroDeRequisitos; i++) {
		int somatorioRequisito = 0;
		for(int j = 0; j < NumClientes ; j++)
			somatorioRequisito += importanciaCliente[j]*importanciaRequisito[j][i];
		score[i] = somatorioRequisito;
		p->getVertice(i)->setRank(somatorioRequisito + risco[i]);
	}

	fin.close();

	for (int i=0; i<NumClientes; i++)
		delete importanciaRequisito[i];
	delete importanciaRequisito;
	delete importanciaCliente;

	return p;
}


void OrdenacaoTopologica(Poset p,int *risco,int *score){
	vector<list<Vertice*>> *ext = p.AlgoritmoOrdenacaoTopologica();
	p.ContabilizarExtLinear(*ext,risco,score);
	p.exibirCadeia(ext);
	if(p.auditarPreecedencia(ext) == true)
		cout << "\n Preecedencia Ok\n";
	else
		cout << "\n ERRO DE Preecedencia\n";
	if(p.auditarSomatorio(ext))
		cout << "\n BUDGET Ok\n";
	else
		cout << "\n ERRO ULTRAPASSOU BUDGET\n";
	
}
bool compare_nocase (Vertice *first, Vertice *second)
{
	if (first->getRank() > second->getRank())
		return true;
    return false;
}


int main(int argc, char **args) {
	char* nomeArquivo = new char[1+strlen(args[1])];
	strcpy(nomeArquivo, args[1]);

	int *risco, *score;
	Poset *p = construirPoset(nomeArquivo,risco,score);

	clock_t ini = clock();
    OrdenacaoTopologica(*p,risco,score);
	clock_t fim = clock();
	double tempo = double(fim-ini)/CLOCKS_PER_SEC;

	cout << "," << tempo << endl;

	getchar();
}
