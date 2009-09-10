#include <stdio.h>
#include <estiva/ary.h>
#include <estiva/mx.h>

void estiva_init_matrix(MATRIX **Ap, long i, long j){
  MATRIX *T;
  if(*Ap == NULL) *Ap = malloc(sizeof(MATRIX));
  T = *Ap;
  T->m = i;
  T->n = j;
  T->I = 1;
  T->J = 1;
  ary2(T->A, T->m, T->n);
  ary2(T->IA,T->m, T->n);
}

double estiva_read_matrix(MATRIX *T, long i, long j){
  register double **A;
  register long   J,  *IAi, **IA, n;
  A  = T->A;
  IA = T->IA;
  n  = T->n;
  IAi = IA[i-1];
  for(J=0; J<n; J++){
    if(IAi[J]==0) return       0.0;
    if(IAi[J]==j) return A[i-1][J];
  } 
  return 0.0;
}

void estiva_write_matrix(MATRIX *T, long i, long j, double a){
  register double **A;
  register long  J, **IA, *IAi, IAiJ, n;
  A  = T->A;
  IA = T->IA;
  n  = T->n;
  if ( a == read_matrix(T,i,j) ) return;
  IAi = IA[i-1];
  for(J=0; J<n; J++){
    IAiJ = IAi[J];
    if (IAiJ == 0 || IAiJ ==j){
      A[i-1][J] = a;
      if (IAiJ==0) IAi[J] = j; 
      return;
    }
  } 
  fprintf(stderr,"init_matrix(m,n)'s n is too short\n");
  exit(1);
}

double *estiva_matrix(MATRIX *T, long i, long j){

  write_matrix(T,T->I,T->J,T->a);
  T->I = i; 
  T->J = j;
  T->a = read_matrix(T,i,j);
  return &(T->a);
}

void estiva_flush_matrix(MATRIX *T){
  matrix(T,1,1);
}