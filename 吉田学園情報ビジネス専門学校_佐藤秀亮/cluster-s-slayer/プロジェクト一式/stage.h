//========================================
// ステージの配置処理
// Author:林
//========================================
#ifndef _STAGE_H_
#define _STAGE_H_
#include "main.h"
#define STAGEPRESET_MAX (5)

class CStage_Preset;

class CStage
{
public:
	//------------------------------------
	// コンストラクタ、デストラクタ
	//------------------------------------
	CStage();
	~CStage();
	HRESULT Init();
	void Uninit();
	void SetStage();//ステージの設定
	void PresetCreate(const char *cFileName);
private:
	CStage_Preset *m_pStagePreset[STAGEPRESET_MAX];
};
#endif _STAGE_H_