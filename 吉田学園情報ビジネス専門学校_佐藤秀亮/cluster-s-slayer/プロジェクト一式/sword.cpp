//=============================================
//swordのクラス
//=============================================
#include "sword.h"
#include "manager.h"
#include "Renderer.h"
#include "model.h"
#include "enemy.h"
#include "effect.h"
#include "game.h"
#include "player.h"
static const D3DXVECTOR3 HitCollisionPos = { 0.0f,50.0f,0.0f };//攻撃当たり判定の位置設定
static const float HitSize = 50.0f;//攻撃の当たり判定の大きさ
//---------------------------------------------
//コンストラクタ
//---------------------------------------------
CSword::CSword() 
{
	m_bWeaponDraw = true;
	m_bCanHitCollision = false;
}

//-----------------------------------------------
//デストラクタ
//---------------------------------------------
CSword::~CSword()
{
}

//-----------------------------------------------
//初期化
//---------------------------------------------
HRESULT CSword::Init()
{
	if (!m_pWeapon)
	{
		//武器モデルの生成
		m_pWeapon = CModel::Create(m_pos, m_rot, 15, CModel::TYPE_PLAYER);
		//武器をどこに持たせるかの設定
		m_pWeapon->SetParent(m_pParent);
		//攻撃当たり判定の位置の設定
		m_HitCollision.m_pos = HitCollisionPos;
	}

	return S_OK;
}

//-----------------------------------------------
//終了
//---------------------------------------------
void CSword::Uninit()
{
	CWeapon::Uninit();

}

//-----------------------------------------------
//更新
//---------------------------------------------
void CSword::Update()
{

	CWeapon::Update();
	//攻撃当たり判定がオンなら
	if (m_bCanHitCollision)
	{
		CPlayer *pPlayer = CManager::GetGame()->GetPlayer();

		//敵の情報を持ってきて当たり判定の処理をする
		//敵との当たり判定
		CScene *pScene_Enemy = CScene::GetScene(CScene::OBJTYPE_ENEMY);
		while (pScene_Enemy != NULL)
		{
			CEnemy *pEnemy = (CEnemy*)pScene_Enemy;
			//死亡判定を取得
			bool bIsDeath = pEnemy->GetDeath();
			bool bNear = pEnemy->GetNearPlayer();
			//敵が生きていたら
			if (!bIsDeath&& bNear)
			{
				D3DXVECTOR3 EnemyPos = pEnemy->GetPos();
				int nSize = pEnemy->GetParts().size();
				if (nSize != 0)
				{
					float fRadius = pEnemy->GetParts(0)->GetMaxPos().x;
					bool bHitAttack = pEnemy->bHitAttack();
					//敵に攻撃を当てたら
					if (IsCollision(EnemyPos, fRadius) && !bHitAttack)
					{
						bool bRushAttack = pPlayer->GetCanRushAttack();
						if (bRushAttack)
						{
							pEnemy->SetRushAttack(true);
						}
						//敵を跳ねさせる
						if (m_nCombo == 3)
						{
							pEnemy->SetGravity(14.0f);
							pEnemy->SetbKnockback(true);

						}
						//敵に当たった判定を渡す
						pEnemy->SetbDamage(true);
						pEnemy->AddLife((int)-m_fPower);
						//ヒットエフェクト
						std::random_device random;	// 非決定的な乱数生成器
						std::mt19937_64 mt(random());            // メルセンヌ・ツイスタの64ビット版、引数は初期シード
						std::uniform_real_distribution<> randAng(-D3DX_PI, D3DX_PI);
						D3DXVECTOR3 EnemyVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						EnemyVec = EnemyPos - m_pos;			//敵と弾のベクトル
						D3DXVECTOR3 EnemyPosBody = {};
						D3DXVECTOR3 EnemyPosHead = {};
						if (pEnemy->GetParts(0) != nullptr)
						{
							EnemyPosBody = pEnemy->GetParts(0)->GetMaxPos();
							EnemyPosHead = pEnemy->GetParts(1)->GetMaxPos();
						}
						float fEnemyAng = atan2(EnemyVec.x, EnemyVec.z) + D3DX_PI;
						D3DXVECTOR3 Addmove = D3DXVECTOR3(
							sinf(fEnemyAng)*EnemyPosBody.x,
							EnemyPosHead.y / 2.0f,
							cosf(fEnemyAng)*EnemyPosBody.x);

						float fAng = (float)randAng(mt);

						CEffect::Create(Addmove + EnemyPos, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,0.0f },
						{ 1.0,1.0,1.0,0.5f }, false, 0.0f, 0.01f, true, CTexture::HitEffect, fAng, true);
						CEffect::Create(Addmove + EnemyPos, { 0.0f,0.0f,0.0f }, { 0.4f,0.2f,0.0f },
						{ 1.0,1.0,1.0,0.7f }, false, 0.0f, 0.015f, true, CTexture::HitEffect, fAng, true);

						CEffect::Create(Addmove + EnemyPos, { 0.0f,0.0f,0.0f }, { 1.5f,1.0f,0.0f },
						{ 1.0f,0.5f,0.0f,1.0f }, false, 0.0f, 0.03f, true, CTexture::Effect, fAng, false, true);
						CEffect::Create(Addmove + EnemyPos, { 0.0f,0.0f,0.0f }, { 1.5f,1.0f,0.0f },
						{ 1.0f,0.5f,0.0f,1.0f }, false, 0.0f, 0.03f, true, CTexture::Effect, fAng, false, true);
					}

				}

			}

			pScene_Enemy = pScene_Enemy->GetSceneNext(pScene_Enemy);
		}
	}
}

//-----------------------------------------------
//描画
//---------------------------------------------
void CSword::Draw()
{
	m_lastpos = { m_HitCollision.m_mtxWorld._41,m_HitCollision.m_mtxWorld._42 ,m_HitCollision.m_mtxWorld._43 };

	CWeapon::Draw();
}
//----------------------------------------------
//インスタンス生成
//----------------------------------------------
CSword * CSword::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel *pParent)
{
	CSword *pSword = nullptr;
	pSword = new CSword;
	pSword->m_pos = pos;
	pSword->m_rot = rot;
	pSword->m_pParent = pParent;
	pSword->Init();

	return pSword;
}
//----------------------------------------------
//当たり判定
//----------------------------------------------
bool CSword::IsCollision(const D3DXVECTOR3 & Hitpos, const float & fRadius)
{
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//自分と相手のベクトル
	D3DXVECTOR3 lastvec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//自分と相手のベクトル

	D3DXVECTOR3 pos = { m_HitCollision.m_mtxWorld._41,m_HitCollision.m_mtxWorld._42 ,m_HitCollision.m_mtxWorld._43 };
	vec = pos - Hitpos;
	lastvec = m_lastpos - Hitpos;
	float fLength = 0.0f;
	float fLastLength = 0.0f;

	//相手と自分の距離を求める
	fLength = sqrtf((vec.z*vec.z) + (vec.x*vec.x));
	fLastLength = sqrtf((lastvec.z*lastvec.z) + (lastvec.x*lastvec.x));
	float fCollisionRadius = HitSize + fRadius;
	//相手と自分の距離が自分の当たり判定の大きさより小さくなったら
	if (fLength <= fCollisionRadius &&fLastLength > fCollisionRadius)
	{
		//当たったという判定を返す
		return true;
	}
	return false;
}
