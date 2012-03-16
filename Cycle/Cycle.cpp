// Cycle.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;

#define REQUIRE(cond, message) \
	if (!(cond)) { cerr << message << endl; assert(false); }


template <typename T>
class function1 {
public:
	function1() {};
	
	int operator()(const T& x, const int& n) {
		//cout << "x:" << x << ", n:" << n << endl;
		return (x*x)%n;
	}
};

template <typename T>
	T search_element_in_cycle(int n, T x0, function1<T> fun1) {
		T x_curr = x0;
		for (int i = 0; i<2*n; i++) {
			x_curr = fun1(x_curr, n);
		}
		return x_curr;
	}

template <typename T>
	int cycle_length_calc(int n, T x0, function1<T> fun1){
		T x_w = search_element_in_cycle(n,x0,fun1);
		T x_curr = x_w;
		int l = 1;
		while (fun1(x_curr, n) != x_w) {
			l++;
			x_curr = fun1(x_curr, n);
		}
		//cout << "cycle length = " << l << endl;
		return l;
	}

	/*int tail_length_calc(int n, int x0, function1 fun1){
		int x_f = x0;
		int f_l_x = x0;
		int l = cycle_length_calc(n, x0, fun1);
		for( int k = 0; k < l; k++) {
			f_l_x = fun1(f_l_x, n);
		}
		int len = 0;
		while ( f_l_x =! x_f) {
			x_f = fun1(x_f, n);
			f_l_x = fun1(f_l_x, n);
			len ++;
		};
		cout << "tail_length = " << len << endl;
		return len;
	}*/

template <typename T>
	bool if_match(T x_curr, vector<T> x_vec) {
		vector<int>::iterator it;
		for ( it=x_vec.begin(); it < x_vec.end(); it++ )
			if ( (*it) == x_curr )
				return true;
		return false;
	}

template <typename T>
	int linear_memory(int n, T x0, function1<T> fun1) {
		vector<T> x_vec;
		//if (x0 = fun1(x0, n) ) return 0;
		T x_curr = x0;
		while(!(if_match(x_curr, x_vec))){
			x_vec.push_back (x_curr);
			x_curr = fun1(x_curr, n);
		}
		while(!(x_vec.back() == x_curr)) 
			x_vec.pop_back();
		//cout << "tail_len = " << x_vec.size() - 1 << endl << " --- " << endl;
		return x_vec.size() - 1;
	}

template <typename T>
	int tail_length_calc(int n, T x0, function1<T> fun1){
		int l = cycle_length_calc(n, x0, fun1);
		if (l = 1) {		// Если цикл длины один, то сравниваем элемент с собой 
			T x_curr = x0;
			T f_x_curr = fun1(x_curr, n);
			int len = 0;
			while (x_curr != f_x_curr)
			{
				x_curr = f_x_curr;
				f_x_curr = fun1(f_x_curr, n);
				len++;
			}
			//cout << "tail_len = " << len << endl;
			return len;
		}
		T x_f = x0;
		T f_l_x = x0;
		for( int k = 0; k < l; k++) {
			f_l_x = fun1(f_l_x, n);
		}
		int len = 0;
		while ( f_l_x =! x_f) {
			x_f = fun1(x_f, n);
			f_l_x = fun1(f_l_x, n);
			len ++;
		};
		//cout << "tail_len = " << len << endl << " --- " << endl;
		return len;
	}

int main()
{
	
	function1<int> fun1;
	//int n, x0;
	//int k;
	/*cin >> k;
	for (int i = 0; i < k; i++) {
		cin >> n >> x0;
		//tail_length_calc(n,x0,fun1); }
		linear_memory(n,x0,fun1); }*/
	for (int n = 0; n < 7; ++n) {
		for (int x0 = 0; x0 < n; ++x0) {
			REQUIRE(tail_length_calc(n, x0, fun1) == linear_memory(n, x0, fun1), "Results differ, " << linear_memory(n, x0, fun1) << " expected, but " << tail_length_calc(n, x0, fun1) << " provided.");
		}
	}
	cout << 1;
	getch();
	return 0;
}
