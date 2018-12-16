#include "Vertice.h"
#include <mutex>
#include <thread>

class Repositorio{

private:

	int FuncObj;
	list<Vertice*> *ExtensaoLinear;
	mutable std::mutex mutex;


public:
	
	Repositorio(){
		//ExtensaoLinear = new list<Vertice*>;
		FuncObj = INT_MAX;
	}

	void setFuncObj(int valor){
		std::lock_guard<std::mutex> lockValor(mutex);
		std::cout << "Thread Lockada\n";
		if(valor < FuncObj)
			FuncObj = valor;
		std::cout << "Thread Liberada\n";
	}

	int getFuncObj(){
		return FuncObj;
	}

	void setExtensaoLinear(list<Vertice*> *Extensao){

		std::lock_guard<std::mutex> guard(mutex);

		ExtensaoLinear->clear();
		
		auto it = Extensao->begin();
		for (; it != Extensao->end(); it++)
			ExtensaoLinear->push_back((*it));
	}

	list<Vertice*> getExtensaoLinear(){
		return *ExtensaoLinear;
	}
	
};
