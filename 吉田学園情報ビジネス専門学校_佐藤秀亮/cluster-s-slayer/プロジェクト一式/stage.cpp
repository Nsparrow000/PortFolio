//========================================
// ステージ
// Author:林
//========================================
#include "stage.h"
#include "model.h"
#include "wall.h"
#include "field.h"
#include "texture.h"
#include "model_spawner.h"
#include "stage_preset_data.h"
#include "stage_preset.h"
static const float IntervalPos = 1650.0f;
//------------------------------------
// コンストラクタ
//------------------------------------
CStage::CStage()
{
	memset(m_pStagePreset, NULL, sizeof(m_pStagePreset));
}

//------------------------------------
// デストラクタ
//------------------------------------
CStage::~CStage()
{

}

HRESULT CStage::Init()
{


	return S_OK;
}

void CStage::SetStage()
{
	for (int nBlockY = 0; nBlockY < STAGEPRESET_MAX; nBlockY++)
	{
		for (int nBlockX = 0; nBlockX < STAGEPRESET_MAX; nBlockX++)
		{
			int nHalf = ((STAGEPRESET_MAX - 1) / 2);
			D3DXVECTOR3 pos = {(nHalf*IntervalPos) - (IntervalPos * nBlockX),0.0f,
				(nHalf*IntervalPos) - (IntervalPos * nBlockY) };
			std::random_device random;	// 非決定的な乱数生成器
			std::mt19937_64 mt(random());            // メルセンヌ・ツイスタの64ビット版、引数は初期シード
			std::uniform_real_distribution<> randType(0, STAGEPRESET_MAX);

			CStage_Preset::Create(pos, randType(mt));
		}
	}
}



