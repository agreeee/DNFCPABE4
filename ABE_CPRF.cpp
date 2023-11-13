#include <string.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include "ABE_CPRF.h"

//������������Cab�ĺ���a>=b>=0
int Comb(int a, int b) {
	int i, j = a;
	if (b == 0) {
		j = 0;
	}
	else {
		for (i = a - 1; i > (a - b); i--) {
			j = j * i;
		}
		for (i = b; i > 1; i--) {
			j = j / i;
		}
	}
	return j;
}

//��λ����
int loc(int a[]) {
	int pos = 0;
	int j, k;
	for (j = 0; j < dt; j++) {
		if (a[j] > j) {
			for (k = j + 1; k <= a[j]; k++) {
				pos = pos + Comb(n - k, dt - j - 1);
			}
		}
	}
	return pos;
}

//����غ�λ�õĺ���
void coin(int a[], int b[], bool c[], int nu) {
	int i;
	nu = 0;
	for (i = 0; i < dt; i++) {
		if (a[i] == b[i]) {
			c[i] = 1;
			nu++;
		}
		else {
			c[i] = 0;
		}
	}
}


//�ж��ַ����Ƿ���ͬ
//�����ȡx�ĺ���ext
void Xext(bool arr[], bool data[], int start, int end,
	int index, unsigned char vec[], int* count) {
	if (index == dt) {
		for (int i = 0; i < dt; i++) {
			vec[*count] = vec[*count] + (data[i] << (dt - i - 1));
		}
		(*count)++;
		return;
	}
	for (int i = start; i <= end && end - i + 1 >= dt - index; i++) {
		data[index] = arr[i];
		Xext(arr, data, i + 1, end, index + 1, vec, count);
	}
}

//�����ȡT�ĺ���
void Text(unsigned char arr[], unsigned char data[], int start, int end,
	int index, unsigned char vec[][dt], int* count) {
	if (index == dt) {
		for (int i = 0; i < dt; i++) {
			vec[*count][i] = data[i];
			//printf("%d, ", data[i]);
		}
		//printf("\n");
		(*count)++;
		return;
	}
	for (int i = start; i <= end && end - i + 1 >= dt - index; i++) {
		data[index] = arr[i];
		Text(arr, data, i + 1, end, index + 1, vec, count);
	}
}

//�����Կ���ɺ���PRF_Setup,��Կ����Ϊ32���ֽ�
void PRF_Setup(unsigned char Pmsk[][32]) {
	int i, j, k;
	unsigned char mid = 0;
	struct timeb Seed;
	ftime(&Seed);
	srand(Seed.time * 1000 + Seed.millitm);
	//�����ʼ��Կp_msk
	unsigned char p_msk[32] = { 0 };
	for (i = 0; i < 32; i++) {
		p_msk[i] = rand() & 0xff;
		//printf("%d, ", p_msk[i]);
	}
	//printf("\n\n");

	//����洢���������������T
	unsigned char T[CNT][dt] = { 0 };
	unsigned char mid_0[n] = { 0 };//�洢[n]
	for (int i = 0; i < n; i++) {
		mid_0[i] = i;
	}
	unsigned char mid_1[dt] = { 0 };
	int count = 0;
	Text(mid_0, mid_1, 0, n - 1, 0, T, &count);

	//����CDT����ϣֵ
	unsigned char c[33 + dt] = { 0 };
	memcpy(c, p_msk, 32);
	for (i = 0; i < CNT; i++) {
		for (j = 0; j < dt; j++) {
			c[33 + j] = T[i][j];
		}
		for (k = 0; k < DT; k++) {
			c[32] = k;
			sm3(c, 33 + dt, Pmsk[i * DT + k]);
			//printf("%d, ", Pmsk[i * DT + k][14]);
		}
	}

}

//������������Eval(msk, x)
void PRF_Eval(unsigned char a[][32], bool* x, unsigned char* b) {
	int i, j;
	int count = 0;
	unsigned char x_t[CNT] = { 0 }; //�洢CNT���Ӵ�
	bool data[dt] = { 0 };
	Xext(x, data, 0, n - 1, 0, x_t, &count);
	for (i = 0; i < CNT; i++) {
		for (j = 0; j < 32; j++) {
			b[j] = b[j] ^ a[i * DT + x_t[i]][j];
		}
		//printf("%d, ", x_t[i]);
	}
	//printf("\n\n");
}

