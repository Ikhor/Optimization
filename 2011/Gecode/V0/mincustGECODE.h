#include <gecode\int.hh>
#include <gecode\search.hh>
#include <gecode\minimodel.hh>

namespace mincustGecode {

using namespace Gecode;

class min : public Space {

public:
	IntVarArray var1;

	IntVarArgs args;
	IntVarArray vet;

	IntVarArgs varAux;
	IntVarArray vetaux;
	IntArgs custos;
	int tam;

public:
	min(int tam) {
		var1 = IntVarArray(*this,tam,0,tam-1);
		distinct(*this,var1);
		this->tam = tam;
	}

protected:
	min(bool share, min &s) : Space(share,s) {
		var1.update(*this,share,s.var1);
		vetaux.update(*this,share,s.vetaux);
		vet.update(*this,share,s.vet);
		custos = s.custos;
		tam = s.tam;
	}

public:
	void aresta(int origem, int destino) {
		rel(*this,var1[origem],IRT_LE,var1[destino]);
	}

	void custo(int v1,int v2,int custo) {
		IntVar E = IntVar(*this,-tam,tam);

		args << E;

		rel(*this,var1[v2]-var1[v1]-E == 0);

		IntVar e = IntVar(*this,0,1);

		IntVarArgs x;
		x << E;

		count(*this, x, 1, IRT_EQ, e);

		varAux << e;
		custos << custo;
	}

	void Branch() {

		vetaux = IntVarArray(*this,varAux);
		vet = IntVarArray(*this,args);

		branch(*this,var1,INT_VAR_NONE, INT_VAL_RND);
		branch(*this,vetaux,INT_VAR_SIZE_MIN, INT_VAL_MIN);
	}

	virtual void constrain(const Space & _b) {
		const min & b = static_cast<const min&>(_b);

		int custoTotal = 0;
		int num;
		for(int i = 0; i < b.custos.size(); i++) {
			num = b.vetaux[i].val();
			custoTotal += b.custos[i] * num;
		}

		linear(*this,custos,vetaux,IRT_LE, custoTotal);
	}

	int GetCustoTotal() {
		int custoTotal = 0;
		for(int i = 0; i < custos.size(); i++) {
			custoTotal += custos[i] * vetaux[i].val();;
		}
		return custoTotal;
	}

	virtual Space * copy(bool share) {
		return new min(share,*this);
	}

};

}