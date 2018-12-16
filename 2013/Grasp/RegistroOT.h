/**********************************************************************************
// REGISTRO_ORDENACAO_TOPOLOGICA
// 
// Cria��o:		22 Abr 2013
// Atualiza��o:	23 Set 2013
// Compilador:	Visual C++ 2010
//
// Descri��o:	Define m�todos para GRASP armazenando o custo
//				com rela��o ao ultimo elemento e o vertice armazenado
//				� usado para compara��es nos grasp armazenado em uma lista de candidatos
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