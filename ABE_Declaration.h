#ifndef __ABE_DECLARATION_H__
#define __ABE_DECLARATION_H__
/*
* 在此文件中定义：
*	1、全局定义
*	2、全局通用方法
*	3、通用头文件
*	4、输入输出通用数据类型
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
/// 初始化，生成每个属性对应的随机数
/// </summary>
/// 系统中总共的属性个数
/// </param>
void InitZrRandomBuff(int All);

/// <summary>
/// 释放初始化时占用的内存空间
/// </summary>
void ClearZrRandomBuff();


/// <summary>
/// 属性密钥
/// </summary>
class MAPK {
public:
	G* h;//G1 h[2*n]
	GT y;//GT y=e(g,g)^s
	MAPK();
	~MAPK();
};


/// <summary>
/// 用户私钥
/// </summary>
class USK {
private:
public:
	G* sk;
	USK();
	~USK();
};


/// <summary>
/// 密文
/// </summary>
class CIP {
public:
	G* c;
	GT c0;
	CIP();
	~CIP();
};


/// <summary>
/// 字节串
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
