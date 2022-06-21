//========================================
// ステージの塊を作成する
// Author:林
//========================================
#include "stage_preset.h"
#include "model.h"
#include "wall.h"
#include "field.h"
#include "texture.h"
#include "model_spawner.h"
#include "stage_preset_data.h"
#include "enemy_spawn_manager.h"
//------------------------------------
// コンストラクタ
//------------------------------------
CStage_Preset::CStage_Preset()
{
}

//------------------------------------
// デストラクタ
//------------------------------------
CStage_Preset::~CStage_Preset()
{

}
//------------------------------------
//初期化
//------------------------------------
HRESULT CStage_Preset::Init()
{

	return S_OK;
}
//------------------------------------
//終了
//------------------------------------
void CStage_Preset::Uninit()
{
}
//------------------------------------
//更新
//------------------------------------
void CStage_Preset::Update()
{
}
//------------------------------------
//描画
//------------------------------------
void CStage_Preset::Draw()
{
}
//------------------------------------
//生成
//------------------------------------
CStage_Preset * CStage_Preset::Create(const D3DXVECTOR3& pos,const int& nType)
{
	CStage_Preset *pStagePreset = new CStage_Preset;
	if (pStagePreset)
	{
		pStagePreset->Init();
		pStagePreset->m_pos = pos;
		pStagePreset->PresetCreate(nType);
	}
	return pStagePreset;
}
//------------------------------------
//ステージプリセットの生成
//------------------------------------
void CStage_Preset::PresetCreate(const int& nType)
{
	//プリセット情報の取得
	CStage_Preset_Data::PRESETDATA PresetData = CStage_Preset_Data::GetStagePresetData(nType);
	{
		int nWallMax = PresetData.WallData.size();
		for (int nNumWall = 0; nNumWall < nWallMax; nNumWall++)
		{
			//壁の情報
			CStage_Preset_Data::WALLDATA& WallData = PresetData.WallData[nNumWall];
			CWall::Create(m_pos + WallData.pos, WallData.size, WallData.rot, WallData.block.x, WallData.block.y, CTexture::FIELD);
		}
	}
	{
		int nFieldMax = PresetData.FieldData.size();
		for (int nNumField = 0; nNumField < nFieldMax; nNumField++)
		{
			//床の情報
			CStage_Preset_Data::FIELDDATA& FieldData = PresetData.FieldData[nNumField];
			CField::Create(m_pos + FieldData.pos, FieldData.size, FieldData.rot, FieldData.block.x, FieldData.block.y, CTexture::FIELD);
		}
	}
	{
		int nModelMax = PresetData.ModelData.size();
		for (int nNumModel = 0; nNumModel < nModelMax; nNumModel++)
		{
			//モデルの情報
			CStage_Preset_Data::MODELDATA& ModelData = PresetData.ModelData[nNumModel];
			CModel_Spawner::Create(m_pos + ModelData.pos,ModelData.rot, ModelData.nType);
		}
	}
	CEnemySpawnManager::Create(m_pos);
}
