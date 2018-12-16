#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
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

		for (int k=0; k<n; k++)
			if ((k != x) && (A[x][k] == 1) && (!visitado[k])) {
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


bool temCircuito(int **A, int n) {
	bool tem = false;

	enum Cor { BRANCO=0, CINZA=1, PRETO=2 };

	int *cor = new int[n];
	for (int k=0; k<n; k++)
		cor[k] = BRANCO;

	for (int i=0; i<n; i++) {
		bool grauEntradaZero = true;
		for (int j=0; ((j<n) && (grauEntradaZero)); j++)
			if (A[j][i] == 1)
				grauEntradaZero = false;

		if (grauEntradaZero)
		{
			stack<int> P;
			P.push(i);
			while (P.size() > 0) {
				int v = P.top(); P.pop();

				if (cor[v] == BRANCO) {
					cor[v] = CINZA;
					P.push(v);

					for (int w=0; w<n; w++)
						if (A[v][w] == 1) {
							if (cor[w] == BRANCO)
								P.push(w);
							else if (cor[w] == CINZA) {
								tem = true;
								break;
							}
						}
				}
				else if (cor[v] == CINZA) {
					cor[v] = PRETO;
				}

				if (tem) break;
			}//while

			if (tem) break;
		}
	}

	delete[] cor;
	return tem;
}

bool temCircuito(int **A, int n, int vertice) {
	bool tem = false;

	enum Cor { BRANCO=0, CINZA=1, PRETO=2 };

	int *cor = new int[n];
	for (int k=0; k<n; k++)
		cor[k] = BRANCO;

	stack<int> P;
	P.push(vertice);

	while (P.size() > 0) {
		int v = P.top(); P.pop();

		if (cor[v] == BRANCO) {
			cor[v] = CINZA;
			P.push(v);

			for (int w=0; w<n; w++)
				if (A[v][w] == 1) {
					if (cor[w] == BRANCO)
						P.push(w);
					else if (cor[w] == CINZA) {
						tem = true;
						break;
					}
				}
		}
		else if (cor[v] == CINZA) {
			cor[v] = PRETO;
		}

		if (tem) break;
	}//while

	delete[] cor;
	return tem;
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
	double fator;

	n = atoi(args[1]); // numero de vertices
	fator = atof(args[2]); // fator multiplicativo de n p/ determinar numero maximo de arcos
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
	//srand( time(0) );

	// Gerar poset (representacao possivelmente redundante)
	int numArcos = 0;
	int MaxArcos = (int)(fator * (double)n);

	for (int i=0; i<2*n*n; i++) {
		int v = rand() % n;
		int w = rand() % n;

		if (R[v][w] == 1) {
			R[v][w] = 0;
			-- numArcos;
		}
		else if (numArcos < MaxArcos) {
			R[v][w] = 1;
			if (temCircuito(R, n, v))
				R[v][w] = 0;
			else
				++ numArcos;
		}
	}

	// Eliminar relacoes transitivas
	for (int i=0; i<n; i++) {
		for (int j=0; j<n; j++) {
			if (R[i][j] == 1) {
				R[i][j] = 0;
				if (!alcanca(R, n, i, j))
					R[i][j] = 1;
			}
		}
	}

	int **D = gerarMatrizDownset(R,n);

	// Calcular numero de relacoes de cobertura
	int nrc = 0, nnr = 0;
	for (int i=0; i<n; i++) {
		for (int j=(i+1); j<n; j++) {
			if (R[i][j] == 1)
				++ nrc;
			if (D[i][j] == 0)
				nnr += 2;
		}
	}

	char *nomeArquivo = new char[50];
	sprintf(nomeArquivo, "RandomPoset_n%d_f%4.2f_s%d.txt", n, fator, s);

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
	arq.close();

	//-------------------------------------------------
	char *nomeArquivo2 = new char[50];
	sprintf(nomeArquivo2, "Reachable_n%d_f%4.2f_s%d.txt", n, fator, s);

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