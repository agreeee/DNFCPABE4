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
	pbc_init(argc, argv);	// 初始化环境
	InitZrRandomBuff(900);
	clock_t begin[4] = { 0 }, end[4] = { 0 };
	G P; // 定义 G 群变量 P
	P.set1();		// 随机生成 G 群中的生成元 P

	printf("\n**********系统建立***********\n");
	begin[0] = clock();
	MAPK MPK;		//主公钥
	G gs;		//主私钥g^s
	Setup(P, MPK, gs);	//给系统生成主私钥和公钥
	//构造密钥列表T
	unsigned char Pmsk[CDT][32] = { 0 };
	PRF_Setup(Pmsk);
	end[0] = clock() - begin[0];


	printf("\n**********私钥生成算法***********\n");
	begin[1] = clock();
	bool x[n] = { 0 };     //用户属性并初始化
	x[0] = 0; x[1] = 1;
	x[2] = 1; x[3] = 0;
	x[4] = 0; x[5] = 1;
	x[6] = 1; x[7] = 0;
	USK SK;     //用户私钥
	KeyGen(P, MPK, gs, x, SK);
	unsigned char Ptx[32] = { 0 };//存储x的评估值
	PRF_Eval(Pmsk, x, Ptx);
	end[1] = clock() - begin[1];

	//加密算法
	printf("\n********加密算法*********\n");
	begin[2] = clock();
	GT M;		//随机明文
	M.rand(); 
	M.print("M = ");
	//访问结构
	int f_psi[N][dt] = { 0 };
	bool f_phi[N][dt] = { 0 };
	//取非？
	f_phi[0][0] = 1; f_phi[0][1] = 0; f_phi[1][0] = 1;
	f_phi[1][1] = 1; f_phi[2][0] = 1; f_phi[2][1] = 1;
	f_phi[3][0] = 1; f_phi[3][1] = 1; f_phi[4][0] = 1;
	f_phi[4][1] = 1; f_phi[5][0] = 1; f_phi[5][1] = 1;
	//属性序号？
	f_psi[0][0] = 0; f_psi[0][1] = 1; f_psi[1][0] = 1;
	f_psi[1][1] = 2; f_psi[2][0] = 2; f_psi[2][1] = 3;
	f_psi[3][0] = 3; f_psi[3][1] = 4; f_psi[4][0] = 4;
	f_psi[4][1] = 5; f_psi[5][0] = 5; f_psi[5][1] = 6;
	CIP C;         //定义密文
	Enc(P, MPK, M, f_psi, f_phi, C);
	unsigned char sk_f[L][32] = {0};//存储受限密钥
	PRF_KeySim(f_psi, f_phi, sk_f);
	end[2] = clock() - begin[2];
	
	

	printf("\n**********解密算法***********\n");
	begin[3] = clock();
	//首先判定f(x)=0是否成立？输出错误符或者评估值b
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
		printf("\nf(x)=1: pos = %d, 进行下一步\n", pos);
		//Eval正确性验证
		unsigned char PPtx[32] = { 0 };//存储x的评估值
		PRF_CEval(pos, f_psi, f_phi, x, sk_f, PPtx);
		if (memcmp(Ptx, PPtx, 32) == 0) {
			printf("\n错误：评估值相等！\n");
		}
		else {
			printf("\n评估值不相等！进行下一步\n");
			GT  DM;
			Dec(SK, C, f_psi, f_phi, x, DM);
			DM.print("DM = ");
		}
	}
	end[3] = clock() - begin[3];
	printf("系统建立时长：%fms\n", (double)end[0] * 1000 / CLOCKS_PER_SEC);
	printf("私钥生成时长：%fms\n", (double)end[1] * 1000 / CLOCKS_PER_SEC);
	printf("加密时长：%fms\n", (double)end[2] * 1000 / CLOCKS_PER_SEC);
	printf("解密时长：%fms\n", (double)end[3] * 1000 / CLOCKS_PER_SEC);


	pbc_clear();			// 释放内存
	ClearZrRandomBuff();	 // 释放内存
	return 0;
}

