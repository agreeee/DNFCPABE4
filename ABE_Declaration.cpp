#include "ABE_Declaration.h"
#include "SM3.h"

namespace Zr_Random_Buff {
	bool IsInited = false;
	Zr* BUF = 0;
}

void InitZrRandomBuff(int All) {
	if (!Zr_Random_Buff::IsInited) {
		printf("called\n");
		Zr_Random_Buff::BUF = new Zr[All];
		for (int i = 0; i < All; i++) {
			Zr_Random_Buff::BUF[i].rand();
		}
		Zr_Random_Buff::IsInited = true;
	}
	return;
}

void ClearZrRandomBuff() {
	if (Zr_Random_Buff::IsInited) {
		Zr_Random_Buff::IsInited = false;
		delete[] Zr_Random_Buff::BUF;
		Zr_Random_Buff::BUF = 0;
	}
	return;
}


///////////////////////////////////////////////////////////////////////////
MAPK::MAPK() {
	this->h = new G[2 * n];
}
MAPK::~MAPK() {
	/*
	if(this->h) {
		delete[]this->h;
		this->h = 0;
	}
	*/
}



///////////////////////////////////////////////////////////////////////////
USK::USK() {
	this->sk = new G[n + 1];
}
USK::~USK() {
	/*
	if(this->SK) {
		delete[] this->SK;
		this->SK = 0;
	}*/
}
///////////////////////////////////////////////////////////////////////////
CIP::CIP() {
	this->c = new G[N+1];
}
CIP::~CIP() {
	/*if(this->c) {
		delete[] this->c;
		this->c = 0;
	}
	*/
}



///////////////////////////////////////////////////////////////////////////

U8STR::U8STR() {
	this->pt = 0;
}
U8STR::U8STR(const int len) {
	this->pt = new U8[len];
}
U8STR::~U8STR() {
	if (this->pt) {
		delete[] this->pt;
		this->pt = 0;
	}
}

U8 U8STR::operator [] (const int index)const {
	return this->pt[index];
}


