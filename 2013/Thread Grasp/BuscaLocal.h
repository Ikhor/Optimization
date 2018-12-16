/**********************************************************************************
// Busca Local
// 
// Cria��o:	22 Abr 2013
// Atualiza��o:	28 Mai 2013
// Compilador:	Visual C++ 2010
//
// Descri��o:	Define m�todos para busca local
//				
//		Existem duas fun��es de custo	
//		
//		Custo(Vertice *A,Vertice *B) 
//		Esta fun��o tem o objetivo de apenas obter o custo com rela��o ao v�rtice
//     
//		Custo(Vertice *X,Vertice *A,Vertice *B,Vertice *Y)
//		Nesta fun��o � verificado qual a sequencia com menor custo
//		XABY ou XBAY
//
//		A busca local consiste em fazer compara��es 4 � 4 da sequencia XABY
//		verificando o menor custo citado em sua fun��o
//		Repetindo esse processo para toda a extens�o enquanto for poss�vel melhorar
//
//
**********************************************************************************/

// ---------------------------------------------------------------------------------
// Inclus�es
#include "Vertice.h"

// ---------------------------------------------------------------------------------

double Custo(Vertice *A,Vertice *B){
	double retorno;
	if(A->obterCustoParaVertice(B->getIndice(),retorno)){
		return retorno;
	}
	else
		return DBL_MAX;
}

double Custo(Vertice *X,Vertice *A,Vertice *B,Vertice *Y){
	double XA,AB,BY;
	if(X->obterCustoParaVertice(A->getIndice(),XA) &&
		A->obterCustoParaVertice(B->getIndice(),AB) &&
		B->obterCustoParaVertice(Y->getIndice(),BY) ){
			return XA+AB+BY;
	}
	else
		return DBL_MAX;
}

void LocalSearch(list<Vertice*> *extlinear){
	bool houveTroca;
	do{
		houveTroca = false;
		vector<Vertice*> LS(extlinear->size());
		int j = 0;
		for(list<Vertice*>::iterator itVertice = extlinear->begin();itVertice != extlinear->end(); itVertice++,j++)
			LS.at(j) = (*itVertice);
		
		if(Custo(LS.at(0),LS.at(1)) > Custo(LS.at(1),LS.at(0))){
			Vertice *Aux = LS.at(0);
			LS.at(0) = LS.at(1);
			LS.at(1) = Aux;
			houveTroca = true;
		}
		unsigned int i = 1;
		for(;i < LS.size()-2;i++)
			if(Custo(LS.at(i-1),LS.at(i),LS.at(i+1),LS.at(i+2)) >
				Custo(LS.at(i-1),LS.at(i+1),LS.at(i),LS.at(i+2))){
				Vertice *Aux = LS.at(i);
				LS.at(i) = LS.at(i+1);
				LS.at(i+1) = Aux;
				houveTroca = true;
			}
			if(Custo(LS.at(i),LS.at(i+1)) > Custo(LS.at(i+1),LS.at(i))){
				Vertice *Aux = LS.at(i);
				LS.at(i) = LS.at(i+1);
				LS.at(i+1) = Aux;
				houveTroca = true;
			}
			extlinear->clear();
			for(unsigned  int i = 0;i < LS.size();i++)
				extlinear->push_back(LS.at(i));
	}while(houveTroca != false);
}
