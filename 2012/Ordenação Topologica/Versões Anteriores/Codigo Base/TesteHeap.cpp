#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;


class Infos{

private:
	int indiceVertice;
	double CustoCRAUltimoElemento;

public:
	Infos(){
	}
	Infos(int i,int c){
		indiceVertice = i;
		CustoCRAUltimoElemento = c;
	}
	double getCustoCRAUltimoElemento(){
		return CustoCRAUltimoElemento;
	}
	int getindiceVertice(){
		return indiceVertice;
	}
	void setCustoCRAUltimoElemento(double c){
		CustoCRAUltimoElemento = c;
	}
	void setIndiceVertice(int i){
		indiceVertice = i;
	}
};

bool compare(Infos A,Infos B){
	if(A.getCustoCRAUltimoElemento() < B.getCustoCRAUltimoElemento())
		return true;
	return false;
}

int main () {
	int n = 8;

	Infos myInfos[8];
	myInfos[0].setIndiceVertice(0);
	myInfos[0].setCustoCRAUltimoElemento(10);

	myInfos[1].setIndiceVertice(1);
	myInfos[1].setCustoCRAUltimoElemento(20);

	myInfos[2].setIndiceVertice(2);
	myInfos[2].setCustoCRAUltimoElemento(30);

	myInfos[3].setIndiceVertice(3);
	myInfos[3].setCustoCRAUltimoElemento(5);

	myInfos[4].setIndiceVertice(4);
	myInfos[4].setCustoCRAUltimoElemento(15);

	myInfos[5].setIndiceVertice(5);
	myInfos[5].setCustoCRAUltimoElemento(7);

	myInfos[6].setIndiceVertice(6);
	myInfos[6].setCustoCRAUltimoElemento(12);

	myInfos[7].setIndiceVertice(8);
	myInfos[7].setCustoCRAUltimoElemento(17);

	vector<Infos> HeapCompleta(myInfos,myInfos+n);
	
	int Melhores40Per = HeapCompleta.size()*0.4;
	Infos *Melhores = new Infos[Melhores40Per];
	vector<Infos> Escolha(Melhores,Melhores);

	make_heap(HeapCompleta.begin(),HeapCompleta.end(),compare);
	make_heap(Escolha.begin(),Escolha.end(),compare);

	int i = 0;
	while(i < Melhores40Per){
		cout << "initial max heap   : " << HeapCompleta.front().getCustoCRAUltimoElemento() << endl;
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

	cout << "final sorted range Escolha :";
	for (unsigned i=0; i<Escolha.size(); i++) cout << " " << Escolha[i].getCustoCRAUltimoElemento();
	
	cout << endl;
  


	system("pause");
	return 0;  
}