/**********************************************************************************
// REGISTRO_ORDENACAO_TOPOLOGICA
// 
// Cria��o:		22 Abr 2013
// Atualiza��o:	22 Abr 2013
// Compilador:	Visual C++ 2010
//
// Descri��o:	Define m�todos para GRASP armazenando o custo
//				com rela��o ao ultimo elemento e o ventice armazenado
//				� usado para compara��es nos grasp armazenado em uma lista de candidatos
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