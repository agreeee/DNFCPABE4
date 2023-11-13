#include "ABE_DECRPT.h"

void Dec(USK& SK, CIP& C, int f_psi[][dt],
	bool f_phi[][dt], bool* x, GT& DM) {
	int i, j, k;
	G mid;
	GT E0, E1;

	//f_2()=1,E0=e(C_2,SK_0)
	E0.E(C.c[1], SK.sk[n]);
	mid.copy(SK.sk[f_psi[1][0]]);
	for (i = 0; i < dt; i++) {
		mid.add(mid, SK.sk[f_psi[1][i]]);
	}
	E1.E(mid, C.c[N]);
	E1.inv(E1);
	DM.mul(C.c0, E0);
	DM.mul(DM, E1);

}

