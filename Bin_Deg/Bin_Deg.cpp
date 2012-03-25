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
class NumberBinFunc {
public:
	T unity() const {
		return 1;
	}
	T operator()(const T& a, const T& b) const {
		return a * b;
	}
};

template< class T>
class MatrixBinFunc {
public:
	MatrixBinFunc(int n) {m_s=n;};
	/*MatrixBinFunc(const VMatrix<T>& matr) {
		m_s = matr.getSize();
	}*/
	VMatrix<T> unity() const {
		return unity_matr<T>(m_s);
	}
	VMatrix<T> operator()(const VMatrix<T>& a, const VMatrix<T>& b) const {
		return a * b;
	}
	int setSize(int size) { m_s=size; return m_s; }
private:
	int m_s;
};

/*template< class T,  template< class > class BOp >
	T pow(const T& a, size_t N, const BOp<T>& f) {
		if (N == 0) return f.unity();
		if (N%2 == 0) return f( pow(a, N/2, f), pow(a, N/2, f) );
		return f( pow(a, N-1, f), a );
	}
*/

template< class Op,  template< class > class BOp >
	Op pow(const Op& a, size_t N, const BOp<int>& f) {
		if (N == 0) return f.unity();
		if (N%2 == 0) return f( pow(a, N/2, f), pow(a, N/2, f) );
		return f( pow(a, N-1, f), a );
	}

template <class Op, template<class> class BOp>
Op powLinear(const Op& a, size_t N, const BOp<int>& f)
{
	Op result = f.unity();
	for (size_t i = 0; i < N; ++i) {
		result = f(result, a);
	}
	return result;
}

int main()
{
	NumberBinFunc<int> f;
	for (int a = 0; a < 10; ++a) {
		for (int n = 0; n < 10; ++n) {
			REQUIRE(pow(a, n, f) == powLinear(a, n, f), "Results differ, " << powLinear(a, n, f) << " expected, but " << pow(a, n, f) << " provided.");
		}
	}
	MatrixBinFunc<int> f_pr(2);
	for(int a = 1; a < 6; a++)
		for(int b = 1; b < 6; b++)
			for(int c = 1; c < 6; c++)
				for(int d = 1; d < 6; d++)
					for(int deg = 1; deg < 6; deg++) {
						int x[4]={a,b,c,d};
						VMatrix<int> matr_p(x,2,2);
						REQUIRE(pow(matr_p, deg, f_pr) == powLinear(matr_p, deg, f_pr), "Results differ then expected.");
					}

	int curr, t;
	cin >> t;
	if(t <= 0) return 0;
	int m_s = t;
	VMatrix<int> matr(m_s,m_s);
	for (int i = 0; i < m_s; i++)
		for (int j = 0; j < m_s; j++)
		{
			cin >> curr;
			matr.set(curr,i,j);
		}
	int deg;
	MatrixBinFunc<int> f_matr(m_s);
	cin >> deg;
	VMatrix<int> val = pow(matr, deg, f_matr);
	for (int i = 0; i < m_s; i++)
		for (int j = 0; j < m_s; j++)
		{
			cout << val(i,j) << "  ";
		}
	getch();
	return 0;
}