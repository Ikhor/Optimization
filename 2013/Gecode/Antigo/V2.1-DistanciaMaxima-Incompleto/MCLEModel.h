#include <gecode\int.hh>
#include <gecode\search.hh>
#include <gecode\minimodel.hh>


using namespace Gecode;
using namespace std;


class MCLE: public /*Minimize*/Space {
protected :

	int          _n;
	int          _m;
	int          _incomparabilities;
	int        **_costs;

	IntVarArray  V;
	IntVarArray  E;
	IntVarArray  *diff, *a, *d;
	IntVar       ObjFunction;

public :

MCLE(Poset *P, int **costs, int incomparabilities, list<int*> sols, int upperBound=-1) {

	_n = P->getNumeroDeVertices();
	_m = sols.size();
	_incomparabilities = incomparabilities;
	_costs = costs;

	// Some decision vars
	V = IntVarArray(*this, _n, 1, _n);
	E = IntVarArray(*this, _incomparabilities, 0, 1);

	// Distinct constraint
	distinct(*this, V);

	// Arc constraints
	for (int i = 0; i < P->getNumeroDeVertices(); i++) {
		Vertice *v = P->getVertice(i);
		for (auto arc = v->getListaDeSaida()->begin(); arc != v->getListaDeSaida()->end(); arc++)
			arco((*arc)->getInicio()->getIndice(), (*arc)->getFinal()->getIndice());
	}

	// Non-arc constraints
	int maxCost = 0; // maximum possible cost
	IntArgs Costs;
	IntVarArgs EVars;
	for (int i=0; i<_incomparabilities; i++) {
		incomparability(i, costs[i][0], costs[i][1], costs[i][2]);
		maxCost += costs[i][2];
		Costs << costs[i][2];
		EVars << E[i];
	}

	// Create decision var associated with objective function
	if (upperBound >= 0)
		ObjFunction = IntVar(*this, 0, upperBound); // Custom upper bound
	else
		ObjFunction = IntVar(*this, 0, maxCost); // Standard upper bound

	// Constraint related to the objective function
	linear(*this, Costs, EVars, IRT_EQ, ObjFunction);

	// Downset and upset bounds
	int Dsize, Usize;
	for (int i = 0; i < P->getNumeroDeVertices(); i++) {
		Dsize = P->tamanhoDownset( i );
		Usize = P->tamanhoUpset( i );
		insertBounds(i, Dsize, Usize);
	}

	//=============
	// Compare with given solutions
	// Create decision var associated with objective function
	//ObjFunction = IntVar(*this, 0, _n);
	// Create decision var array corresponding to differences
	diff = new IntVarArray[ _m ];
	a = new IntVarArray[ _m ];
	d = new IntVarArray[ _m ];
	for (int j=0; j<_m; j++) {
		diff[j] = IntVarArray(*this, _n, 0, _n-1);
		a[j] = IntVarArray(*this, _n, -_n + 1, _n - 1);
		d[j] = IntVarArray(*this, _n, 0, 1);
	}

	if (sols.size() > 0) {
		list<int*>::iterator itSol = sols.begin();
		int j = 0;
		while (itSol != sols.end()) {
			for (int i=0; i<_n; i++) {
				// Make sure d[i]=0 if there is no difference
				rel(*this, diff[j][i] == (*itSol)[i] - V[i]);
				abs(*this, diff[j][i], a[j][i]);
				rel(*this, d[j][i] <= a[j][i]);
			}

			// Constraint related to the objective function
			linear(*this, d[j], IRT_GQ, ObjFunction);

			++ itSol;
			++ j;
		}
	}
	else {
		// There is no constraint regarding existing solutions.
		// Therefore, we revert to the standard model.
		for (int j=0; j<_m; j++)
			for (int i=0; i<_n; i++) {
				rel(*this, diff[j][i] == _n-1);
				rel(*this, a[j][i] == _n-1);
				rel(*this, d[j][i] == 1);
			}
	}
	//=============

	branch(*this, V, INT_VAR_SIZE_MIN, INT_VAL_MIN);
	branch(*this, E, INT_VAR_SIZE_MIN, INT_VAL_MIN);
	branch(*this, ObjFunction, INT_VAL_MIN);
	for (int j=0; j<_m; j++) {
		branch(*this, diff[j], INT_VAR_RND, INT_VAL_RND);
		branch(*this, a[j], INT_VAR_RND, INT_VAL_RND);
		branch(*this, d[j], INT_VAR_RND, INT_VAL_RND);
	}
}

MCLE(bool share , MCLE& s) : /*Minimize*/Space (share , s) {
	_n = s._n;
	_m = s._m;
	_incomparabilities = s._incomparabilities;
	_costs = s._costs;

	V.update(*this, share, s.V);
	E.update(*this, share, s.E);
	ObjFunction.update(*this, share, s.ObjFunction);

	for (int j=0; j<_m; j++) {
		diff[j].update(*this, share, s.diff[j]);
		a[j].update(*this, share, s.a[j]);
		d[j].update(*this, share, s.d[j]);
	}
}

virtual Space * copy ( bool share ) {
	return new MCLE(share , * this);
}

virtual void constrain(const Space & _b) {
	const MCLE & b = static_cast<const MCLE&>(_b);

	int custoTotal = 0;
	for(int i = 0; i < _incomparabilities; i++)
		custoTotal += b._costs[i][2] * b.E[i].val();

	rel(*this, ObjFunction, IRT_LE, custoTotal);
}

void arco(int origem, int destino) {
	rel(*this, V[origem], IRT_LE, V[destino]);
}

void incomparability(int i, int v1, int v2, int custo) {
	rel(*this, (V[v2]-V[v1] == 1 && E[i] == 1) || (V[v2]-V[v1] != 1 && E[i] == 0));
}

void insertBounds(int vertice, int tamanhoDownset, int tamanhoUpset) {
	rel(*this, V[vertice], IRT_GQ, tamanhoUpset);
	rel(*this, V[vertice], IRT_LQ, _n - tamanhoDownset + 1);
}

void print() {
	for (int i=0; i<_n; i++)
		cout << " " << V[i].val();
	cout << "  -- Obj: " << ObjFunction.val() << endl;
}

int getOjbFunctionValue() {
	return ObjFunction.val();
}

};