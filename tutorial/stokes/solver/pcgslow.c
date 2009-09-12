#include <stdio.h>
#include "ary.h"
#include "date.h"
#include "spm.h"

#define A(i,j) (*spm_double(pA,i,j))

typedef long*    I_  ;
typedef long**   I__ ;
typedef double*  D_  ;
typedef double** D__ ;

#define D_  static D_
#define D__ static D__
#define I_  static I_
#define I__ static I__

static int count_NL(void *pA)
{
  long i, j, k, N, NL ;

  N = dim1(pA);

  for(NL=0, i=1; i<=N; i++){
    for(k=0, j=1; j<=N; j++) if(A(i,j) != 0.0) k++;
    if(NL<k) NL = k;
  }
  return NL;
}


int pcgslow(void* pA, double* b)
{

  /* (i)  �����μ���                                                    */
  D_     D, B         ;/*    D  �켡������ D(N), B(N)                   */
  D__    A            ;/*    D  �󼡸����� A(N1, 2 * NL)                */
  I__    IA           ;/*    I  �󼡸����� IA(N1, 2 * NL)               */
  D_     R            ;/*    D  �켡������ R(N)                         */
  long   NL, N1, N, ITR, IER                                            ;
  double EPS, S                                                         ;
  D_     X, DD, P, Q  ;/*    D  �켡�������, ���Ǥ� 0��N               */
  I_     M            ;/*    I  �켡������  M(2 * N)  �����            */
  
  long   i, j, k      ;
  
  N   = dim1(pA)      ;
  NL  = count_NL(pA)  ;
  
  ary1(  D, N   )         ;
  ary2(  A, 2*NL, N+2*NL );
  ary2( IA, 2*NL, N+2*NL );
  ary1(  R, N   )         ;
  ary1(  X, N+1 )         ;  ary1( DD, N+1 );  ary1( P, N+1 );  ary1( Q, N+1 ); 
  ary1(  M, 2*N )         ;
  

  /* (ii) ��ץ���� �� ���֥롼����                                  */
  /*      ���֥롼�����CALL����Ȥ��ˤ�, �Ĥ����ͤ�Ϳ����.             */
  /*  D   : ����D����1����n���֤˹���A���г����Ǥ�����Ƥ���            */
  /*  A   : ����A�ˤϹ���A�β����ѹ�����ʬ���󥼥����Ǥ�Ԥ��Ȥ˺���    */
  /*        ��Ǥ���Ƥ���. ������, �г����Ǥˤ�����ʤ�. �ޤ�, ��������*/
  /*        ��ˤ�0������Ƥ���.                                        */
  /* IA   : ����A�����줿���Ǥ����ֹ��, �б�������֤�����Ƥ���.      */
  /*  N   : ����A�ιԿ�������Ƥ���.                                    */
  /*  N1  : ����A�ιԿ�������Ƥ���. N1��N+2*NL �Ǥʤ��Ȥ����ʤ�.       */
  /*  NL  : ����A�γƹԤˤ������󥼥����ǿ��κ����ͤ�����Ƥ���.        */
  /*  B   : ϢΩ�켡�������α��դ�����Ƥ���.                           */
  /* EPS  : ��«Ƚ���֤�����Ƥ���. �դĤ��� 1.��10^(-7)                */
  /* ITR  : ���ڤ�ޤǤκ��緫�֤����������Ƥ���.                     */
  /*  S   : ICCGˡ�ΤȤ� 0., MICCGˡ�ΤȤ���(>0)������Ƥ���.           */
  

  for(i=1; i<=N; i++) D[i-1] = A(i,i); 

  for(i=1; i<=N; i++)    for(k=0, j=1; j<i; j++)    if(A(i,j) != 0.0){
    A[k][i-1]  = A(i,j)  ;
    IA[k][i-1] = j       ;
    k++                  ;
  }
  
  N1  =  N+2*NL;

  B   = &b[1]  ;
  EPS = 1.0e-7 ;
  ITR = N      ;
  S   = 0.     ;

  date("pcgslow");
  pcg_(D,A[0],IA[0],&N,&N1,&NL,B,&EPS,&ITR,&S,X,DD,P,Q,R,M,&IER);

  for(i=0; i<N; i++) B[i] = X[i+1];
  return IER;
}
