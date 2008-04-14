// tmat.h - simple STL matrix class
//
//  PliaTech International 2002-2004
// Author: Michael B. Pliam
// Originated Oct 5, 2002
// Revised January 29, 2004

// #pragma warning (disable: 4786)

#ifndef _TMAT_
#define _TMAT_

//#include <complex>

#include <string>
#include <deque>
#include <vector>
#include "poujol.h"
//#include <fstream>
using namespace std;


// #include <crtdbg.h>

//////////////////////////////////////////////////////////////////////
// tmat class

template <class T>
class tmat
{
public:
	double tab[9];

	// Attributes
	long m_nrows;		// number of rows
	long m_ncols;		// number of columns

public:
	// constructors and destructors
	tmat()
	{
		m_nrows = 3;
		m_ncols = 3;
		// resize(0);
	}

	tmat(long row, long col)
	{
		m_nrows = row;
		m_ncols = col;
		// resize(row * col);
	}

	tmat(const tmat<T> &a)	// copy
	{
        m_nrows = a.m_nrows;
        m_ncols = a.m_ncols;
		
		tab[0] = a.tab[0];
		tab[1] = a.tab[1];
		tab[2] = a.tab[2];
		tab[3] = a.tab[3];
		tab[4] = a.tab[4];
		tab[5] = a.tab[5];
		tab[6] = a.tab[6];
		tab[7] = a.tab[7];
		tab[8] = a.tab[8];
	}

	tmat &operator = (const tmat<T> &a)	// assignment
	{
        m_nrows = a.m_nrows;
        m_ncols = a.m_ncols;

		tab[0] = a.tab[0];
		tab[1] = a.tab[1];
		tab[2] = a.tab[2];
		tab[3] = a.tab[3];
		tab[4] = a.tab[4];
		tab[5] = a.tab[5];
		tab[6] = a.tab[6];
		tab[7] = a.tab[7];
		tab[8] = a.tab[8];
		return *this;
	}

	virtual ~tmat()
	{
		m_nrows = 0;
		m_ncols = 0;
	}

// Implementation
	long nrows() { return m_nrows; }

	long ncols() { return m_ncols; }

	/*void resize(long rows, long cols)
	{
		m_nrows = rows;
		m_ncols = cols;
		resize(rows * cols);			
	}*/

	void show()
	{
		/*long i,j,k; k = 0;
		cout << m_nrows << "  "<< m_ncols << endl;
		for(i=0; i<=m_nrows; i++) {
			for(j=0; j<=m_ncols; j++ ) {
				cout << m_vec(k) << "   "; k++;
			}cout << endl;
		}cout << endl;*/

	}

	//  error output
	void cerror(char *text) 
	{
		printf("%s\n", text);

	}

	// indexing operators
	T &operator() (long i)	// 1d indexing
	{
		if ( i < 1 || i > m_nrows * m_ncols )
		cerror("operator(): index out of range");

		return tab[i];
	}

	T &operator() (long i, long j) // 2d indexing
	{
		//if (i < 1 || i > m_nrows || j < 1 || j > m_ncols)
		//cerror("operator(): index out of range");

		//long li = i-1;
		//long lj = j-1;
		long lc = m_ncols;
		return (*this).tab[i * lc + j];
		//return at(i *lc + lj);
	}

	// output
	friend ostream &operator << (ostream &s, tmat<T> &m)  
	{	
		s << m.nrows() << "  " << m.ncols() << endl;
		for(int i=0;i<m.nrows();i++) {
			for(int j=0;j<m.ncols();j++) {
				s << m(i,j) << "  ";
			} s << endl;
		}s << endl;

		return s;
	}	

	void mul(tmat<T> &b) {
		tab[0] = tab[0]*b.tab[0] + tab[1]*b.tab[3] + tab[2]*b.tab[6];
		tab[1] = tab[0]*b.tab[1] + tab[1]*b.tab[4] + tab[2]*b.tab[7];
		tab[2] = tab[0]*b.tab[2] + tab[1]*b.tab[5] + tab[2]*b.tab[8];

		tab[3] = tab[3]*b.tab[0] + tab[4]*b.tab[3] + tab[5]*b.tab[6];
		tab[4] = tab[3]*b.tab[1] + tab[4]*b.tab[4] + tab[5]*b.tab[7];
		tab[5] = tab[3]*b.tab[2] + tab[4]*b.tab[5] + tab[5]*b.tab[8];

		tab[6] = tab[6]*b.tab[0] + tab[7]*b.tab[3] + tab[8]*b.tab[6];
		tab[7] = tab[6]*b.tab[1] + tab[7]*b.tab[4] + tab[8]*b.tab[7];
		tab[8] = tab[6]*b.tab[2] + tab[7]*b.tab[5] + tab[8]*b.tab[8];
	}

