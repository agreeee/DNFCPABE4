#include "ABE_SETUP.h"
/// <summary>
/// ��ʼ���㷨�������Կ����˽Կ
/// </summary>

void Setup(G& P, MAPK& MPK, G& gs) {
	int i;
	Zr s;
	s.rand();
	for (i = 0; i < 2 * n; i++) {
		MPK.h[i].rand();
	}
	gs.DotPro(s, P);
	MPK.y.E(P, gs);
	//return;		// ���ع�Կ MAPK ��˽Կ MSK
}
