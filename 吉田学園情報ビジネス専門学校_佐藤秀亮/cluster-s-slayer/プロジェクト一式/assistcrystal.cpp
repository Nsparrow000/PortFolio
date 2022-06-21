//-------------------------------------------
//ブラックホールの処理
//------------------------------------------
#include "assistcrystal.h"
#include "manager.h"
#include "Renderer.h"
#include "Particle.h"
#include "enemy.h"
#include "PresetSetEffect.h"
#include "assistcrystal_model.h"
#include "player.h"
#include "character.h"
#include "skill_leveldata.h"
static const float MoveSpeedMax = 4.0f;
static const float MoveSpeedMin = 2.0f;
static const float HoleSpeed = 1.5f;
static const float HoleArea = 200.0f;
static const int DeleteTime = 200;
static const int HoleExpansionTime = 60;
static const float FollowingLimit = 10.0f;//慣性の制限値
static const float PlayerDistance = 100.0f;//慣性の制限値

//--------------------------------------------
//コンストラクタ
//--------------------------------------------
CAssistCrystal::CAssistCrystal()
{
	m_CrystalModel = nullptr;

}

//--------------------------------------------
//デストラクタ
//--------------------------------------------
CAssistCrystal::~CAssistCrystal()
{

}

//--------------------------------------------
//初期化
//--------------------------------------------
HRESULT CAssistCrystal::Init(void)
{
	CAssistCrystal::State LevelState = CSkill_LevelData::GetStateAssistCrystal(m_nLevel);
	if (!m_CrystalModel)
	{
		m_CrystalModel = CAssistCrystal_Model::Create({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, LevelState.m_fMoveSpeed, LevelState.m_nPower, LevelState.m_nLaunchInterval);
	}
	return S_OK;
}
//--------------------------------------------
//終了
//--------------------------------------------
void CAssistCrystal::Uninit(void)
{
	if (m_CrystalModel)
	{
		m_CrystalModel->Uninit();
		m_CrystalModel = nullptr;
	}

}
//--------------------------------------------
//更新
//--------------------------------------------
void CAssistCrystal::Update(void)
{
	PlayerFollowing();
	m_rot.y = m_pPlayer->GetRot().y;
	if (m_CrystalModel)
	{
		m_CrystalModel->Update();
	}
	if (m_bUninit)
	{
		Uninit();
	}
}

//--------------------------------------------
//描画
//--------------------------------------------
void CAssistCrystal::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ

	D3DXMATRIX mtxRotModel, mtxTransModel, mtxScaleModel;//計算用マトリックス

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

	//if (m_pPlayer)
	//{
	//	m_mtxParent = m_pPlayer->GetMtx();
	//}

	////算出した各パーツのワールドマトリックスと親のマトリックスを掛け合わせる
	//D3DXMatrixMultiply(&m_mtxWorld,
	//	&m_mtxWorld,
	//	&m_mtxParent);

	//各パーツのワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	if (m_CrystalModel)
	{
		m_CrystalModel->SetMtx(m_mtxWorld);
		m_CrystalModel->Draw();
	}
}

//--------------------------------------------
//描画
//--------------------------------------------
CAssistCrystal * CAssistCrystal::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CPlayer *pPlayer, const int& nLevel)
{
	CAssistCrystal *pRushAttack = NULL;
	pRushAttack = new CAssistCrystal();
	pRushAttack->m_pos = pos;
	pRushAttack->m_fInstallationAngle = rot.y;
	pRushAttack->m_pPlayer = pPlayer;
	pRushAttack->m_nLevel = nLevel;
	pRushAttack->Init();

	return pRushAttack;
}

void CAssistCrystal::PlayerFollowing()
{
	D3DXVECTOR3 Pos = { m_pPlayer->GetPos().x + (cosf(m_fInstallationAngle-m_pPlayer->GetRot().y)*PlayerDistance),0.0f,
						m_pPlayer->GetPos().z + (sinf(m_fInstallationAngle-m_pPlayer->GetRot().y)*PlayerDistance) };

	{
		//カメラの注視点とプレイヤーの位置の差分
		float PosDifference_X = m_pos.x - Pos.x;

		//プレイヤーの位置に戻す速度
		float RevertSpeed = abs(PosDifference_X / FollowingLimit);
		//RevertSpeedが0.0f出はなかったら
		if (RevertSpeed != 0.0f)
		{
			//差分が正の数なら負の数に戻すようにする
			if (PosDifference_X > 0)
			{
				m_pos.x -= RevertSpeed;
			}
			else
			{
				m_pos.x += RevertSpeed;
			}
		}
	}
	{
		float PosDifference_Z =  m_pos.z - Pos.z;//カメラの注視点とプレイヤーの位置の差分
		float RevertSpeed = abs(PosDifference_Z / FollowingLimit);//プレイヤーの位置に戻す速度
															   //RevertSpeedが0.0f出はなかったら
		if (RevertSpeed != 0.0f)
		{
			//差分が正の数なら負の数に戻すようにする
			if (PosDifference_Z > 0)
			{
				m_pos.z -= RevertSpeed;
			}
			else
			{
				m_pos.z += RevertSpeed;
			}

		}

	}

}
