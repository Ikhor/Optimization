/**********************************************************************************
// REGISTRO_ORDENACAO_TOPOLOGICA
// 
// Criação:		22 Abr 2013
// Atualização:	23 Set 2013
// Compilador:	Visual C++ 2010
//
// Descrição:	Define métodos para GRASP armazenando o custo
//				com relação ao ultimo elemento e o vertice armazenado
//				É usado para comparações nos grasp armazenado em uma lista de candidatos
//				
//
**********************************************************************************/
#define _REGISTRO_ORDENACAO_TOPOLOGICA_H_

class Dados{

private:
	int Indice;
	double Custo;

public:
	Dados(){}

	Dados(int i,int c){
		Indice = i;
		Custo = c;
	}
	double getCusto(){
		return Custo;
	}
	int getIndice(){
		return Indice;
	}
	void setCusto(double c){
		Custo = c;
	}
	void setIndiceVertice(int i){
		Indice = i;
	}
	
	inline bool operator < (const Dados &Dados) const{
		return Dados.Custo < Custo;	
	}

};