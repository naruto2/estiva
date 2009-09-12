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


int etalu(void* pA,double* b)
{
  /* (i)  �����η��ȼ���                                                */
  D_     A, E      ;/*    D  �켡������  A(L), E(L2)                    */
  I_     IA, IE    ;/*    I  �켡������  IA(L), IE(L2)                  */
  D_     B, X, WE  ;/*    D  �켡������  B(M), X(M), WE(M)              */
  I_     NW, IP    ;/*    I  �켡������  NWORK(0:M), IP(M)              */
  I_     NETA      ;/*    I  �켡������  NETA(-1:2*M)                   */
  static long   L, L2, M, NUM, IS, IER                                  ;
  static double EPS                                                     ;

  static long   i, j, k                                                 ;

  M   = dim1(pA) ;

  ary1( NW, M+1 );

  for(j=1;j<=M;j++)
    for(i=1;i<=M;i++)
      if(A(i,j) != 0.0) NW[j]++;
  NW[0] = 0;


  for(L=0,j=1;j<=M;j++) L += NW[j];

  L2 = L*2;
  

  k=0;
  for(i=1;i<=M;i++) for(j=1;j<=M;j++) if( A(i,j) != 0.0) k++;
  fprintf(stderr,"M L L2 k %d %d %d %d\n",M,L,L2,k); fflush(stderr);  

  ary1(  A, L ); ary1(  E, L2 );
  ary1( IA, L ); ary1( IE, L2 );
  ary1(  B, M ); ary1(  X, M  ); ary1( WE, M );
                 ary1( IP, M  );
  ary1(NETA, 2*M+2 );

  /* (ii) ��ץ���� �� ���֥롼����                                  */
  /*      ���֥롼�����CALL����Ȥ��ˤ�, �Ĥ����ͤ�Ϳ����.             */
  /*  A   : ����A�γ�����󥼥����Ǥ��󤴤Ȥ��¤٤Ƥ���.                */
  /*  IA  : ����A�γ����Ǥ��б��������ֹ���󤴤Ȥ��¤٤Ƥ���.          */
  /*  L   : ��ץ����ˤ���������A���礭��                           */
  /*  L2  : ��ץ����ˤ���������E���礭��  L2>L                     */
  /*  B   : ���ե٥��ȥ�b������Ƥ���.                                  */
  /*  NW  : ����A�γ�����󥼥����ǿ������¤������. NW(0)=0�Ȥ���.     */
  /*  M   : ����A�����������Ƥ���.                                    */
  /*  EPS : �ð�����Ƚ���ͤ�Ϳ����. �դĤ��� 1.0��10^(-14)              */

  k=0;
  for(j=1;j<=M;j++)
    for(i=1;i<=M;i++)
      if(A(i,j) != 0.0){
	A[k] = A(i,j); 
	IA[k] = i;
	k++;
      }

  B   = &b[1]    ;

  EPS = 1.0e-14  ;

  /* (a) ��󥯤���ˡ                                                   */
  /* CALL ETALU(A,IA,L,L2,B,NW,M,EPS,NUM,X,E,IE,WE,NETA,IP,IS,IER)      */

  etalu_(A,IA,&L,&L2,B,NW,&M,&EPS,&NUM,X,E,IE,WE,NETA,IP,&IS,IER);

  for(i=0; i<M; i++) B[i] = X[i+1];
  return IER;
}

