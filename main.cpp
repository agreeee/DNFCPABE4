#include <stdio.h>
#include <time.h>
#include <math.h>
#include "pbc-class.h"
#include "ABE_Declaration.h"
#include "param.h"
#include "ABE_SETUP.h"
#include "ABE_KEYGEN.h"
#include "ABE_ENCRPT.h"
#include "ABE_DECRPT.h"
#include "ABE_CPRF.h"

unsigned char buf[100] = { 0 }, * pb = buf;

int main(int argc, char* argv[]) {
	pbc_init(argc, argv);	// ��ʼ������
	InitZrRandomBuff(900);
	clock_t begin[4] = { 0 }, end[4] = { 0 };
	G P; // ���� G Ⱥ���� P
	P.set1();		// ������� G Ⱥ�е�����Ԫ P

	printf("\n**********ϵͳ����***********\n");
	begin[0] = clock();
	MAPK MPK;		//����Կ
	G gs;		//��˽Կg^s
	Setup(P, MPK, gs);	//��ϵͳ������˽Կ�͹�Կ
	//������Կ�б�T
	unsigned char Pmsk[CDT][32] = { 0 };
	PRF_Setup(Pmsk);
	end[0] = clock() - begin[0];


	printf("\n**********˽Կ�����㷨***********\n");
	begin[1] = clock();
	bool x[n] = { 0 };     //�û����Բ���ʼ��
	x[0] = 0; x[1] = 1;
	x[2] = 1; x[3] = 0;
	x[4] = 0; x[5] = 1;
	x[6] = 1; x[7] = 0;
	USK SK;     //�û�˽Կ
	KeyGen(P, MPK, gs, x, SK);
	unsigned char Ptx[32] = { 0 };//�洢x������ֵ
	PRF_Eval(Pmsk, x, Ptx);
	end[1] = clock() - begin[1];

	//�����㷨
	printf("\n********�����㷨*********\n");
	begin[2] = clock();
	GT M;		//�������
	M.rand(); 
	M.print("M = ");
	//���ʽṹ
	int f_psi[N][dt] = { 0 };
	bool f_phi[N][dt] = { 0 };
	//ȡ�ǣ�
	f_phi[0][0] = 1; f_phi[0][1] = 0; f_phi[1][0] = 1;
	f_phi[1][1] = 1; f_phi[2][0] = 1; f_phi[2][1] = 1;
	f_phi[3][0] = 1; f_phi[3][1] = 1; f_phi[4][0] = 1;
	f_phi[4][1] = 1; f_phi[5][0] = 1; f_phi[5][1] = 1;
	//������ţ�
	f_psi[0][0] = 0; f_psi[0][1] = 1; f_psi[1][0] = 1;
	f_psi[1][1] = 2; f_psi[2][0] = 2; f_psi[2][1] = 3;
	f_psi[3][0] = 3; f_psi[3][1] = 4; f_psi[4][0] = 4;
	f_psi[4][1] = 5; f_psi[5][0] = 5; f_psi[5][1] = 6;
	CIP C;         //��������
	Enc(P, MPK, M, f_psi, f_phi, C);
	unsigned char sk_f[L][32] = {0};//�洢������Կ
	PRF_KeySim(f_psi, f_phi, sk_f);
	end[2] = clock() - begin[2];
	
	

	printf("\n**********�����㷨***********\n");
	begin[3] = clock();
	//�����ж�f(x)=0�Ƿ����������������������ֵb
	int count, pos = -1;
	for (int i = 0; i < N; i++) {
		count = 0;
		for (int j = 0; j < dt; j++) {
			if (f_phi[i][j] == x[f_psi[i][j]]) { count++; }
		}
		if (count == dt) {
			pos = i;
			break;
		}
	}
	if (pos == -1) {
		printf("\n***f(x)=0***\n");
	}
	else {
		printf("\nf(x)=1: pos = %d, ������һ��\n", pos);
		//Eval��ȷ����֤
		unsigned char PPtx[32] = { 0 };//�洢x������ֵ
		PRF_CEval(pos, f_psi, f_phi, x, sk_f, PPtx);
		if (memcmp(Ptx, PPtx, 32) == 0) {
			printf("\n��������ֵ��ȣ�\n");
		}
		else {
			printf("\n����ֵ����ȣ�������һ��\n");
			GT  DM;
			Dec(SK, C, f_psi, f_phi, x, DM);
			DM.print("DM = ");
		}
	}
	end[3] = clock() - begin[3];
	printf("ϵͳ����ʱ����%fms\n", (double)end[0] * 1000 / CLOCKS_PER_SEC);
	printf("˽Կ����ʱ����%fms\n", (double)end[1] * 1000 / CLOCKS_PER_SEC);
	printf("����ʱ����%fms\n", (double)end[2] * 1000 / CLOCKS_PER_SEC);
	printf("����ʱ����%fms\n", (double)end[3] * 1000 / CLOCKS_PER_SEC);


	pbc_clear();			// �ͷ��ڴ�
	ClearZrRandomBuff();	 // �ͷ��ڴ�
	return 0;
}