//����������Կ����Constant(msk, f)
void PRF_Const(unsigned char Pmsk[][32], int f_psi[][dt], bool f_phi[][dt],
	unsigned char sk_f[][32]) {
	int i, j, k, r, num, count;
	bool mark[CNT] = { 0 };//�������Ƿ���f�У����ڣ���Ϊ��1��
	int y[WT] = { 0 };//
	//Si
	int cd[N] = { 0 };
	for (i = 0; i < N; i++) {
		num = loc(f_psi[i]);
		mark[num] = 1;
		cd[i] = num;
		//printf("num = %d, ", num);
	}
	//S^0 = S_1+....+S_N
	int mid[4] = { 0 };
	mid[1] = WT >> 2; mid[2] = WT >> 1; mid[3] = mid[1] + mid[2];
	//S_1
	for (i = 0; i < WT; i++) {
		memcpy(sk_f[i], Pmsk[cd[0] * DT + 2], 32);
	}
	for (i = 0; i < 32; i++) {
		//�ȼ�ѡ����
		for (j = 0; j < mid[2]; j++) {
			sk_f[j][i] = sk_f[j][i] ^ Pmsk[cd[1] * DT][i];
			sk_f[j + mid[2]][i] = sk_f[j + mid[2]][i] ^ Pmsk[cd[1] * DT + 1][i];
		}
		//��������(ȫ0��ȫ1)
		for (j = 0; j < WT; j++) {
			count = 0;
			for (k = N - 1; k > 1; k--) {
				sk_f[j][i] = sk_f[j][i] ^ Pmsk[cd[k] * DT + (j >> count) & (DT - 1)][i];
				count++;
			}
		}
	}
	//S_2
	for (i = 0; i < WT; i++) {
		y[i] = i + WT;
		memcpy(sk_f[y[i]], Pmsk[cd[1] * DT + 3], 32);
	}
	for (i = 0; i < 32; i++) {
		//�ȼ�ѡ����
		for (j = 0; j < mid[1]; j++) {
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[0] * DT + 1][i];
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[2] * DT + 2][i];
		}
		for (j = mid[1]; j < mid[2]; j++) {
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[0] * DT + 1][i];
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[2] * DT + 3][i];
		}
		for (j = mid[2]; j < mid[3]; j++) {
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[0] * DT + 3][i];
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[2] * DT + 2][i];
		}
		for (j = mid[3]; j < WT; j++) {
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[0] * DT + 3][i];
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[2] * DT + 3][i];
		}
		//��������(ȫ0��ȫ1)
		for (j = 0; j < 4; j++) {
			for (k = 0; k < mid[2]; k++) {
				count = 0;
				for (r = N - 1; r > 2; r--) {
					sk_f[y[j] + mid[j]][i] = sk_f[y[j] + mid[j]][i] ^
						Pmsk[cd[r] * DT + (k >> count) & 3][i];
					count++;
				}
			}
		}
	}
	//S_3
	for (i = 0; i < WT; i++) {
		y[i] = y[i] + WT;
		memcpy(sk_f[y[i]], Pmsk[cd[2] * DT + 3], 32);
	}
	for (i = 0; i < 32; i++) {
		//�ȼ�ѡ����
		for (j = 0; j < mid[1]; j++) {
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[1] * DT + 1][i];
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[3] * DT + 2][i];
		}
		for (j = mid[1]; j < mid[2]; j++) {
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[1] * DT + 3][i];
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[3] * DT + 2][i];
		}
		for (j = mid[2]; j < mid[3]; j++) {
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[1] * DT + 1][i];
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[3] * DT + 3][i];
		}
		for (j = mid[3]; j < WT; j++) {
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[1] * DT + 3][i];
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[3] * DT + 3][i];
		}

		//��������(ȫ0��ȫ1)
		for (j = 0; j < 4; j++) {
			for (k = 0; k < mid[2]; k++) {
				count = 0;
				for (r = N - 1; r > 3; r--) {
					sk_f[y[j] + mid[j]][i] = sk_f[y[j] + mid[j]][i] ^
						Pmsk[cd[r] * DT + (k >> count) & 3][i];
					count++;
				}
				sk_f[y[j] + mid[j]][i] = sk_f[y[j] + mid[j]][i] ^
					Pmsk[cd[0] * DT + (k >> count) & 3][i];
				//count++;
			}
		}
	}
	//S_4
	for (i = 0; i < WT; i++) {
		y[i] = y[i] + WT;
		memcpy(sk_f[y[i]], Pmsk[cd[3] * DT + 3], 32);
	}
	for (i = 0; i < 32; i++) {
		//�ȼ�ѡ����
		for (j = 0; j < mid[1]; j++) {
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[2] * DT + 1][i];
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[4] * DT + 2][i];
		}
		for (j = mid[1]; j < mid[2]; j++) {
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[2] * DT + 3][i];
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[4] * DT + 2][i];
		}
		for (j = mid[2]; j < mid[3]; j++) {
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[2] * DT + 1][i];
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[4] * DT + 3][i];
		}
		for (j = mid[3]; j < WT; j++) {
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[2] * DT + 3][i];
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[4] * DT + 3][i];
		}
		//��������(ȫ0��ȫ1)
		for (j = 0; j < 4; j++) {
			for (k = 0; k < mid[2]; k++) {
				count = 0;
				for (r = N - 1; r > 3; r--) {
					sk_f[y[j] + mid[j]][i] = sk_f[y[j] + mid[j]][i] ^
						Pmsk[cd[r] * DT + (k >> count) & 3][i];
					count++;
				}
				for (r = 1; r >= 0; r--) {
					sk_f[y[j] + mid[j]][i] = sk_f[y[j] + mid[j]][i] ^
						Pmsk[cd[r] * DT + (k >> count) & 3][i];
					count++;
				}
			}
		}
	}
	//S_5
	for (i = 0; i < WT; i++) {
		y[i] = y[i] + WT;
		memcpy(sk_f[y[i]], Pmsk[cd[4] * DT + 3], 32);
	}
	for (i = 0; i < 32; i++) {
		//�ȼ�ѡ����
		for (j = 0; j < mid[1]; j++) {
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[3] * DT + 1][i];
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[5] * DT + 2][i];
		}
		for (j = mid[1]; j < mid[2]; j++) {
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[3] * DT + 3][i];
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[5] * DT + 2][i];
		}
		for (j = mid[2]; j < mid[3]; j++) {
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[3] * DT + 1][i];
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[5] * DT + 3][i];
		}
		for (j = mid[3]; j < WT; j++) {
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[3] * DT + 3][i];
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[5] * DT + 3][i];
		}
		//��������(ȫ0��ȫ1)
		for (j = 0; j < 4; j++) {
			for (k = 0; k < mid[2]; k++) {
				count = 0;
				for (r = 2; r >= 0; r--) {
					sk_f[y[j] + mid[j]][i] = sk_f[y[j] + mid[j]][i] ^
						Pmsk[cd[r] * DT + (k >> count) & 3][i];
					count++;
				}
			}
		}
	}
	//S_6
	for (i = 0; i < WT; i++) {
		y[i] = y[i] + WT;
		memcpy(sk_f[y[i]], Pmsk[cd[5] * DT + 3], 32);
	}
	for (i = 0; i < 32; i++) {
		//�ȼ�ѡ����
		for (j = 0; j < mid[2]; j++) {
			sk_f[y[j]][i] = sk_f[y[j]][i] ^ Pmsk[cd[4] * DT + 1][i];
			sk_f[y[j] + mid[2]][i] = sk_f[y[j] + mid[2]][i] ^ Pmsk[cd[4] * DT + 3][i];
		}
		//��������(ȫ0��ȫ1)
		for (j = 0; j < WT; j++) {
			count = 0;
			for (k = N - 3; k >= 0; k--) {
				sk_f[j][i] = sk_f[j][i] ^ Pmsk[cd[k] * DT + (j >> count) & (DT - 1)][i];
				count++;
			}
		}
	}

	count = N * WT;
	//S^1
	for (i = 0; i < CNT; i++) {
		if (mark[i] == 0) {
			for (j = 0; j < DT; j++) {
				{
					memcpy(sk_f[count], Pmsk[i * DT + j], 32);
					count++;
				}
			}
		}
	}
}

