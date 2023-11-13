#include "ABE_SETUP.h"
/// <summary>
/// 初始化算法输出主公钥和主私钥
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
	//return;		// 返回公钥 MAPK 和私钥 MSK
}
