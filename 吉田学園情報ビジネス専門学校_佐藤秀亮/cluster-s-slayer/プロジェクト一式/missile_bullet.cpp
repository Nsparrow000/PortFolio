//-------------------------------------------
//ミサイル弾の処理
//------------------------------------------
#include "missile_bullet.h"
#include "manager.h"
#include "Renderer.h"
#include "Particle.h"
#include "enemy.h"
#include "PresetSetEffect.h"
#include "player.h"
#include "model.h"
#include "sound.h"

static const float MoveSpeed = 12.0f;
static const int DeleteTime = 200;

//--------------------------------------------
//コンストラクタ
//--------------------------------------------
CMissile_Bullet::CMissile_Bullet(OBJTYPE nPriority) :CScene(nPriority)
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_rot = { 0.0f,0.0f,0.0f };
	m_scale = { 0.0f,0.0f,0.0f };
	m_pMissileModel = nullptr;
}

//--------------------------------------------
//デストラクタ
//--------------------------------------------
CMissile_Bullet::~CMissile_Bullet()
{
}

//--------------------------------------------
//初期化
//--------------------------------------------
HRESULT CMissile_Bullet::Init(void)
{
	if (!m_pMissileModel)
	{
		m_pMissileModel = CModel::Create({0.0f,0.0f,0.0f}, { 0.0f,0.0f,0.0f }, 7, CModel::TYPE_OBJECT);
		m_pMissileModel->SetCanObjParent(false);
	}
	return S_OK;
}
//--------------------------------------------
//終了
//--------------------------------------------
void CMissile_Bullet::Uninit(void)
{
	if(m_pMissileModel)
	{
		m_pMissileModel->Uninit();
		m_pMissileModel = nullptr;
	}
	Release();
}
//--------------------------------------------
//更新
//--------------------------------------------
void CMissile_Bullet::Update(void)
{
	m_pos.y -= MoveSpeed;
	if (m_pos.y <= 0.0f)
	{
		//敵との当たり判定
		CScene *pScene_Enemy = CScene::GetScene(CScene::OBJTYPE_ENEMY);
		while (pScene_Enemy != NULL)
		{
			CEnemy *pEnemy = (CEnemy*)pScene_Enemy;
			//死亡判定を取得
			bool bIsDeath = pEnemy->GetDeath();
			//敵が生きていたら
			if (!bIsDeath)
			{
				D3DXVECTOR3 EnemyPos = pEnemy->GetPos();
				int nSize = pEnemy->GetParts().size();
				bool bLimit = pEnemy->GetNearPlayer();
				if (nSize != 0 && bLimit)
				{
					float fRadius = pEnemy->GetParts(0)->GetMaxPos().x;
					bool bHitAttack = pEnemy->bHitAttack();
					//敵に攻撃を当てたら
					if (IsCollision(EnemyPos, fRadius) && !bHitAttack)
					{
						//敵に当たった判定を渡す
						pEnemy->SetbDamage(true);
						pEnemy->AddLife(-m_nPower);

					}
				}
			}
			pScene_Enemy = pScene_Enemy->GetSceneNext(pScene_Enemy);

		}
		m_bUninit = true;

	}
	if (m_bUninit)
	{
		//エフェクト
		CPresetEffect::SetEffect3D(13, m_pos, {},{},{});
		CPresetEffect::SetEffect3D(14, m_pos, {},{},{});
		CPresetEffect::SetEffect3D(15, m_pos, {},{},{});
		CPresetEffect::SetEffect3D(12, m_pos, {}, D3DXVECTOR3(100.0f, 0.0f, 100.0f),true);
		CPresetEffect::SetEffect3D(19, m_pos, {}, D3DXVECTOR3(100.0f, 0.0f, 100.0f), true);

		CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_MISSILEBURST);
		CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_MISSILEBURST, 1.0f);

		Uninit();
	}
}

//--------------------------------------------
//描画
//--------------------------------------------
void CMissile_Bullet::Draw()
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

	//各パーツのワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	if (m_pMissileModel)
	{
		m_pMissileModel->SetParentMtx(m_mtxWorld);
		m_pMissileModel->Draw();
	}
}

//----------------------------------------------
//当たり判定
//----------------------------------------------
bool CMissile_Bullet::IsCollision(const D3DXVECTOR3 & Hitpos, const float & fRadius)
{
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//自分と相手のベクトル
	D3DXVECTOR3 lastvec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//自分と相手のベクトル

	vec = m_pos - Hitpos;
	float fLength = 0.0f;
	float fLastLength = 0.0f;

	//相手と自分の距離を求める
	fLength = sqrtf((vec.z*vec.z) + (vec.x*vec.x));
	float fCollisionRadius = (m_fRadiusSize*2) + fRadius;
	//相手と自分の距離が自分の当たり判定の大きさより小さくなったら
	if (fLength <= fCollisionRadius)
	{
		//当たったという判定を返す
		return true;
	}
	return false;
}

//--------------------------------------------
//描画
//--------------------------------------------
CMissile_Bullet * CMissile_Bullet::Create(const D3DXVECTOR3& pos, const float& fSize, const int& nPower)
{
	CMissile_Bullet *pRushAttack = NULL;
	pRushAttack = new CMissile_Bullet(OBJTYPE_EFFECT);
	pRushAttack->m_pos = pos;
	pRushAttack->m_nPower = nPower;
	pRushAttack->m_fRadiusSize = fSize;
	pRushAttack->Init();

	return pRushAttack;
}