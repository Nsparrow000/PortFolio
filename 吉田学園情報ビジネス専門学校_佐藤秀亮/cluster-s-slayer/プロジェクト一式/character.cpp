//=============================================-
//キャラクター共通の処理
//=============================================
#include "character.h"
#include "model.h"
#include "manager.h"
#include "Renderer.h"
#include "shadow.h"
#include "model_spawner.h"
static const float fDrawLimit = 2000.0f;
//---------------------------------------------
//コンストラクタ
//---------------------------------------------
CCharacter::CCharacter(OBJTYPE nPriority) : CScene(nPriority)
{
	m_pParts.clear();
	m_IsCharacterDraw = true;
	m_nPartsMax = 0;
	m_pMotion = nullptr;
}
//-----------------------------------------------
//デストラクタ
//---------------------------------------------
CCharacter::~CCharacter()
{

}
//-----------------------------------------------
//初期化
//---------------------------------------------
HRESULT CCharacter::Init()
{
	return S_OK;
}
//-----------------------------------------------
//終了
//---------------------------------------------
void CCharacter::Uninit()
{
	{
		int nPartsSize = m_pParts.size();
		for (int nCntParts = 0; nCntParts < nPartsSize; nCntParts++)
		{
			if (m_pParts[nCntParts] != nullptr)
			{
				m_pParts[nCntParts]->Uninit();
				m_pParts[nCntParts] = nullptr;
			}
		}
	}

	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}
	Release();
}

//-----------------------------------------------
//更新
//---------------------------------------------
void CCharacter::Update()
{
}
//-----------------------------------------------
//描画
//---------------------------------------------
void CCharacter::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ

	D3DXMATRIX mtxRotModel, mtxTransModel;//計算用マトリックス

	//各パーツのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを設定
	D3DXMatrixRotationYawPitchRoll(&mtxRotModel, m_rot.y, m_rot.x, m_rot.z);

	//向きを反映
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRotModel);

	//位置を設定
	D3DXMatrixTranslation(&mtxTransModel, m_pos.x, m_pos.y, m_pos.z);

	//位置を反映
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTransModel);

	//各パーツのワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//モデル生成
	if (m_IsCharacterDraw)
	{
		int nMaxParts = 0;//パーツの最大数
		nMaxParts = m_pParts.size();
		//キャラクターのパーツ数が０以外なら
		if (nMaxParts != 0)
		{
			//パーツ数を代入する
			nMaxParts = m_pParts.size();
		}
		for (int nCntParts = 0; nCntParts < nMaxParts; nCntParts++)
		{
			m_pParts[nCntParts]->Draw();
		}
	}

	//前回の位置情報の保存
	m_lastpos = m_pos;

}

//-----------------------------------------------
//当たり判定(相手の現在の位置、相手の一つ前の位置、相手の当たり判定の大きさ)
//---------------------------------------------
bool CCharacter::IsCollision(D3DXVECTOR3 *pMyPos, const D3DXVECTOR3& Hitpos, const float& fRadius, const float& MoveSpeed)
{
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//自分と相手のベクトル
	vec = *pMyPos - Hitpos;
	float fLength = 0.0f;
	//相手と自分の距離を求める
	fLength = sqrtf((vec.z*vec.z) + (vec.x*vec.x));
	float fCollisionRadius = m_fRadius + fRadius;
	//相手と自分の距離が自分の当たり判定の大きさより小さくなったら
	if (fLength <= fCollisionRadius)
	{
		float fAng = atan2(vec.x, vec.z);
		//押し戻す
		D3DXVECTOR3 returnpos = {sinf(fAng)*MoveSpeed,0.0f,cosf(fAng)*MoveSpeed};
		pMyPos->x += returnpos.x;
		pMyPos->z += returnpos.z;
		return true;
	}
	return false;
}
void CCharacter::IsModelCollision(const bool& bPlayer)
{
	CScene *pSceneModel = CScene::GetScene(OBJTYPE_MODELSPAWNER);

	while (pSceneModel)
	{
		CModel_Spawner *pModel = (CModel_Spawner*)pSceneModel;
		D3DXVECTOR3 ModelPos = pModel->GetPos();
		D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//自分と相手のベクトル
		vec = m_pos - ModelPos;
		float fRadius = pModel->GetModel()->GetMaxPos().x;
		float fLength = 0.0f;
		//相手と自分の距離を求める
		fLength = sqrtf((vec.z*vec.z) + (vec.x*vec.x));
		float fCollisionRadius = m_fRadius + fRadius;
		if (bPlayer)
		{
			if (fLength >= fDrawLimit)
			{
				pModel->SetDrawLimit(false);
			}
			else
			{
				pModel->SetDrawLimit(true);
			}
		}
		//相手と自分の距離が自分の当たり判定の大きさより小さくなったら
		if (fLength <= fCollisionRadius && pModel->GetDrawLimit())
		{
			float fAng = atan2(vec.x, vec.z);
			//押し戻す
			D3DXVECTOR3 returnpos = { 0.0f,0.0f,0.0f };
			returnpos.x = ModelPos.x + sinf(fAng) * fCollisionRadius;
			returnpos.z = ModelPos.z + cosf(fAng) * fCollisionRadius;

			m_pos.x = returnpos.x;
			m_pos.z = returnpos.z;
		}
		pSceneModel = pSceneModel->GetSceneNext(pSceneModel);

	}
}
//-----------------------------------------------
//HPの増減
//---------------------------------------------
void CCharacter::HPChange(float fNum)
{
	m_fHitPoint += fNum;
}
//-----------------------------------------------
//状態の変更
//---------------------------------------------
void CCharacter::StateChange(state_type StateType)
{
	m_StateType = StateType;
}
//-----------------------------------------------
//キャラクターのパーツ取得処理
//---------------------------------------------
CModel * CCharacter::GetParts(int nPartsNum)
{
	int nSize = m_pParts.size();
	if (nSize != 0)
	{
		return  m_pParts[nPartsNum];
	}
	return nullptr;
}
