#include <iostream>


/****************************************************************/
/*																*/
/*							General								*/
/*																*/
/****************************************************************/


//Empty type
class NullType {};

//A container that converts an int to a distinct type, with a value
template<int N> struct intc { enum { value = N }; };


/****************************************************************/
/*																*/
/*							List								*/
/*																*/
/****************************************************************/


//Macro wrapper around List, V is the name of the variable defines
#define makeList(V, ...) typedef List<__VA_ARGS__> V

//Macro wrapper around ListGet
#define get(T, N) ListGet<T,N>::result


//A vector of types
template <class T, class ... U> struct List {
	typedef List<U...> Tail;
	typedef T Head;
	enum { size = Tail::size + 1 };
};
template <class T> struct List<T> {
	typedef NullType Tail;
	typedef T Head;
	enum { size = 1 };
};


//Get the n'th item of the list
template <class T, int N> struct ListGet {
	typedef typename ListGet<typename T::Tail, N-1>::result result;
};
template <class T> struct ListGet<T, 0> {
	typedef typename T::Head result;
};


/****************************************************************/
/*																*/
/*							Dot Product							*/
/*																*/
/****************************************************************/


//Dot product wrapper
#define dotProd(A, B) dotProd<A, B>::result


//Dot product
template <class T1, class T2> struct dotProd {

	//Check for errors
	static_assert(T1::size > 0, "Error, invalid type passed in");
	static_assert(T1::size == T2::size, "Error, cannot take dot product "
											"of different sized vectors");

	//Multiply the heads then recurse with the tails.
	enum { result =  T1::Head::value * T2::Head::value + dotProd<typename T1::Tail, typename T2::Tail>::result };
};

//Called when the end of the lists have been hit
template <> struct dotProd<NullType, NullType> { enum { result = 0 }; };


/****************************************************************/
/*																*/
/*						For vector printing						*/
/*																*/
/****************************************************************/


//Print wrapper
#define pnt(V) {							\
	std::cout << #V << " = [ ";				\
	if (V::size) printVectorHelper<V>();	\
	std::cout << "]" << std::endl;			\
}


//Print a vector
template<class V> void printVectorHelper() {
	std::cout << get(V, 0)::value << (V::size > 1 ? ", ":" ");
	printVectorHelper<typename V::Tail>();
}

//Called when there is nothing left to print
template<> void printVectorHelper<NullType>() {}



/****************************************************************/
/*																*/
/*								Main 							*/
/*																*/
/****************************************************************/


//Main function
int main() {

	//Initial vectors
	makeList(A_1, intc<1>, intc<2>, intc<3>);
	makeList(A_2, intc<4>, intc<5>, intc<6>);
	makeList(A_3, intc<7>, intc<8>, intc<9>);
	makeList(A, A_1, A_2, A_3);
	//TODO: better wrapper
	//makeIntList(B, 6, 5, 4, 3, 2, 1);

	//Print the maticies
	pnt(A_1); 

	//Dot product
	std::cout << dotProd(A_1, A_2) << std::endl;

	//Success
	return 0;
}
