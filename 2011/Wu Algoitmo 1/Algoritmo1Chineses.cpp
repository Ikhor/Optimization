#include <iostream>
using namespace std;

struct celula{
	bool infinito;
	double custo;
};
struct Dados {
	celula A;
	celula B;
};

/*
As funções abaixo atribue os custos diretamente a matriz dinamica não
necessitando retorna o minino
*/
void custoA(Dados **Matriz,int i, int j, celula ** MatrizAB){

		if((MatrizAB[j][i].infinito || Matriz[i-1][j].B.infinito) && Matriz[i-1][j].A.infinito)
			Matriz[i][j].A.infinito = true;
		else if(Matriz[i-1][j].A.infinito)
			Matriz[i][j].A.custo = Matriz[i-1][j].B.custo + MatrizAB[j][i].custo;
		else if((MatrizAB[j][i].infinito || Matriz[i-1][j].B.infinito))
			Matriz[i][j].A.custo = Matriz[i-1][j].A.custo;
		else{
			if(Matriz[i-1][j].A.custo < Matriz[i-1][j].B.custo + MatrizAB[j][i].custo)
				Matriz[i][j].A.custo = Matriz[i-1][j].A.custo;
			else
				Matriz[i][j].A.custo = Matriz[i-1][j].B.custo + MatrizAB[j][i].custo;
		}
	
}

void custoB(Dados **Matriz,int i, int j, celula ** MatrizBA){
	
		if( (Matriz[i][j-1].A.infinito || MatrizBA[i][j].infinito) && Matriz[i][j-1].B.infinito)
			Matriz[i][j].B.infinito = true;
		else if(Matriz[i][j-1].B.infinito)
			Matriz[i][j].B.custo =  MatrizBA[i][j].custo + Matriz[i][j-1].A.custo;
		else if(Matriz[i][j-1].A.infinito || MatrizBA[i][j].infinito)
			Matriz[i][j].B.custo = Matriz[i][j-1].B.custo;
		else{
			if(Matriz[i][j-1].B.custo <  MatrizBA[i][j].custo + Matriz[i][j-1].A.custo)
				Matriz[i][j].B.custo = Matriz[i][j-1].B.custo;
			else
				Matriz[i][j].B.custo =  MatrizBA[i][j].custo + Matriz[i][j-1].A.custo;
		}
}

/*
As duas funções abaixo realizam a mesma coisa ambas com a finalidade de apenas
poder visualisar o exemplo feito entretanto o codigo que permanecerá é o que está
comentado em ambas.
*/
celula** PreencheMatrizCustoBparaA(int l,int k){

	srand( 26 );

	celula **MatrizBA = new celula*[l];
	for(int i = 0; i < l; i++)
	MatrizBA[i] = new celula [k];

		for(int i = 0;i < l;i++)
		for(int j = 0; j < k ; j++){
			MatrizBA[i][j].custo = 0;
			MatrizBA[i][j].infinito = false;
		}

		MatrizBA[1][1].custo = 0;
		MatrizBA[1][2].custo = 0;
		MatrizBA[1][3].custo = 0;
		
		MatrizBA[2][1].custo = 2;
		MatrizBA[2][2].custo = 0;
		MatrizBA[2][3].custo = 0;

		MatrizBA[3][1].custo = 3;
		MatrizBA[3][2].custo = 3;
		MatrizBA[3][3].custo = 3;

		MatrizBA[4][1].custo = 4;
		MatrizBA[4][2].custo = 4;
		MatrizBA[4][3].custo = 4;

		MatrizBA[5][1].infinito = true;
		MatrizBA[5][2].infinito = true;
		MatrizBA[5][3].infinito = true;

		return MatrizBA;
}
celula** PreencheMatrizCustoAparaB(int l, int k){

	srand( 26 );

	celula **MatrizAB = new celula*[k];
	for(int i = 0; i < l; i++)
	MatrizAB[i] = new celula [l];

		for(int i = 0;i < l;i++)
		for(int j = 0; j < k ; j++){
			MatrizAB[i][j].custo = 0;
			MatrizAB[i][j].infinito = false;
		}

		MatrizAB[1][1].infinito = true;
		MatrizAB[1][2].custo = 1;
		MatrizAB[1][3].custo = 1;
		MatrizAB[1][4].custo = 1;
		MatrizAB[1][5].infinito = true;

		MatrizAB[2][1].infinito = true;
		MatrizAB[2][2].infinito = true;
		MatrizAB[2][3].custo = 2;
		MatrizAB[2][4].custo = 2;
		MatrizAB[2][5].infinito = true;

		MatrizAB[3][1].infinito = true;
		MatrizAB[3][2].infinito = true;
		MatrizAB[3][3].custo = 3;
		MatrizAB[3][4].custo = 3;
		MatrizAB[3][5].infinito = true;

		return MatrizAB;
}

/*
Apenas inicializa a matriz e realiza o STEP 1 do algoritmo
*/
void inicializarMatriz(Dados**Matriz,int l,int k){

	for(int i = 0;i < l;i++)
		for(int j = 0; j < k ; j++){
			Matriz[i][j].A.custo = 0;
			Matriz[i][j].A.infinito = false;
			Matriz[i][j].B.custo = 0;
			Matriz[i][j].B.infinito = false;
		}
		
		for(int i = 1; i < l; i++)
			Matriz[i][0].B.infinito = true;

		for(int j = 1; j < k ;j++)
			Matriz[0][j].A.infinito = true;
}

void AlgoritmoChineses(int l, int k){

	Dados **matriz = new Dados*[l];
	for(int i = 0; i < l; i++)
	matriz[i] = new Dados [k];

	inicializarMatriz(matriz,l,k);

	celula ** MatrizAB = PreencheMatrizCustoAparaB(l,k);
	celula ** MatrizBA = PreencheMatrizCustoBparaA(l,k);

	//Preenche A Matriz Dinamica
	for(int i = 1;i < k;i++)
		for(int j =1; j < l ; j++){
			custoA(matriz,j,i,MatrizAB);
			custoB(matriz,j,i,MatrizBA);
		}


	//Resultado Da Matriz Dinamica
	if(matriz[0][1].A.infinito)
		cout << "Terminando em A o custo eh infinito\n";
	else
		cout << "Terminando em A o custo eh " << matriz[1][3].A.custo << endl;

	
	if(matriz[1][3].B.infinito)
		cout << "Terminando em B o custo eh infinito\n";
	else
		cout << "Terminando em B o custo eh " << matriz[1][3].B.custo << endl;

}

int main(){

	cout.setf(ios::boolalpha);

	int l = 6;
	int k = 4;

	AlgoritmoChineses(l,k);
		
	getchar();
	
}
