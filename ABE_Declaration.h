#ifndef __ABE_DECLARATION_H__
#define __ABE_DECLARATION_H__
/*
* �ڴ��ļ��ж��壺
*	1��ȫ�ֶ���
*	2��ȫ��ͨ�÷���
*	3��ͨ��ͷ�ļ�
*	4���������ͨ����������
*/

#define DEBUG

#include "pbc-class.h"
#include "param.h"
#ifdef DEBUG
#include <stdio.h>
#endif // DEBUG

typedef unsigned char	U8;
typedef unsigned int	U32;
/// <summary>
/// ��ʼ��������ÿ�����Զ�Ӧ�������
/// </summary>
/// ϵͳ���ܹ������Ը���
/// </param>
void InitZrRandomBuff(int All);

/// <summary>
/// �ͷų�ʼ��ʱռ�õ��ڴ�ռ�
/// </summary>
void ClearZrRandomBuff();


/// <summary>
/// ������Կ
/// </summary>
class MAPK {
public:
	G* h;//G1 h[2*n]
	GT y;//GT y=e(g,g)^s
	MAPK();
	~MAPK();
};


/// <summary>
/// �û�˽Կ
/// </summary>
class USK {
private:
public:
	G* sk;
	USK();
	~USK();
};


/// <summary>
/// ����
/// </summary>
class CIP {
public:
	G* c;
	GT c0;
	CIP();
	~CIP();
};


/// <summary>
/// �ֽڴ�
/// </summary>
class U8STR {
public:
	U8* pt;
	U8 operator [] (int index)const;
	U8STR();
	U8STR(const int len);
	~U8STR();
};

#endif // !__ABE_DECLARATION_H__
