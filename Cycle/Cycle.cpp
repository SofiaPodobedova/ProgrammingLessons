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
class Transformation {
public:
	int operator()(const T& x) const {
		return (x*x)%n;
	}
	void setMod( int mod ) { n = mod; }
private:
	int n;
};

template <typename T>
	T search_element_in_cycle(T x0, const Transformation<T>& transformation) {
		T x_curr = x0;
		for (int i = 0; i<2*n; i++) {
			x_curr = transformation(x_curr);
		}
		return x_curr;
	}

template <typename T>
	int cycle_length_calc(T x0, Transformation<T>& transformation){
		T x_w = search_element_in_cycle(x0,transformation);
		T x_curr = x_w;
		int l = 1;
		while (transformation(x_curr) != x_w) {
			l++;
			x_curr = transformation(x_curr);
		}
		return l;
	}

template <typename T>
	bool if_match(T x_curr, vector<T> x_vec) {
		vector<int>::iterator it;
		return find(x_vec.begin(), x_vec.end(), x_curr) != x_vec.end();
	}

template <typename T>
	int linear_memory(T x0, Transformation<T>& transformation) {
		vector<T> x_vec;
		T x_curr = x0;
		while(!(if_match(x_curr, x_vec))){
			x_vec.push_back (x_curr);
			x_curr = transformation(x_curr);
		}
		while(!(x_vec.back() == x_curr)) 
			x_vec.pop_back();
		return x_vec.size() - 1;
	}

template <typename T>
	int tail_length_calc(T x0, Transformation<T>& transformation){
		int l = cycle_length_calc(x0, transformation);
		T x_f = x0;
		T f_l_x = x0;
		for( int k = 0; k < l; k++) {
			f_l_x = transformation(f_l_x);
		}
		int len = 0;
		while ( f_l_x =! x_f) {
			x_f = transformation(x_f);
			f_l_x = transformation(f_l_x);
			len ++;
		};
		return len;
	}

int main()
{
	Transformation<int> fun1;
	for (int n = 0; n < 7; ++n) {
		fun1.setMod( n );
		for (int x0 = 0; x0 < n; ++x0) {
			REQUIRE(tail_length_calc(x0, fun1) == linear_memory(x0, fun1), "Results differ, " << linear_memory(x0, fun1) << " expected, but " << tail_length_calc(x0, fun1) << " provided.");
		}
	}
	cout << 1;
	getch();
	return 0;
}
