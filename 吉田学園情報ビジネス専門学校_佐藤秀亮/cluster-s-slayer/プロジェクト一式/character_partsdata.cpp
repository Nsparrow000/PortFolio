//--------------------------------------------
//キャラクターパーツデータの保持クラス
//作成者：林
//--------------------------------------------
#include "character_partsdata.h"
#include "layer_structure.h"
#include "model.h"
CCharacterPartsData::PartsData CCharacterPartsData::m_EnemyPartsData = {};

//--------------------------------------------
//コンストラクタ
//--------------------------------------------
CCharacterPartsData::CCharacterPartsData()
{
	
}
//--------------------------------------------
//デストラクタ
//--------------------------------------------
CCharacterPartsData::~CCharacterPartsData()
{
}
//--------------------------------------------
//初期化
//--------------------------------------------
HRESULT CCharacterPartsData::Init(void)
{
	//ファイル読み込み
	CLayer_Structure *pLayer = new CLayer_Structure;
	pLayer->SetLayer_Structure("data/TEXT/EnemyParts000.txt", &m_EnemyPartsData, CModel::TYPE_ENEMY);

	return S_OK;
}
//--------------------------------------------
//終了
//--------------------------------------------
void CCharacterPartsData::Uninit(void)
{

}
//----------------------------------------------
//インスタンス生成
//----------------------------------------------
CCharacterPartsData *CCharacterPartsData::Create()
{
	CCharacterPartsData *pCharacterPartsData = NULL;
	pCharacterPartsData = new CCharacterPartsData;
	pCharacterPartsData->Init();
	return pCharacterPartsData;
}
//----------------------------------------------
//パーツの設定
//----------------------------------------------
void CCharacterPartsData::SetParts(std::vector<CModel*> *Parts)
{
	for (int nIndex = 0; nIndex < m_EnemyPartsData.nMaxModelNum; nIndex++)
	{
		Parts->push_back(CModel::Create(m_EnemyPartsData.pModelData[nIndex]->pos, m_EnemyPartsData.pModelData[nIndex]->rot, nIndex, m_EnemyPartsData.nLoadModelType));

		//親子関係を付ける
		if (m_EnemyPartsData.pModelData[nIndex]->nParent != -1)
		{
			
			(*Parts)[nIndex]->SetParent((*Parts)[m_EnemyPartsData.pModelData[nIndex]->nParent]);
			(*Parts)[nIndex]->SetNumParent(m_EnemyPartsData.pModelData[nIndex]->nParent);
		}
		else
		{
			(*Parts)[nIndex]->SetParent(NULL);
			(*Parts)[nIndex]->SetNumParent(-1);
		}
	}
}
