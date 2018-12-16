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

int main(){

	int ** custos;
	int incomparabilidades;
	Poset *poset = construirPoset("rp_n10_p0.80_s1.txt" , custos, incomparabilidades);
	MCLE * model = new MCLE(poset, custos, incomparabilidades);

	// Instantiate a model
	BAB<MCLE> bab(model);

	int count = 0, opt = 0;
	for (int i=0; i<incomparabilidades; i++)
		opt += custos[i][2];

	MCLE* s;
	while (s = bab.next ()) {
		++ count;
		//s->print();
		cout << s->getOjbFunctionValue() << "  " << opt << endl;
		if (s->getOjbFunctionValue() < opt)
			opt = s->getOjbFunctionValue();
		//if (count > 1000) break;
	}

	cout << "opt = " << opt << endl;

	delete model; // model has been copied by dfs

	std::cout << "Nb. of solutions found: " << count << std::endl;

	system("pause");
}