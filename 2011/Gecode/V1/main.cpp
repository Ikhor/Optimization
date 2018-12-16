#include "mincustGECODE.h"
#include <fstream>
using namespace std;

int main(){
	
	ifstream fin;
	fin.open("TEXTO.txt");
	int NumeroDeVertices;
	fin >> NumeroDeVertices;
	
	mincustGecode::min* model = new mincustGecode::min(NumeroDeVertices);

	
	int NumeroDeArcosDiretos;
	fin >> NumeroDeArcosDiretos;
	int NumeroDeArcosComCustos;
	fin >> NumeroDeArcosComCustos;
	int aux1,aux2,aux3;
	for(int i = 0;i < NumeroDeArcosDiretos; i++){
		fin >> aux1;
		fin >> aux2;
		model->aresta(aux1,aux2);
	}
	for(int i = 0;i < NumeroDeArcosComCustos; i++){
		fin >> aux1;
		fin >> aux2;
		fin >> aux3;
		model->custo(aux1,aux2,aux3);
	}

	fin.close();

	model->Branch();

	Gecode::BAB<mincustGecode::min>* eBAB;

	eBAB = new Gecode::BAB<mincustGecode::min>(model);

	mincustGecode::min* s;
	int*  vet = new int[NumeroDeVertices];
	while( s = eBAB->next()){
		for(int i = 0; i < NumeroDeVertices; i++){			
			vet[s->var1[i].val()] = i;
		}
		for(int i = 0; i < NumeroDeVertices; i++){			
			cout << vet[i] << " " ;
		}
		cout << "Custo: " << s->GetCustoTotal() << endl;
		delete s;
	}

	system("pause");
}