	void mul(float &x, float &y) {
		float X, Y;
		X = tab[0] * x + tab[1] * y + tab[2];
		Y = tab[3] * x + tab[4] * y + tab[5];
		x = X;
		y = Y;
	}


	tmat<T> operator * (tmat<T> &b) {
		if(ncols() != b.nrows()) {
			cerror("matrices do not conform for multiplication\n");
			return *this;
		}
		mul(b);
		return *this;
	}

	// friends
	// friend tmat<T> operator + (tmat<T> a, tmat<T> b);
	// friend tmat<T> operator - (tmat<T> a, tmat<T> b);
	// friend tmat<T> operator * (tmat<T> a, tmat<T> b);


};


template<class T>
tmat<T> operator + (tmat<T> a, tmat<T> b)
{
	if (a.nrows() != b.nrows() || a.ncols() != b.ncols()) {
		a.cerror("matrices do not conform in addition\n");
		return a;
	}

	tmat<T> c(a.nrows(), a.ncols());
	for (long i = 1; i <= c.nrows(); i++)
		for (long j = 1; j <= c.ncols(); j++) 
			c(i, j) = a(i, j) + b(i, j);
		return c;
	}

template <class T>
tmat<T> operator - (tmat<T> a, tmat<T> b)
{
	if (a.nrows() != b.nrows() || a.ncols() != b.ncols()) {
		a.cerror("matrices do not conform in subtraction\n");
		return a;
	}

	tmat<T> c(a.nrows(), a.ncols());
	for (long i = 1; i <= c.nrows(); i++)
		for (long j = 1; j <= c.ncols(); j++) 
			c(i, j) = a(i, j) - b(i, j);
		return c;
	}

template <class T>
tmat<T> operator * (tmat<T> a, tmat<T> b)
{
	if(a.ncols() != b.nrows()) {
		a.cerror("matrices do not conform for multiplication\n");
		return a;
	}

	T sum;
	tmat<T> c(a.nrows(), b.ncols());
	for(long i=0;i<a.nrows();i++) {
		for(long j=0;j<b.ncols();j++) {
			sum = 0;
			for(long k=0;k<a.ncols();k++)
				sum += a(i,k) * b(k,j);
			c(i,j) = sum;
		}
	}

	return c;
}

// tmat x-class functions

#include <math.h>

template <class T>
tmat<T> transpose(tmat<T> m)
{
	tmat<T> c(m.ncols(), m.nrows());
	for(long i=0;i<=m.ncols();i++) {
		for(long j=0;j<=m.nrows();j++) {
			c(i,j) = m(j,i);
		}
	}
	return c;

}// transpose(tmat<T> m)

template <class T>
tmat<T> add(tmat<T> a, tmat<T> b)
{
	if(a.ncols() != b.ncols() || a.nrows() != b.nrows()) {
		//cerror("matrices do not conform for addition\n");
		return a;
	}

	tmat<T> c(a.nrows(), a.ncols());
	for(long i=0;i<=a.nrows();i++) {
		for(long j=0;j<=a.ncols();j++) {
			c(i,j) = a(i,j) + b(i,j);
		}
	}
	return c;

}// add(tmat<T> a, tmat<T> b)

template <class T>
tmat<T> subtract(tmat<T> a, tmat<T> b)
{
	if(a.ncols() != b.ncols() || a.nrows() != b.nrows()) {
		//cerror("matrices do not conform for subtraction\n");
		return a;
	}

	tmat<T> c(a.nrows(), a.ncols());
	for(long i=0;i<=a.nrows();i++) {
		for(long j=0;j<=a.ncols();j++) {
			c(i,j) = a(i,j) - b(i,j);
		}
	}
	return c;

}// subtract(tmat<T> a, tmat<T> b)

template <class T>
tmat<T> mul(tmat<T> a, tmat<T> b)
{
	if(a.ncols() != b.nrows()) {
		//cerror("matrices do not conform for multiplication\n");
		return a;
	}

	T sum;
	tmat<T> c(a.nrows(), b.ncols());
	for(long i=0;i<=a.nrows();i++) {
		for(long j=0;j<=b.ncols();j++) {
			sum = 0;
			for(long k=0;k<=a.ncols();k++)
				sum += a(i,k) * b(k,j);
			c(i,j) = sum;
		}
	}
	return c;
}// mul(tmat<T> a, tmat<T> b)

