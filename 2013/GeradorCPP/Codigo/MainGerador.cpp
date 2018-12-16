#include <iostream>
#include <fstream>
#include <queue>
#include <time.h>

using namespace std;

bool alcanca(int **A, int n, int i, int j) {
	queue<int> fila;
	fila.push(i);

	bool *visitado = new bool[n];
	for (int k=0; k<n; k++)
		visitado[k] = false;

	visitado[i] = true;

	int x;
	while (fila.size() > 0) {
		x = fila.front();
		fila.pop();

		for (int k=x+1; k<n; k++)
			if ((A[x][k]) && (!visitado[k])) {
				visitado[k] = true;
				if (k == j) {
					while (fila.size()) fila.pop();
					delete[] visitado;
					return true;
				}
				else
					fila.push(k);
			}
	}
	while (fila.size()) fila.pop();
	delete[] visitado;
	return false;
}

int ** gerarMatrizDownset(int **A, int n) {
	int **d = new int*[n];
	for (int i=0; i<n; i++) {
		d[i] = new int[n];
		for (int j=0; j<n; j++)
			d[i][j] = A[i][j];
	}

	for (int i=n-2; i>=0; i--) {
		for (int j=(i+1); j<n; j++)
			if (d[i][j] == 1) {
				d[i][j] = 1;
				for (int k=(j+1); k<n; k++)
					if (d[j][k] == 1)
						d[i][k] = 1;
			}
	}

	return d;
}

int main(int argc, char **args) {

	int n, s;
	double prob, v;

	n = atoi(args[1]); // numero de vertices
	prob = atof(args[2]); // probabilidade de criacao de cada arco
	s = atoi(args[3]); // semente

	clock_t inicio = clock();

	// Alocar memoria e inicializar
	int **R;
	R = new int*[n];
	for (int i=0; i<n; i++) {
		R[i] = new int[n];
		for (int j=0; j<n; j++)
			R[i][j] = 0;
	}

	srand( s );

	// Gerar poset (representacao possivelmente redundante)
	for (int i=0; i<n; i++) {
		for (int j=i+1; j<n; j++) {
			v = rand() / (double) RAND_MAX;
			if (v <= prob) {
				R[i][j] = 1;
			}
		}
	}

	// Eliminar relacoes transitivas
	/*for (int i=n-2; i>=0; i--) {
		for (int j=i+1; j<n; j++) {
			if (R[i][j] == 1) {
				R[i][j] = 0;
				if (!alcanca(R, n, i, j))
					R[i][j] = 1;
			}
		}
	}*/

	//int **D = gerarMatrizDownset(R,n);

	//// Calcular numero de relacoes de cobertura
	//int nrc = 0, nnr = 0;
	//for (int i=0; i<n; i++) {
	//	for (int j=(i+1); j<n; j++) {
	//		if (R[i][j] == 1)
	//			++ nrc;
	//		if (D[i][j] == 0)
	//			nnr += 2;
	//	}
	//}

	/*for (int i=0; i<n; i++) {
		for (int j=0; j<n; j++)
			cout << " " << R[i][j];
		cout << endl;
	}
	cout << endl;
	for (int i=0; i<n; i++) {
		for (int j=0; j<n; j++)
			cout << " " << D[i][j];
		cout << endl;
	}*/


	/*char *nomeArquivo = new char[50];
	sprintf(nomeArquivo, "RandomPoset_n%d_p%4.2f_s%d.txt", n, prob, s);

	ofstream arq(nomeArquivo);
	arq << n << endl;
	arq << nrc << endl;
	arq << nnr << endl;

	for (int i=0; i<n; i++) {
		for (int j=(i+1); j<n; j++)
			if (R[i][j] == 1)
				arq << i << " " << j << endl;
	}
	for (int i=0; i<n; i++) {
		for (int j=(i+1); j<n; j++)
			if (D[i][j] == 0) {
				arq << i << " " << j << " " << rand() % (n*n) << endl;
				arq << j << " " << i << " " << rand() % (n*n) << endl;
			}
	}
	arq << endl;
	arq.close();*/

	//-------------------------------------------------
	char *nomeArquivo2 = new char[50];
	sprintf(nomeArquivo2, "Reachable_n%d_p%4.2f_s%d.txt", n, prob, s);

	ofstream arq2(nomeArquivo2);
	arq2 << "A:[" << endl;

	for (int i=0; i<n; i++) {
		for (int j=0; j<n; j++)
			if (alcanca(R, n, i, j))
				arq2 << " 1";
			else
				arq2 << " 0";
		arq2 << endl;
	}

	arq2 << "]" << endl;
	arq2.close();
	//-------------------------------------------------



	clock_t fim = clock();

	double tempo = (fim-inicio) / (double) CLOCKS_PER_SEC;

	cout << "tempo = "  << tempo << endl;
	//getchar();
	return 0;
}