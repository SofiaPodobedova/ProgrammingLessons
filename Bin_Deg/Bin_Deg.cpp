// Bin_Fun.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include <iostream>
#include <assert.h>
#include "Matr.cpp"

using namespace std;

#define REQUIRE(cond, message) \
	if (!(cond)) { cerr << message << endl; assert(false); }


template< class T>
class BinFunc {
public:
	T unity(T uno) const {
		return uno;
	}
	T operator()(const T& a, const T& b) const {
		return a * b;
	}
};

template< class T,  template< class > class BOp >
	T pow(T a, size_t N, const BOp<T>& f, T m1) {
		if (N == 0) return f.unity(m1);
		if (N%2 == 0) return f( pow(a, N/2, f, m1), pow(a, N/2, f, m1) );
		return f( pow(a, N-1, f, m1), a );
	}

template <class T, template<class> class BOp>
T powLinear(T a, size_t N, const BOp<T>& f, T m1)
{
	T result = f.unity(m1);
	for (size_t i = 0; i < N; ++i) {
		result = f(result, a);
	}
	return result;
}

int main()
{
	BinFunc<int> f;
	int m1 = 1;
	for (int a = 0; a < 10; ++a) {
		for (int n = 0; n < 10; ++n) {
			REQUIRE(pow(a, n, f, m1) == powLinear(a, n, f, m1), "Results differ, " << powLinear(a, n, f, m1) << " expected, but " << pow(a, n, f, m1) << " provided.");
		}
	}
	

	int a, N;
	cin >> a >> N;
	BinFunc<int> funcpr;
	int k = pow(a, N, funcpr, m1);
	cout << k;
	
	int curr, t;
	cin >> t;
	if(t <= 0) return 0;
	int m_s = t;
	VMatrix<int> uno = unity_matr<int>(m_s);
	VMatrix<int> matr(m_s,m_s);
	for (int i = 0; i < m_s; i++)
		for (int j = 0; j < m_s; j++)
		{
			cin >> curr;
			matr.set(curr,i,j);
		}
	int deg;
	BinFunc<VMatrix<int> > f_matr;
	cin >> deg;
	VMatrix<int> val = pow(matr, deg, f_matr, uno);
	for (int i = 0; i < m_s; i++)
		for (int j = 0; j < m_s; j++)
		{
			cout << val(i,j) << "  ";
		}
	getch();
	return 0;
}