template <class T>
tmat<T> sweep(long k1, long k2, tmat<T> &a)
{
	//double eps = 1.0e-8;
	T eps = 1.0e-8;
	T d;
	long i, j, k, n, it;
	
	if (a.ncols() != a.nrows())
		a.cerror("sweep: matrix a not square\n");

	n = a.nrows();
	if (k2 < k1) { k = k1; k1 = k2; k2 = k; }

	for (k = k1; k <= k2; k++) {
		if ( fabs( a(k, k) ) < eps)
			for (it = 1; it <= n; it++)
				a(it, k) = a(k, it) = 0.0;
		else {
			d = 1.0 / a(k, k);
			a(k, k) = d;
			for (i = 1; i <= n; i++) 
				if (i != k) 
					a(i, k) *= (T) - d;
			for (j = 1; j <= n; j++) 
				if (j != k)
					a(k, j) *= (T) d; 
			for (i = 1; i <= n; i++) {
				if (i != k) {
					for (j = 1; j <= n; j++) {
						if (j != k)
							a(i, j) += a(i, k) *a(k, j) / d;
					} // end for j
				} // end for i != k
			} // end for i
		} // end else
	} // end for k
	return a;

}// sweep(long k1, long k2, tmat<T> &a)

template <class T>
tmat<T> inv(tmat<T> &a)
{
	//printf("typeid(a(1,1)).name() == %s\n", typeid(a(1,1)).name());

	if( strcmp(typeid(a(0,0)).name(), "double") != 0 && 
		strcmp(typeid(a(0,0)).name(), "float") != 0 ) {
		a.cerror("not a floating point type\n");
		return a;
	}

	if (a.ncols() != a.nrows())
          a.cerror("INVERSE: matrix a not square\n");

        tmat<T> b = a;
        sweep(1, b.nrows(), b);
        return b;

}   // inverse 

template <class T>
tmat<T> submatrix(tmat<T> a, long lr, long r, long lc, long c)
// returns a designated submatrix, sa, of matrix a
{
	long m = a.nrows(); long n = a.ncols();
	if ((r > m) || (c > n)) { 
		//cerror("index out of range\n"); 
		return a;
	}
        
	long r2 = r - lr + 1; long c2 = c - lc + 1;
	tmat <T> b( r2, c2);
    long lrm1 = lr - 1; long lcm1 = lc - 1;
    for (long i = 1; i <= r2; i++)  
		for (long j = 1; j <= c2; j++) 
			b(i, j) = a(lrm1 + i, lcm1 + j);
	return b;

}// submatrix(tmat<T> a, long lr, long r, long lc, long c)


/////////////////////////////////////////////////////////////////////
// determinant algorithms and definitions

#define VERY_SMALL	1.0e-12
#define VERY_LARGE	1.0E+12

// mine

//#define zeroc complex<double>(0,0)
//#define zeroc 0.0;

//#define ZERO 1.0E-8
#define ZERO VERY_SMALL


template <class T>
void Pivot(tmat<T> &Data, long RefRow, T &Determ, bool &DetEqualsZero)
{
  DetEqualsZero = true;
  long NewRow = RefRow;
  while (DetEqualsZero && (NewRow < Data.nrows())) {
    NewRow++;
    if (fabs(Data(NewRow, RefRow)) > ZERO) {
      for (long i = 1; i <= Data.nrows(); i++) {
        T temp = Data(NewRow, i);
        Data(NewRow, i) = Data(RefRow, i);
        Data(RefRow, i) = temp;
      }
      DetEqualsZero = false;
      Determ = -Determ;   // row swap adjustment to det
    }
  }

}// Pivot(Matrix<T> &Data, long RefRow, T &Determ, bool &DetEqualsZero)

