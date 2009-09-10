#include <stdio.h>
#include <math.h>
#include <estiva/ary.h>
#include <estiva/esolver.h>

/* power.c : 1999 3 8 by Oguni */
// Modified by T. KUDO
// Modified by TSUKUDA Yoshio

static const int N = 3;        // ����μ���
static const double tol=1e-6;  // ��«��Ƚ�ꤹ�뾮������
static const int Itr=100;     //ȿ���κ�����

/* �桼��������η�Matrix����� */
///typedef double Matrix[N+1][N+1];
#define Matrix double **

/* �桼��������η�Vector����� */
//typedef double Vector[N+1];
#define Vector double *

double power(Matrix ,Vector ,int,int *);
void mvsub(Matrix ,Vector,Vector,int);
void normarize(Vector x,int n);
extern double estiva_maxesolver(double **A, double *x);

double estiva_maxesolver(double **A, double *x)
{
  int itr[2]={Itr, 0};
  return power(A,x,dim1(x),itr);
}

/* function power */
double power(Matrix A,Vector x,int n,int *itr)
{
  int i=0, m=0;
  double c=0.0, s=0.0, lambda=0.0, xmax=0.0;
  static Vector y;
  ary1(y,n+1);

  normarize(x,n); //x�����������롣
  for (m=0; m<itr[0]; m++){
    mvsub(A,x,y,n);
    s=0.0; c=0.0;
    for (i=1; i<=n; i++){
      s+=y[i]*y[i];
      c+=y[i]*x[i]; 
    }
    lambda=s/c;
    itr[1]=m;
    if (fabs(xmax-lambda)<tol)
      return lambda;
    xmax=lambda;
    //  cout << m << " " << lambda << "\n" ;
    for (i=1; i<=n; i++)
      x[i]=y[i]/sqrt(s);  //x��y ,�������������롣
  }
  return lambda;
}

// multmv , 
void mvsub(Matrix A,Vector b,Vector c,int n)
{
  int i, j;
  // c = A��b��׻�������ʬ
  for (i=1; i<=n; i++){
    c[i]=0.0;
    for (j=1; j<=n; j++){
      c[i]+=A[i][j]*b[j];
    }
  }
}

//�礭���򣱤ˤ����Ѵ�
void normarize(Vector x, int n)
{
  double c=0.0;
  int i;

  for (i=1; i<=n; i++)
    c+=x[i]*x[i]; 
  c = sqrt(c);
  for (i=1; i<=n; i++)
    x[i]/=c; 
}
