/**********************************************************************************
// Arcos (Arquivo de Cabeçalho)
// 
// Criação:		22 Abr 2013
// Atualização:	22 Abr 2013
// Compilador:	Visual C++ 2010
//
// Descrição:	Define uma classe para gerenciar a dependências entre vertices.
//				
//				Um arco tem a responsabilidade de identificar dependencia entre 
//				recursos para composição do grafo.
//              
//				Possui apenas get/set e o construtor
//
**********************************************************************************/
#ifndef _ARCO_H_
#define _ARCO_H_

// ---------------------------------------------------------------------------------
// Inclusões
#include "Vertice.h"

// ---------------------------------------------------------------------------------

class Arco {

private:

	class Vertice *U;
    class Vertice *V;

public:

	Arco(Vertice *u, Vertice *v) {
		U = u;
		V = v;
    }

    Arco(Arco *a) {
		U = a->getInicio();
		V = a->getFinal();
    }

	Vertice * getInicio() {
		return U;
	}

	Vertice * getFinal() {
		return V;
	}
};

#endif