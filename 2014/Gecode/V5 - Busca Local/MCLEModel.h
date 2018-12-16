#include <gecode\int.hh>
#include <gecode\search.hh>
#include <gecode\minimodel.hh>


using namespace Gecode;
using namespace std;


class MCLE: public /*Minimize*/Space {
protected :

	int          _n;
	int          _incomparabilities;
	int        **_costs;
	int         *_baseSolution;

	IntVarArray   V;
	BoolVarArray  E;
	BoolVarArray  D; // No need to branch on these since they're completely defined by V
	IntVar        ObjFunction;

public :

// p: a linear extension to which local search will be applied
MCLE(Poset *P, int **costs, int incomparabilities, int *solution,int Tvizinhaca, int upperBound=-1) {

	_n = P->getNumeroDeVertices();
	_incomparabilities = incomparabilities;
	_costs = costs;
	_baseSolution = solution;

	// Some decision vars
	V = IntVarArray(*this, _n, 1, _n);
	E = BoolVarArray(*this, _incomparabilities, 0, 1);
	if (_baseSolution != NULL)
		D = BoolVarArray(*this, _n, 0, 1);

	// Distinct constraint
	distinct(*this, V);

	// Arc constraints
	for (int i = 0; i < P->getNumeroDeVertices(); i++) {
		Vertice *v = P->getVertice(i);
		for (auto arc = v->getListaDeSaida()->begin(); arc != v->getListaDeSaida()->end(); arc++)
			orderConstraint((*arc)->getInicio()->getIndice(), (*arc)->getFinal()->getIndice());
	}

	// Non-arc constraints
	int maxCost = 0; // maximum possible cost
	IntArgs Costs;
	BoolVarArgs EVars;
	for (int i=0; i<_incomparabilities; i++) {
		incomparabilityConstraint(i, costs[i][0], costs[i][1], costs[i][2]);
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

	if (_baseSolution != NULL) {
		// ==========================
		// === BEGIN LOCAL SEARCH === 
		for (int i = 0; i < _n; i++) {
			rel(*this, (V[i] != _baseSolution[i]) == D[i]);
		}

		linear(*this, D, IRT_LQ, Tvizinhaca);
		// === END LOCAL SEARCH =====
		// ==========================
	}

	//branch(*this, V, INT_VAR_SIZE_MIN, INT_VAL_MIN);
	IntVarArgs group1, group2;

	for (int i=0; i<_n; i++) group1 << V[i];
	branch(*this, group1, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
	group2 << ObjFunction;
	branch(*this, group2, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
}

MCLE(bool share , MCLE& s) : /*Minimize*/Space (share , s) {
	_n = s._n;
	_incomparabilities = s._incomparabilities;
	_costs = s._costs;
	_baseSolution = s._baseSolution;

	V.update(*this, share, s.V);
	E.update(*this, share, s.E);
	if (_baseSolution != NULL)
		D.update(*this, share, s.D);
	ObjFunction.update(*this, share, s.ObjFunction);
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

void orderConstraint(int origem, int destino) {
	rel(*this, V[origem], IRT_LE, V[destino]);
}

void incomparabilityConstraint(int i, int v1, int v2, int custo) {
	rel(*this, (V[v2] == V[v1] + 1) == E[i]);
}

void print() {
	for (int i=0; i<_n; i++)
		cout << " " << V[i].val();
	cout << "  -- Obj: " << ObjFunction.val() << endl;
}

int getOjbFunctionValue() {
	return ObjFunction.val();
}

void getSolution(int *&sol) {
	for (int i=0; i<_n; i++)
		sol[i] = V[i].val();
}


};