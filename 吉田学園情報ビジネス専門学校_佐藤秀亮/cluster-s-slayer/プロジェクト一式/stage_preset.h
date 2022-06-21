//========================================
// ステージの塊を生成したものを持つクラス
// Author:林
//========================================
#ifndef _STAGE_PRESET_H_
#define _STAGE_PRESET_H_
#include "main.h"
class CStage_Preset_Data;
class CStage_Preset
{
public:

	CStage_Preset();
	~CStage_Preset();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static CStage_Preset*Create(const D3DXVECTOR3& pos,const int& nType);
	void PresetCreate(const int& nType);

private:
	D3DXVECTOR3 m_pos;
};
#endif _STAGE_PRESET_H_