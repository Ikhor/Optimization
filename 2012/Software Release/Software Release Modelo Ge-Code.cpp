#include <iostream>
#include <fstream>
#include <queue>
#include <time.h>

using namespace std;

bool alcanca(int **A, int n, int i, int j) {
	queue<int> fila;
	fila.push(i);

	bool *visitado = new bool[n];
	for (int i=0; i<n; i++)
		visitado[i] = false;

	int x;
	while (fila.size() > 0) {
		x = fila.front();
		fila.pop();

		for (int k=x+1; k<n; k++)
			if ((A[x][k]) && (!visitado[k])) {
				if (k == j) {
					delete visitado;
					return true;
				}
				else
					fila.push(k);
			}
	}
	delete visitado;
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

	int n, P, M, semente;
	double pr, v;

//	n = atoi(args[1]); // numero de requerimentos
//	pr = atof(args[2]); // probabilidade de criacao de cada arco
//	P = atoi(args[3]); // Numero de releases
//  M  numero de clientes
	cin >> n;
	cin >> pr;
	cin >> P;
	cin >> M;
	cin >> semente;

	clock_t inicio = clock();

	// Alocar memoria e inicializar
	int **R;
	R = new int*[n];
	for (int i=0; i<n; i++) {
		R[i] = new int[n];
		for (int j=0; j<n; j++)
			R[i][j] = 0;
	}

	srand( semente );

	// Gerar poset (representacao possivelmente redundante)
	for (int i=0; i<n; i++) {
		for (int j=i+1; j<n; j++) {
			v = rand() / (double) RAND_MAX;
			if (v > pr) {
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

	char *nomeArquivo = new char[20];
	sprintf(nomeArquivo, "rp_n%d_p%4.2f_s%d.txt", n, pr, P);

	ofstream arq(nomeArquivo);
	arq << P << endl; //Numero de releases
	for (int i=0; i<P; i++) {
		arq << rand()%(n*n*P/2) << endl; // Budget de cada release
	}
	arq << n << endl;//Numero requerimentos
	for(int j = 0 ; j < n; j++){
		    arq << rand()%(50) << " ";//Risco
			arq << rand()%(n*n) << endl;//Custo
	}

	arq << M << endl;
	// Importancia do cliente para a organizacao
	for (int i=0; i<M; i++)
		arq << rand() % 10 << " ";
	arq << endl;

	for (int i=0; i<M; i++) {
		for(int j = 0 ; j < n; j++)
		    arq << rand() % 10 << " ";//Importancia do requerimento para o cliente
		arq << endl;
	}

	arq << nrc << endl;//Numero de arcos diretos(relações)
	for (int i=0; i<n; i++) {
		for (int j=(i+1); j<n; j++)
			if (R[i][j] == 1)
				arq << i << " " << j << endl;
	}
	arq.close();

	clock_t fim = clock();

	double tempo = (fim-inicio) / (double) CLOCKS_PER_SEC;

	cout << "tempo = "  << tempo << endl;
	//getchar();
	return 0;
}