//������Կģ�⺯��KeySim(,f)
void PRF_KeySim(int f_psi[][dt], bool f_phi[][dt],
	unsigned char sk_f[][32]) {
	unsigned char SPmsk[CDT][32] = { 0 };
	PRF_Setup(SPmsk);
	PRF_Const(SPmsk, f_psi, f_phi, sk_f);
}

//����ƥ�亯��CEval(f, x)
void PRF_CEval(int pos, int f_psi[][dt], bool f_phi[][dt], bool* x, 
	unsigned char sk_f[][32], unsigned char* b) {
	int i, k;
	int num, count;
	unsigned char x_t[CNT] = { 0 }; //�洢CNT���Ӵ�
	bool data[dt] = { 0 };
	bool mark[CNT] = { 0 };//�������Ƿ���f�У����ڣ���Ϊ��1��
	count = 0;
	Xext(x, data, 0, n - 1, 0, x_t, &count);

	//����ȷ��S_{i,j}�е�iΪpos,jΪ7
	memcpy(b, sk_f[pos * WT + 7], 32);
	for (i = 0; i < N; i++) {
		num = loc(f_psi[i]);
		mark[num] = 1;
	}
	count = N * WT;
	for (i = 0; i < CNT; i++) {
		if (mark[i] == 0) {
			for (k = 0; k < 32; k++) {
				b[k] = b[k] ^ sk_f[count + x_t[i]][k];
			}
			count = count + DT;
		}
	}
		
	

}