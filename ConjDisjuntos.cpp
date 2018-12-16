/*
 * Universidade Federal Rural do Semiarido, 2011.2
 * Estruturas de Dados I (EXA-0156)
 * Tiberius O. Bonates
 */
#include <iostream>
#include <string>

using namespace std;

//==============================================
struct ConjDisjuntos {
	int tamanho; // numero total de elementos na estrutura
	int *p;      // vetor com o "pai" de cada elemento
	             // o representante de um conjunto e' pai de si mesmo
	int *r;      // vetor com o "rank" de cada elemento
};

// Inicializa vetores "p" e "r" e preenche campo "tamanho"
void inicializar(ConjDisjuntos &cd, int tam) {
	cd.tamanho = tam;
	cd.p = new int[tam];
	cd.r = new int[tam];

	for (int i=0; i<cd.tamanho; i++) {
		cd.p[i] = i;
		cd.r[i] = 0;
	}
}

// Retorna o representante do conjunto que contem o elemento
// Aplica compressao de caminho
int encontrar(ConjDisjuntos &cd, int x) {
	if (cd.p[x] == x) return x;
	cd.p[x] = encontrar(cd, cd.p[x]);
	return cd.p[x];
}

int encontrarNaoRecursiva(ConjDisjuntos &cd, int x) {
	cout << "Ajude-me: eu nao faco nada." << endl;
	return 0;
}

// Realiza a uniao dos conjuntos que contem os elementos "x" e "y"
// Aplica uniao por rank
void unir(ConjDisjuntos &cd, int x, int y) {
	int rx = encontrar(cd, x);
	int ry = encontrar(cd, y);

	if (cd.r[rx] > cd.r[ry])
		cd.p[ry] = rx;
	else if (rx != ry) {
		cd.p[rx] = ry;
		if (cd.r[rx] == cd.r[ry])
			cd.r[ry] ++;
	}
}

// Funcao auxiliar: imprime conteudo dos vetores "p" e "r"
void imprimir(const ConjDisjuntos &cd) {
	cout << endl << "    ";
	for (int i=0; i<cd.tamanho; i++)
		cout << " " << i << " ";
	cout << endl;

	cout << "p: [";
	for (int i=0; i<cd.tamanho; i++)
		cout << " " << cd.p[i] << " ";
	cout << "]" << endl;

	cout << "r: [";
	for (int i=0; i<cd.tamanho; i++)
		cout << " " << cd.r[i] << " ";
	cout << "]" << endl << endl;
}

// Funcao auxiliar: imprime estrutura na linguagem DOT
// Abrir utilizando o software GraphViz (http://www.graphviz.org)
void imprimir2(const ConjDisjuntos &cd) {
	cout << "digraph G {" << endl;
	for (int i=0; i<cd.tamanho; i++) {
		cout << "\tno" << i << " [shape=record, label=\"" << i << "\"]" << endl;
	}
	for (int i=0; i<cd.tamanho; i++) {
		cout << "\tno" << i << " -> no" << cd.p[i] << endl;
	}
	cout << "}" << endl;
}

//==============================================

void main() {

	ConjDisjuntos conjDisj;
	inicializar(conjDisj, 10);

	unir(conjDisj, 1,3);
	unir(conjDisj, 3,4);
	unir(conjDisj, 8,9);
	unir(conjDisj, 6,2);
	unir(conjDisj, 2,7);
	imprimir2(conjDisj);

	unir(conjDisj, 8,6);
	imprimir2(conjDisj);

	unir(conjDisj, 2,1);
	imprimir2(conjDisj);

	// Durante este Find, 8 passa a ser filho direto de 2
	cout << "Representante do conj. que contem 8: " << encontrar(conjDisj, 8) << endl;
	imprimir(conjDisj);

	// Durante este Find, 4 passa a ser filho direto de 2
	cout << "Representante do conj. que contem 4: " << encontrar(conjDisj, 4) << endl;
	imprimir(conjDisj);

	// Durante este Find, nenhuma mudanca acontece na estrutura
	cout << "Representante do conj. que contem 5: " << encontrar(conjDisj, 5) << endl;
	imprimir(conjDisj);

	getchar();
    return;
}
