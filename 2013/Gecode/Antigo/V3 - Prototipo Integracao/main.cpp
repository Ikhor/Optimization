#include <fstream>
#include "Poset.h"
#include "MCLEModel.h"

// ---------------------------------------------------------------------------------
// Inclusões Grasp
#include <iostream>
#include <time.h>
#include <algorithm>
#include "RegistroOT.h"
#include "BuscaLocal.h"
// ---------------------------------------------------------------------------------

using namespace std;

Poset* construirPoset(char* arq, int ** &custos, int &incomparabilidades) {
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
		//model->arco( aux1 , aux2 );
	}

	incomparabilidades = 0;
	custos = new int*[ NumeroDeArcosComCustos ];
	for (int i = 0; i < NumeroDeArcosComCustos; i++) {
		fin >> aux1;
		fin >> aux2;
		fin >> aux3;
		p->adicionarArcoDeCusto( aux1 , aux2 , aux3 );
		++ incomparabilidades;
		custos[i] = new int[3];
		custos[i][0] = aux1;
		custos[i][1] = aux2;
		custos[i][2] = aux3;
		//model->incomparabilidade( aux1 , aux2, aux3 );
	}

	fin.close();

	// Impor intervalos baseados nos tamanhos de upset e downset
	/*int tD, tU;
	for (int i = 0; i < NumeroDeVertices; i++) {
		tD = p->tamanhoDownset( i );
		tU = p->tamanhoUpset( i );
		model->inserirLimites(i, tD, tU);
	}*/

	return p;
}

bool compare(Infos A,Infos B){
	if(A.getCustoCRAUltimoElemento() < B.getCustoCRAUltimoElemento())
		return true;
	return false;
}

