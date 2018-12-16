#include <fstream>
#include "Poset.h"
#include "MCLEModel.h"

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
	}

	fin.close();

	return p;
}

bool getASolution(Poset *poset, int **costs, int incomparabilities, int *&sol, int &objValue, int upperBound, bool disregardSolution=false) {
	bool returnValue = true;

	MCLE * model;
	if (disregardSolution)
		model = new MCLE(poset, costs, incomparabilities, NULL, upperBound);
	else
		model = new MCLE(poset, costs, incomparabilities, sol, upperBound);

	BAB<MCLE> bab(model);

	MCLE *s;
	while (s = bab.next ()) { break; } // Obtem só uma solução.

	if (s != NULL) {
		s->getSolution( sol );
		objValue = s->getOjbFunctionValue();
	}
	else
		returnValue = false;

	delete s;
	delete model;
	
	return returnValue;
}

int main(){

	int ** custos;
	int incomparabilidades;
	char nome[50];
	strcpy(nome, "RandomPoset_n25_f0.50_s1.txt");

	Poset *poset = construirPoset(nome, custos, incomparabilidades);

	int *solucao = new int[ poset->getNumeroDeVertices() ];
	int fobj;

	if (getASolution(poset, custos, incomparabilidades, solucao, fobj, -1, true)) {
		while (getASolution(poset, custos, incomparabilidades, solucao, fobj, fobj-1)) {
			cout << fobj << endl;
		}
	}

	system("pause");
}