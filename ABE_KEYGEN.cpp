#include "ABE_KEYGEN.h"

void KeyGen(G& P, MAPK& MPK, G& gs, bool* x, USK& SK) {
	int i;

	//randomly select v
	Zr v;
	v.rand();
	//compute sk_0 = g^v
	SK.sk[n].DotPro(v, P);
	for (i = 0; i < n; i++) {
		SK.sk[i].DotPro(v, MPK.h[x[i] * n + i]);
		SK.sk[i].add(SK.sk[i], gs);
		//SK.sk[i].print();
	}

}