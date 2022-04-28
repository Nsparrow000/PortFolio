//=============================================================================
// エフェクト情報をロードさせる [Load.h]
// Author : 佐藤秀亮
//=============================================================================
#ifndef _LOADEFFECT_H_
#define _LOADEFFECT_H_

#include "main.h"

class CLoadEffect
{
public:
	CLoadEffect();
	~CLoadEffect();

	static void EffectStateLoad(const char *aFileName);	//エフェクトのステータス

	//static void PresetTotal(const char *aFileName);
	static int GetPresetTotal() { return m_Total; }

private:
	static int m_Total;
};

#endif // !_LOADEFFECT_H_