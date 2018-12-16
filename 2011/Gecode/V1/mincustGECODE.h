#include <gecode\int.hh>
#include <gecode\search.hh>
#include <gecode\minimodel.hh>

namespace mincustGecode{

using namespace Gecode;

class min:public Space{
public:
	IntVarArray V;
	IntVarArgs varAux;
	IntVarArray vetaux;
	IntArgs custos;
	int tam;
public: 
	min(int tam){
		V = IntVarArray(*this,tam,0,tam-1);
		distinct(*this,V);
		this->tam = tam;
	}
public: void aresta(int origem, int destino){
			rel(*this,V[origem],IRT_LE,V[destino]);
		}
		void custo(int v1,int v2,int custo){

			IntVar e = IntVar(*this,0,1);
			rel(*this,V[v2]-V[v1] == 1 && e == 1 || V[v2]-V[v1] != 1 && e == 0);

			varAux << e;
			custos << custo;
		}
public: 
		void Branch(){
			vetaux = IntVarArray(*this,varAux);
			branch(*this,V,INT_VAR_NONE, INT_VAL_RND);
			branch(*this,vetaux,INT_VAR_SIZE_MIN, INT_VAL_MIN);
		}
		virtual void constrain(const Space & _b){
			const min & b = static_cast<const min&>(_b);

			int custoTotal = 0;
			int num;
			for(int i = 0; i < b.custos.size(); i++){
				num = b.vetaux[i].val();
				custoTotal += b.custos[i] * num;
			}

			linear(*this,custos,vetaux,IRT_LE, custoTotal);
		}
		int GetCustoTotal(){
			int custoTotal = 0;
			for(int i = 0; i < custos.size(); i++){
				custoTotal += custos[i] * vetaux[i].val();;
			}
			return custoTotal;
		}
protected:
	min(bool share, min &s) : Space(share,s){
		V.update(*this,share,s.V);
		vetaux.update(*this,share,s.vetaux);
		custos = s.custos;
		tam = s.tam;
	}
	virtual Space*copy(bool share){
		return new min(share,*this);
	}
};

}