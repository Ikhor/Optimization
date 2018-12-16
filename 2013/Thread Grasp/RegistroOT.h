/**********************************************************************************
// REGISTRO_ORDENACAO_TOPOLOGICA
// 
// Criação:		22 Abr 2013
// Atualização:	22 Abr 2013
// Compilador:	Visual C++ 2010
//
// Descrição:	Define métodos para GRASP armazenando o custo
//				com relação ao ultimo elemento e o ventice armazenado
//				É usado para comparações nos grasp armazenado em uma lista de candidatos
//				
//
**********************************************************************************/
#define _REGISTRO_ORDENACAO_TOPOLOGICA_H_

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