/*template <class T>
T det(tmat<T> &Datain)
{
  T Determ;
  Determ = 1.0;
  if (Datain.nrows() == Datain.ncols()) {

    long Dimen = Datain.nrows();
    tmat<T> Data( Dimen, Dimen);

    Data  = Datain;
    T Coef;
    long Row, RefRow = 0;
    //enum bboolean DetEqualsZero = false;
	bool DetEqualsZero = false;

    while (!(DetEqualsZero) && (RefRow < Dimen - 1)) {
      RefRow++;
      if (fabs(Data(RefRow, RefRow)) < ZERO)
        Pivot(Data, RefRow, Determ, DetEqualsZero);
      if (!(DetEqualsZero))
        for (Row = RefRow + 1; Row <= Dimen; Row++)
          if (fabs(Data(Row, RefRow)) > ZERO) {
            Coef = -((T) Data(Row, RefRow)) /
            ((T) Data(RefRow, RefRow));
            for (int i = 1; i <= Dimen; i++)
              Data(Row, i) = Data(Row, i) +
                (T) (Coef*((T) Data(RefRow,i)));
          }
          Determ *= Data(RefRow, RefRow);
    }
    Determ = (DetEqualsZero) ? zeroc.real() : Determ * Data(Dimen, Dimen);
  }
  else Datain.cerror("Matrix is not square\n");
  return Determ;

}*/
#undef ZERO

// end determinant algorithms
/////////////////////////////////////////////////////////////////////

template<class T>
tmat<T> hconcat(tmat<T> a, tmat<T> b)
// horizontally concatenate matrix a with matrix b
{
    long ar=a.nrows(); long ac=a.ncols();
    long br=b.nrows(); long bc=b.ncols();

	if (ar != br) {
		cout << "hconcat: matrices have differing row numbers" << endl;
		return a;
	}
	long i,j;
    tmat<T> c(ar, ac+bc, (T)0);
	for(i=0;i<=ar;i++) {
		for(j=0;j<=ac+bc;j++) {
			if(j <= ac) c(i,j) = a(i,j);
			if(j > ac) c(i,j) = b(i,j-ac);
		}
	}
    return c;

}// hconcat(tmat<T> a, tmat<T> b)

template<class T>
tmat<T> flop(tmat<T> a)
// flop matrix top to bottom
{
	long i, j;
	long m = a.nrows(); long n = a.ncols();
	tmat<T> t1; t1 = a; tmat<T> t2(m, n);
	for (i = 1; i <= m; i++) {  
		for (j = 1; j <= n; j++)
		t2(m - i + 1, j) = t1(i, j);
	}
	return t2;

}// flop(tmat<T> a)

template<class T>
T diagprod(tmat<T> a)
// returns the diagonal product of matrix a
{
	T dp = 1;
	for (long i = 1; i <= a.nrows(); i++)   // wasteful algo !!
		for (long j = 1; j <= a.ncols(); j++) 
			if (i == j) dp *= a(i,j);
	return dp;

}// diagprod(tmat<T> a)

template<class T>
void delcol(tmat<T> &a, long c)
// delete by reference a selected matrix column
{
	long ar = a.nrows(); long ac = a.ncols();
	if(c <= 0 || c > ac) return; // invalid col index
	tmat<T> s, s1, s2;
	if(c == 1) { s = submatrix(a, 1, ar, 2, ac); goto quit;}
	if(c == ac) { s = submatrix(a,0, ar, 1, ac - 1); goto quit;}
	s1 = submatrix(a, 1, ar, 1, c - 1);
	s2 = submatrix(a, 1, ar, c + 1, ac);
	s = hconcat(s1, s2);
	quit: a = s;

}// delcol(tmat<T> a, long n)a

template <class T>
T perm(tmat<T> a)
// Faulty algorithm for calculating the permanent of a 
// real square matrix
{
	long i, n;
	T prm = 0;
	n = a.ncols();		// ?? rows might be better
	tmat<T> s, o; 
	o = hconcat(a,a);	// concat for n easy diagonal products
	s=flop(o);			// save flopped version 

	for(i=0;i<=n;i++) {
		prm += diagprod(o);
		delcol(o, 1);
		prm += diagprod(s);
		delcol(s, 1);
	}

	return prm;

}// Perm(tmat<T> a)


#endif // _TMAT_

//////////////////////////////////////////////////////////////////////
// PliaTech 2002


// Copyright (c) 2002 by M.B. Pliam.  ALL RIGHTS RESERVED. 
// Consult your license regarding permissions and restrictions.
//

//
// This file is derived from software bearing the following
// restrictions:
//
// Copyright (c) 2002
// PliaTech International
//
// Permission to use, copy, modify, distribute and sell this
// software and its documentation for any purpose is hereby
// granted without fee, provided that the above copyright notice
// appear in all copies and that both that copyright notice and
// this permission notice appear in supporting documentation.
// PliaTech International makes no representations about the
// suitability of this software for any purpose. It is provided
// "as is" without express or implied warranty.
//

