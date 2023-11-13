#include "ABE_ENCRPT.h"

void Enc(G& P, MAPK& MPK, GT& M, int f_psi[][dt], 
	bool f_phi[][dt], CIP& C) {

	int i, j, ij;
	Zr rr;
	rr.rand();

	//º∆À„ C_0=M*e(g,g)^(s*r)
	C.c0.pow(MPK.y, rr);
	C.c0.mul(C.c0, M);

	C.c[N].DotPro(rr, P);
	for (i = 0; i < N; i++) {
		C.c[i].set1();
		for (j = 0; j < dt; j++) {
			ij = i * dt + j;
			C.c[i].add(C.c[i], MPK.h[f_phi[i][j] * n + f_psi[i][j]]);
		}
	}

	return;
}


