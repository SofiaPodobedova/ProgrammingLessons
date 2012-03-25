#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

template<class DBL>
class VMatrix {
protected:
	DBL** m_a;	//элементы матрицы
public:
	int m_n[2];		//размеры матрицы

	VMatrix() {
		m_a=NULL;
	}
	VMatrix(int m1, int m2 /*=0*/)
		//m2==0 -> matrix is square 
	{
		m_a=NULL;
		Alloc(m1, m2);
	}
	VMatrix(DBL**a, int m1, int m2 /*=0*/)
	{
		m_a=NULL;
		Alloc(m1, m2>0 ? m2 : m1);
		for(int i=0; i<m_n[0]; i++)
			for(int j=0; j<m_n[1]; j++)
				m_a[i][j]=a[i][j];
	}
	VMatrix(DBL*a, int m1, int m2 /*=0*/)
	{
		m_a=NULL;
		Alloc(m1, m2>0 ? m2 : m1);
		int n=0;
		for(int i=0; i<m_n[0]; i++)
			for(int j=0; j<m_n[1]; j++)
				m_a[i][j]=a[n++];
	}
	VMatrix(const VMatrix& b)	//Copy constructor
	{
		int m, n;
		m_a=NULL;
		Alloc(b.m_n[0], b.m_n[1]);
		for(m=0; m<m_n[0]; m++)
			for(n=0; n<m_n[1]; n++)
				m_a[m][n]=b.m_a[m][n];
	}
	~VMatrix()
	{
		Free();
	}
protected:	
	int Alloc(int m1 /*=1*/, int m2=0, DBL dInit=0.)
		//m2==0 -> matrix is square 
	{
		if(m_a!=NULL)
			Free();
		m_n[0]=m1;
		m_n[1]=m2>0 ? m2 : m1;

		m_a=new DBL*[m_n[0]];
		for(int i=0; i<m_n[0]; i++) {
			m_a[i]=new DBL[m_n[1]];
			for(int j=0; j<m_n[1]; j++)
				m_a[i][j]=dInit;
		}
		return 1;
	}
	void Free() {
		if(m_a!=NULL) {
			for(int i=0; i<m_n[0]; i++)
				delete [] m_a[i];
			delete [] m_a;
		}
		m_a=NULL;
	}
public:
	const VMatrix& operator=(const VMatrix& b) {
		if(m_n[0]!=b.m_n[0] || m_n[1]!=b.m_n[1]) {
			Free();
			Alloc(b.m_n[0], b.m_n[1]);
		}
		int m, n;
		for(m=0; m<m_n[0]; m++)
			for(n=0; n<m_n[1]; n++)
				m_a[m][n]=b.m_a[m][n];
		return *this;
	}
	const bool operator==(const VMatrix& b){
		if(m_n[0]!=b.m_n[0] || m_n[1]!=b.m_n[1]) return false;
		for(int m=0; m<m_n[0]; m++)
			for(int n=0; n<m_n[1]; n++)
				if (m_a[m][n]!=b.m_a[m][n]) return false;
		return true;
	}
	DBL& operator()(int i, int j) const
	{
		return m_a[i][j];
	}
	void set(DBL a, int i, int j) {
		m_a[i][j] = a;
		return;
	}
	int getSize() {
		return m_n[0];
	}
	void Transp(int m=0) {
		int i, j, dim=m ? m : __min(m_n[0], m_n[1]);
		DBL t;
		for(i=1; i<dim; i++)
			for(j=0; j<i; j++) {
				t=m_a[i][j];
				m_a[i][j]=m_a[j][i];
				m_a[j][i]=t;
			}
	}
	DBL Trace() {
		int i, dim=__min(m_n[0], m_n[1]);
		DBL t=0.;
		for(i=0; i<dim; i++)
			t+=m_a[i][i];
		return t;
	}
	int Rank() {
		int i, j, k, t, r;
		DBL x;
		r=0;
		for(j=0; j<m_n[1]; j++) {
			t=-1;
			for(i=j; i<m_n[0]; i++) {
				if(m_a[i][j]!=0) {
					if(t==-1)
						t=i;
					else {
						x=m_a[i][j]/m_a[t][j];
						for(k=j; k<m_n[1]; k++)
							m_a[i][k]-=m_a[t][k]*x;
					}
				}
			}
			if(t!=-1) {
				for(k=j; k<m_n[1]; k++) {
					x=m_a[j][k];
					m_a[j][k]=m_a[t][k];
					m_a[t][k]=x;
				}
				r++;
			}
		}
		return r;
	}
	DBL Det() {
		int i, j, k, t;
		DBL x, d=1.;
		for(j=0; j<m_n[0]; j++) {
			t=-1;
			for(i=j; i<m_n[0]; i++)
				if(m_a[i][j]!=0) {
					if(t==-1)
						t=i;
					else {
						x=m_a[i][j]/m_a[t][j];
						for(k=j; k<m_n[0]; k++)
							m_a[i][k]-=m_a[t][k]*x;
					}
				}
				if(t!=-1 && t!=j) {
					for(k=j; k<m_n[0]; k++) {
						x=m_a[j][k];
						m_a[j][k]=m_a[t][k];
						m_a[t][k]=x;
					}
					d=-d;
				}
		}
		for(i=0; i<m_n[0]; i++) {
			try {
				d*=m_a[i][i];
			}
			catch(DBL) {
				return 0.;
			}
		}
		return d;
	}
};
template<class DBL>
VMatrix<DBL> operator+(const VMatrix<DBL>& b1, const VMatrix<DBL>& b2)
{
	if(b1.m_n[0]!=b2.m_n[0] || b1.m_n[1]!=b2.m_n[1]) {
		printf("Error: different sizes\n");
		return b1;
	}
	VMatrix<DBL> c(b1.m_n[0], b1.m_n[1]);
	int i, j;
	for(i=0; i<b1.m_n[0]; i++)
		for(j=0; j<b1.m_n[1]; j++)
			c(i, j)=b1(i, j) + b2(i, j);
	return c;
}
template<class DBL>
	VMatrix<DBL> unity_matr(int m1) {
		VMatrix<DBL> c (m1, m1);
		for( int i=0; i<m1; i++)
			for(int j=0; j<m1; j++) {
				if (i == j) c.set(1, i, j);
				else c.set(0, i, j);
			}
		return c;
	}
template<class DBL>
VMatrix<DBL> operator-(const VMatrix<DBL>& b1, const VMatrix<DBL>& b2)
{
	if(b1.m_n[0]!=b2.m_n[0] || b1.m_n[1]!=b2.m_n[1]) {
		printf("Error: different sizes\n");
		return b1;
	}
	VMatrix<DBL> c(b1.m_n[0], b1.m_n[1]);
	int i, j;
	for(i=0; i<b1.m_n[0]; i++)
		for(j=0; j<b1.m_n[1]; j++)
			c(i, j)=b1(i, j) - b2(i, j);
	return c;
}
template<class DBL>
VMatrix<DBL> operator*(const VMatrix<DBL>& b1, const VMatrix<DBL>& b2)
{
	if(b1.m_n[1]!=b2.m_n[0]) {
		printf("Error: different sizes\n");
		return b1;
	}
	VMatrix<DBL> c(b1.m_n[0], b2.m_n[1]);
	int i, j, k;
	for(i=0; i<b1.m_n[0]; i++)
		for(j=0; j<b2.m_n[1]; j++)
			for(k=0, c(i, j)=0.; k<b1.m_n[1]; k++) {
				c(i, j)+=b1(i, k)*b2(k, j);
				DBL d;
				d=c(i, j);
			}
			return c;
}