list<Vertice*>* OrdenacaoTopologica(Poset &p,float &valorAlfa){

	double avgSizeRCL = 0;
	int NbRandomSelections = 0;

	list<Vertice*> *ExtLinear = new list<Vertice*>();

	//Descobre o maior grau
	int maiorGrau = -1;

	for(vector<Vertice*>::iterator it = p.getVertices()->begin()++;it != p.getVertices()->end();it++){
		if((*it)->getGrauDeSaida() > maiorGrau)
			maiorGrau = (*it)->getGrauDeSaida();
	}
	
	//Preenche as "caixas"
	int *NaCaixa = new int[p.getNumeroDeVertices()];
	vector<list<Vertice*>> *caixas = new vector<list<Vertice*>>(maiorGrau+1);

	//Vector de iteradores
	list<Vertice*>::iterator *CaixaIterator = new list<Vertice*>::iterator[p.getNumeroDeVertices()];

	for(vector<Vertice*>::iterator it = p.getVertices()->begin()++;it != p.getVertices()->end();it++) {
		int indiceDaCaixa = (*it)->getGrauDeSaida();
		caixas->at(indiceDaCaixa).push_back((*it));
		NaCaixa[(*it)->getIndice()] = indiceDaCaixa;
		//Parte Iterator
		auto itAuxiliarParaCapturarFinal = caixas->at(indiceDaCaixa).end();
		CaixaIterator[(*it)->getIndice()] = --itAuxiliarParaCapturarFinal;
	}
	//Ordenação topologica por caixas
	bool PrimeiraSelecao = true;
	while(caixas->at(0).size() > 0) {
		//Seleção dos melhores
		Infos *myInfos = new Infos[caixas->at(0).size()];
		list<Vertice*>::iterator itV = caixas->at(0).begin();
		for(unsigned int i = 0; i < caixas->at(0).size(); i++){
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
		int Alfa;
		if(valorAlfa == 1)
			Alfa = HeapCompleta.size();
		else{
			Alfa = int(HeapCompleta.size()*valorAlfa);
			Alfa++;//Adicionado +1 para garantir que haverá pelo menos 1 elemento na heap dos melhores
		}
		Infos *Melhores = new Infos[Alfa];
		vector<Infos> Escolha(Melhores,Melhores);
		make_heap(HeapCompleta.begin(),HeapCompleta.end(),compare);
		make_heap(Escolha.begin(),Escolha.end(),compare);
		int i = 0;
		while(i < Alfa){
			pop_heap (HeapCompleta.begin(),HeapCompleta.end(),compare);
			Escolha.push_back(HeapCompleta.back());
			push_heap (Escolha.begin(),Escolha.end(),compare);
			HeapCompleta.pop_back();
			i++;
		}
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
		//Parte Aleatorio
		vector<Infos>::iterator SelecionarAleatorioMelhores = Escolha.begin();
		avgSizeRCL += Escolha.size();
		NbRandomSelections ++;
		advance(SelecionarAleatorioMelhores,rand()%Escolha.size());
		list<Vertice*>::iterator it2;//BuscaVertice selecionado aleatorio
		Vertice * verticeGrauZero;
		for(it2 = caixas->at(0).begin(); it2 != caixas->at(0).end(); it2++)
			if((*it2)->getIndice() == (*SelecionarAleatorioMelhores).getindiceVertice()){
				verticeGrauZero = (*it2);
				break;
			}
			list<Arco*>::iterator it = verticeGrauZero->getListaDeEntrada()->begin();
			for ( ;it != verticeGrauZero->getListaDeEntrada()->end(); it++) {
				int ondeInicioEsta = NaCaixa[(*it)->getInicio()->getIndice()];
				//remoção
				caixas->at(ondeInicioEsta).erase(CaixaIterator[(*it)->getInicio()->getIndice()]);
				caixas->at(ondeInicioEsta-1).push_front((*it)->getInicio());
				NaCaixa[(*it)->getInicio()->getIndice()] = ondeInicioEsta-1;
				//Atualização do Iterador
				CaixaIterator[(*it)->getInicio()->getIndice()] = caixas->at(ondeInicioEsta-1).begin();
			}
			ExtLinear->push_front(verticeGrauZero);
			caixas->at(0).remove(verticeGrauZero);
			verticeGrauZero->posicao = p.getNumeroDeVertices() + 1 - ExtLinear->size();
			delete myInfos;
			delete Melhores;
	}

	avgSizeRCL /= (double)NbRandomSelections;
	if ((avgSizeRCL < 1.5f) && (valorAlfa <= 0.6f)) {
		valorAlfa += .05f; // Note the valorAlfa foi passado por referencia agora!
		if (valorAlfa > 0.6f) valorAlfa = 0.6f;
		cout << "avgSizeRCL = " << avgSizeRCL  << endl;
		cout << "Aumentando o de alfa em 0.05: alfa = " << valorAlfa << endl;
	}

	delete NaCaixa;
	delete caixas;
	return ExtLinear;
}

int BuscaLocal(Poset p,list<Vertice*> *ext){

	double custoTotal = DBL_MAX;

	int ultimaMudanca = 0;
	int indiceUp;
	int indiceDown;
	int t;
	int n = p.getNumeroDeVertices();


	for(t = 0; t < n; t++){
		p.IntervaloDownUpSet(t,indiceDown,indiceUp);		
		p.melhorMovimento(ext,t,indiceDown,indiceUp);
	}

	custoTotal = p.ContabilizarExtLinear(*ext);			

	return (int)custoTotal;
}


double GRASPBL(Poset p,float alfa,int qtdRodadas){

	int i = 0;
	double custoTotal = DBL_MAX;
	while(i < qtdRodadas){
		//Chamada ao GRASP
		list<Vertice*> *ext = OrdenacaoTopologica(p,alfa);
		double custo = p.ContabilizarExtLinear(*ext);

		if(p.auditarExtLinear(ext) == false)
			cout << "ERRO geração de solução inválida"<< endl;
		
		//cout << "\nCusto Antes: " << custo << endl;
		int ultimaMudanca = 0;
		int indiceUp;
		int indiceDown;
		int t;
		int n = p.getNumeroDeVertices();
		bool parada = true;
		while(parada){
			for(t = 0; t < n; t++){
				int posicao = (ultimaMudanca+t)%n;
				p.IntervaloDownUpSet(posicao,indiceDown,indiceUp);		
				if(p.melhorMovimento(ext,posicao,indiceDown,indiceUp)){
					if(ultimaMudanca != posicao){
						ultimaMudanca = posicao;
						//cout << posicao << endl;
					}else{
						parada = false;
					}
					break;
				}
			}
			if(t == n)
				break;
		}
		if(p.auditarExtLinear(ext) == false){
			cout << "ERRO Busca local gerou solução inválida"<< endl;
		}else{
			if(custoTotal > p.ContabilizarExtLinear(*ext)){
				custoTotal = p.ContabilizarExtLinear(*ext);
				//cout << " % de melhoria =  "<< custoTotal/custo << endl;
			}
		}
		i++;
		//cout << "Rodada I: " << i << endl;
	}
	
	return custoTotal;
}

int main(){

	int ** custos;
	int incomparabilidades;
	srand((unsigned)time(NULL));

	char* nomeArquivo = "rp_n25_p0.80_s1.txt";
	float alfa = 0.2f;
	int qtdRodadas = 10;

	Poset *poset = construirPoset( nomeArquivo , custos, incomparabilidades);
	
	double CustoFuncObj = GRASPBL(*poset,alfa,qtdRodadas);

	int opt = CustoFuncObj;
	cout << "CustoFuncObj Inicial: " << CustoFuncObj << endl;

	bool repet = true;
	list<Vertice*> *ext = new list<Vertice*>;

	while(repet)
	{
		MCLE * model = new MCLE(poset, custos, incomparabilidades,opt-1);

		// Instantiate a model
		BAB<MCLE> bab(model);

		MCLE* s;

		if(s = bab.next ()){

			cout << s->getOjbFunctionValue() << "  " << opt << endl;
			if (s->getOjbFunctionValue() < opt)
				opt = s->getOjbFunctionValue();

			cout << "opt = " << opt << endl;
			
			s->SaveExt(ext,*poset);
			cout << "Auditoria Gecode\n";
			poset->auditarExtLinear(ext);
			cout << "Extensao gerada pelo Gecode\n";
			poset->exibirCadeia(*ext);
		
			cout << "\n\n\nBusca Local\n";
			int valor = BuscaLocal(*poset,ext);
			cout << "Auditoria Busca Local\n";
			poset->auditarExtLinear(ext);
			cout << "Extensao gerada pela Busca Local\n";
			poset->exibirCadeia(*ext);
			cout << "otimo BL " << valor << endl;

			if(valor < opt)
				opt = valor;
		}
		else
			repet = false;
		
		delete model; // model has been copied by dfs
		system("pause");

	}


	system("pause");
}