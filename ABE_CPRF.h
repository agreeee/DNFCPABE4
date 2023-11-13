#ifndef __ABE_CPRF_H__
#define __ABE_CPRF_H__
#include "pbc-class.h"
#include "param.h"
#include "SM3.h"

void PRF_Setup(unsigned char Pmsk[][32]);
void Text(unsigned char arr[], unsigned char data[], int start, int end,
	int index, unsigned char vec[][dt], int* count);
void PRF_Eval(unsigned char a[][32], bool* x, unsigned char* b);
void PRF_Const(unsigned char Pmsk[][32], int f_psi[][dt], bool f_phi[][dt],
	unsigned char sk_f[][32]);
void PRF_KeySim(int f_psi[][dt], bool f_phi[][dt], unsigned char sk_f[][32]);
void PRF_CEval(int pos, int f_psi[][dt], bool f_phi[][dt], bool* x,
	unsigned char sk_f[][32], unsigned char* b);


#endif // !__CPRF_H__