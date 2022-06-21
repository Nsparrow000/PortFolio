//-------------------------------------------
//ブラックホールの処理
//------------------------------------------
#include "bullet.h"
#include "manager.h"
#include "Renderer.h"
#include "Particle.h"
#include "enemy.h"
#include "model.h"
#include "3DPolygon.h"
#include "enemy.h"
const float Size = 10.0f;
const int MaxTime = 90;
const float MoveSpeed = 5.0f;
const int Power = 5;

//--------------------------------------------
//コンストラクタ
//--------------------------------------------
CBullet::CBullet(OBJTYPE nPriority):CScene(nPriority)
{
	m_bUninit = false;
	m_pos = { 0.0f,0.0f,0.0f };
	m_rot = { 0.0f,0.0f,0.0f };
	m_bMoveDirection = true;
	m_pBullet = nullptr;
}

//--------------------------------------------
//デストラクタ
//--------------------------------------------
CBullet::~CBullet()
{

}

//--------------------------------------------
//初期化
//--------------------------------------------
HRESULT CBullet::Init(void)
{
	if (!m_pBullet)
	{
		m_pBullet = C3DPolygon::Create(m_pos, { 0.0f,0.0f,0.0f }, { Size, Size, 0.0f }, CTexture::FIRE, {1.0,1.0,1.0,1.0},true);
	}
	return S_OK;
}
//--------------------------------------------
//終了
//--------------------------------------------
void CBullet::Uninit(void)
{
	if (m_pBullet)
	{
		m_pBullet->Uninit();
		m_pBullet = nullptr;
	}
	Release();
}
//--------------------------------------------
//更新
//--------------------------------------------
void CBullet::Update(void)
{
	m_nTimer++;
	if (m_nTimer >= MaxTime)
	{
		m_bUninit = true;
	}
	m_pos.x += sinf(m_rot.y)*m_fMoveSpeed;
	m_pos.z += cosf(m_rot.y)*m_fMoveSpeed;

	if (m_pBullet)
	{
		m_pBullet->SetPos(m_pos);
	}
	//敵の情報を持ってきて当たり判定の処理をする
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
					m_bUninit = true;
				}

			}

		}

		pScene_Enemy = pScene_Enemy->GetSceneNext(pScene_Enemy);
	}

	if (m_bUninit)
	{
		Uninit();
	}
}

//--------------------------------------------
//描画
//--------------------------------------------
void CBullet::Draw()
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


	//算出した各パーツのワールドマトリックスと親のマトリックスを掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&m_mtxParent);

	//各パーツのワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	if (m_pBullet)
	{
		m_pBullet->Draw();
	}

}

//--------------------------------------------
//描画
//--------------------------------------------
CBullet * CBullet::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float& fMoveSpeed, const int& nPower)
{
	CBullet *pRushAttack = NULL;
	pRushAttack = new CBullet();
	pRushAttack->m_pos = pos;
	pRushAttack->m_rot = rot;
	pRushAttack->m_fMoveSpeed = fMoveSpeed;
	pRushAttack->m_nPower = nPower;

	pRushAttack->Init();

	return pRushAttack;
}
//----------------------------------------------
//当たり判定
//----------------------------------------------
bool CBullet::IsCollision(const D3DXVECTOR3 & Hitpos, const float & fRadius)
{
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//自分と相手のベクトル
	D3DXVECTOR3 lastvec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//自分と相手のベクトル

	vec = m_pos - Hitpos;
	float fLength = 0.0f;
	float fLastLength = 0.0f;

	//相手と自分の距離を求める
	fLength = sqrtf((vec.z*vec.z) + (vec.x*vec.x));
	fLastLength = sqrtf((lastvec.z*lastvec.z) + (lastvec.x*lastvec.x));
	float fCollisionRadius = Size + fRadius;
	//相手と自分の距離が自分の当たり判定の大きさより小さくなったら
	if (fLength <= fCollisionRadius)
	{
		//当たったという判定を返す
		return true;
	}
	return false;
}
