// Universidade Federal Rural do Semi-Arido
// Felipe Oliveira, Sep 2012

#include <gecode/driver.hh>
#include <gecode/int.hh>

int v[] = {5, 3, 4,  8, 2, 9, 7, 6};
int p[] = {1 ,3, 2,  4, 7 ,5 ,3 ,4};
int b = 7;
int Nmochilas = 5;

using namespace Gecode;
using namespace std;

class Mochila : public Space {
protected :
	int n;        
	IntVarArray X;
public :

Mochila(int * v,int *p, int b) {

	n= 8;
	X = IntVarArray(*this,n*Nmochilas,0,1);

	IntArgs coef;
	for(int i = 0; i < n ; i ++)
		coef << p[i];

	for(int i = 0; i < n ; i++){
		IntVarArgs Aux;
		for(int j = 0; j < Nmochilas ; j++)
			Aux << X[i+j*n];
		rel(*this,sum(Aux) < 2);	
	}

	for(int i = 0; i < Nmochilas; i++){
		IntVarArgs vars;
		for(int j = 0; j < n; j++)
			vars << X[j + n*i];
		linear(*this,coef,vars,IRT_LQ,b);
	}
	
	//Dependencia
	/*
	Pegar relações de dependência do arquivo em cima de v1 e v2
	*/
	int V1 = 0,V2 = 2;
	IntVarArgs Vert1;
	IntVarArgs Vert2;
	for(int j = 0; j < Nmochilas ; j++){
		Vert1 << X[V1+j*n];
		Vert2 << X[V2+j*n];
		rel(*this,sum(Vert2) <= sum(Vert1));
	}
	rel(*this,sum(Vert2) <= sum(Vert1));	

	branch(*this, X, INT_VAR_SIZE_MIN, INT_VAL_MAX);
}

Mochila(bool share , Mochila& s) : Space (share , s) {
	n = s.n;
	X.update(*this, share, s.X);
}

virtual Space * copy ( bool share ) {
	return new Mochila(share , * this);
}


void print () {
	int count = 0;
	int total = 0;
	int pesototal = 0;
	int peso = 0;
	cout << "--------------------------\n";
	for(int j = 0; j < Nmochilas ; j++){
		peso = 0;
		for (int i=0; i<n; i++) {
			cout << X[i+j*n] << " ";
			if(X[i+j*n].val() == 1){
				count++;
				total = total + v[i];
				peso  = peso + p[i];
			}
		}
		pesototal = pesototal + peso;
		cout << " Peso: " << peso << endl;
	}
	cout << "--------------------------\n";
	cout << "Qtd de itens: " << count  << "\nTotal: " << total << "\nPeso Total: " << pesototal << endl;
}

void constrain(const Space &best){

	Mochila *m = (Mochila *) &best;
	IntArgs coef;
	int total = 0;
	for(int j = 0; j < Nmochilas; j++)
		for (int i = 0; i < n; i++)
			coef << v[i];
	for(int j = 0; j < Nmochilas; j++)
		for (int i = 0; i < n; i++){
			if((m->X[i+j*n].assigned()) && m->X[i+j*n].val()==1){
				total = total + v[i];
			}
		}
	linear(*this,coef,X, IRT_GR,total);	
}	

};

int main () {


	Mochila* m = new Mochila(v,p,b);
	BAB<Mochila> dfs (m);

	int count = 0;
	Mochila* s;
	while ((s = dfs . next ())) {
		s->print();
		count ++;
	}

	delete m; // model has been copied by dfs

	std::cout << "Nb. of solutions found: " << count << std::endl;
	getchar();
	return 0;
}

