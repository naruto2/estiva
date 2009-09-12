#include <stdio.h>
#include "ary.h"
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


int pcgs(void* pA, double* b)
{

  /* (i)  �����η��ȼ���                                                */
  D_     D, B         ;/*    D  �켡������ D(N), B(N)                   */
  D__    A            ;/*    D  �󼡸����� A(N1, 2 * NL)                */
  I__    IA           ;/*    I  �󼡸����� IA(N1, 2 * NL)               */
  D_     R            ;/*    D  �켡������ R(N)                         */
  long   NL, N1, N, ITR, IER                                            ;
  double EPS, S                                                         ;
  D_     X, DD, P, Q  ;/*    D  �켡�������, ���Ǥ� 0��N               */
  I_     M            ;/*    I  �켡������  M(2 * N)  �����            */

  /*--    D����R�ޤǤ�, EPS����S�ޤǤΰ�����, ���֥롼����PCG��Ʊ��   --*/  

  D_     R0, E, H     ;/*    D  �켡������  ���ǿ���N                   */
  D_     W            ;/*    D  �켡������  W(0:N)                      */
  
  long   i, j, k      ;
  
  N   = dim1(pA)      ;
  NL  = count_NL(pA)  ;
  
  ary1(  D, N   )         ;
  ary2(  A, 2*NL, N+2*NL );
  ary2( IA, 2*NL, N+2*NL );
  ary1(  R, N   )         ;
  ary1(  X, N+1 )         ;  ary1( DD, N+1 );  ary1( P, N+1 );  ary1( Q, N+1 ); 
  ary1(  M, 2*N )         ;

  ary1( R0, N   )         ;  ary1(  E, N   );  ary1( H, N   );
  ary1(  W, N+1 )         ;
  

  /* (ii) ��ץ���� �� ���֥롼����                                  */
  /*      ���֥롼�����CALL����Ȥ��ˤ�, �Ĥ����ͤ�Ϳ����.             */
  /*  D   : ����D����1����n���֤˹���A���г����Ǥ�����Ƥ���            */
  /*  N   : ����A�ιԿ�������Ƥ���.                                    */
  /*  N1  : ����A�ιԿ�������Ƥ���. N1��N+2*NL �Ǥʤ��Ȥ����ʤ�.       */
  /*  NL  : ����A�γƹԤˤ������󥼥����ǿ��κ����ͤ�����Ƥ���.        */
  /*  B   : ϢΩ�켡�������α��դ�����Ƥ���.                           */
  /* EPS  : ��«Ƚ���֤�����Ƥ���. �դĤ��� 1.��10^(-7)                */
  /* ITR  : ���ڤ�ޤǤκ��緫�֤����������Ƥ���.                     */

  /*--    D, N, N1, NL, B, EPS, ITR �ˤĤ��Ƥ�, ���֥롼����PCG��Ʊ�� --*/

  /*  A   : ����A�γƹ�1��NL���Ǥ�, ����A�β�������ʬ�γƹԤ��󥼥�     */
  /*        ���Ǥ������. �ޤ�, �ƹԤ�NL+1��2*NL���Ǥ�, �廰����ʬ      */
  /*        �γƹԤ��󥼥����Ǥ������. ������, �ƹԤ��г����Ǥ�����D�� */
  /*        �����.                                                     */
  /* IA   : ����A�����줿���Ǥ����ֹ��, �б�������֤�����Ƥ���.      */
  /*  S   : LUCGSˡ�ΤȤ� 0., MLUCGˡ�ΤȤ���(>0)������Ƥ���.          */  

  for(i=1; i<=N; i++) D[i-1] = A(i,i); 

  for(i=1; i<=N; i++)    for(k=0, j=1; j<i; j++)    if(A(i,j) != 0.0){
    A[k][i-1]  = A(i,j)  ;
    IA[k][i-1] = j       ;
    k++                  ;
  }

  for(i=1; i<=N; i++)    for(k=NL, j=i+1; j<=N; j++) if(A(i,j) != 0.0){
    A[k][i-1]  = A(i,j)  ;
    IA[k][i-1] = j       ;
    k++                  ;
  }
      

  N1  =  N+2*NL;

  B   = &b[1]  ;
  EPS = 1.0e-7 ;
  ITR = N      ;
  S   = 0.     ;
  
  /* (a) ��󥯤���ˡ                                                   */
  /* CALL PCGS(D,A,IA,N,N1,NL,B,EPS,ITR,S,X,DD,P,Q,R,R0,E,H,W,M,IER)    */

  pcgs_(D,A[0],IA[0],&N,&N1,&NL,B,&EPS,&ITR,&S,X,DD,P,Q,R,R0,E,H,W,M,&IER);


  for(i=0; i<N; i++) B[i] = X[i+1];
  return IER;
}
