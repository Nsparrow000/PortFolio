//-----------------------------
//敵の処理
//-----------------------------
#include "enemy001.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "layer_structure.h"
#include "game.h"
#include "player.h"
#include "collision.h"
#include "stage.h"
#include "sound.h"
#include "effect.h"
#include "gauge.h"
#include "Particle.h"
#include "wall.h"
#include "map_polygon.h"
#include "shadow.h"
#include "motion.h"
#include "exp_ball.h"
#include "smallscore.h"
#include "character_partsdata.h"
#include "damage_collision.h"
#include "gaugeber.h"
static const float AttackStartNear = 150.0f;//攻撃を始めるまでのプレイヤーとの近さ
static const float AttackStartTime = 20.0f;//攻撃開始までの時間
static const int Power = 10;//攻撃力
static const int Life = 20;//体力
static const float MoveSpeed = 1.0f;//移動速度
static const int MinEXPNum = 2;//経験値を落とす数
static const int MaxEXPNum = 5;//経験値を落とす数
static const float AttackSize = 100.0f;//攻撃範囲
static const float AttackCollisionStartTime = 15.0f;//攻撃判定開始までの時間
static const float AttackCollisionEndTime = 20.0f;//攻撃判定開始までの時間
static const D3DXCOLOR AttackEffect = {1.0,0.2f,0.6f,1.0f};//攻撃判定開始までの時間

CEnemy001::CEnemy001(OBJTYPE nPriority) : CEnemy(nPriority)
{
	m_nPower = Power;
	m_fHitPoint = (float)Life;
	m_fMoveSpeed = MoveSpeed;
	m_pDamageCollision = nullptr;
	m_bOnAttackCollision = false;
}

CEnemy001::~CEnemy001()
{

}
//----------------------------------
//インスタンス生成
//----------------------------------
CEnemy001 *CEnemy001::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CEnemy001 *pEnemy = NULL;
	pEnemy = new CEnemy001(OBJTYPE_ENEMY);
	pEnemy->m_pos = pos;
	pEnemy->m_rot = rot;
	pEnemy->Init();
	return pEnemy;
}
//----------------------------------
//初期化
//----------------------------------
HRESULT CEnemy001::Init()
{
	m_nType = NONE;
	CCharacterPartsData::SetParts(&m_pParts);

	int nSize = m_pParts.size();
	if (nSize != 0)
	{
		m_fRadius = m_pParts[0]->GetMaxPos().x;
	}
	if (!m_pDamageCollision)
	{
		m_pDamageCollision = CDamage_Collision::Create({0.0f,50.0f,30.0f}, AttackSize,m_pParts[3]);
	}
	if (!m_pMotion)
	{
		m_pMotion = new CMotion;
		m_MotionType = 0;
		m_pMotion->MotionLoad("data/TEXT/poyo_motion.txt");
	}
	CEnemy::Init();

	return S_OK;
}
//----------------------------------
//終了
//----------------------------------
void CEnemy001::Uninit()
{
	int nDiameter = CManager::GetGameTimeMinut();
	static std::random_device random;	// 非決定的な乱数生成器
	std::mt19937_64 mt(random());            // メルセンヌ・ツイスタの64ビット版、引数は初期シード
	std::uniform_real_distribution<> randEXPNum(MinEXPNum, MaxEXPNum);//経験値の数を乱数で決める
	std::uniform_real_distribution<> randBonusEXPNum(0, 1 * (nDiameter / 60.0f));//経験値の数を乱数で決める

	int nExpNum = randEXPNum(mt);
	int nExpBonus = randBonusEXPNum(mt);
	//経験値を落とす
	for (int nCnt = 0; nCnt < nExpNum; nCnt++)
	{
		CExp_Ball::Create(m_pos,1);
	}
	for (int nCnt = 0; nCnt < nExpBonus; nCnt++)
	{
		CExp_Ball::Create(m_pos, 5);
	}
	CEnemy::Uninit();
	Release();
}
//-----------------------------------------
//更新
//-----------------------------------------
void CEnemy001::Update()
{
	CEnemy::Update();

	if (m_pDamageCollision)
	{
		m_pDamageCollision->Update();
	}
	//モーションタイプが攻撃なら
	if (m_bOnAttackCollision)
	{
		Attack();
	}
	else
	{
		m_fAttackCollisionTime = 0.0f;
	}
}
//-----------------------------------------
//描画
//-----------------------------------------
void CEnemy001::Draw()
{
	CEnemy::Draw();
	m_pDamageCollision->Draw();

}

//-----------------------------------------
//攻撃処理
//-----------------------------------------
void CEnemy001::AIAttack()
{
	//プレイヤーが攻撃範囲に入ったら
	if (m_fPlayerVecLength < AttackStartNear)
	{
		if (m_bAttack == false)
		{
			m_nAttackStartCnt++;
			//攻撃開始時間になったら
			if (m_nAttackStartCnt >= AttackStartTime)
			{
				m_nAttackStartCnt = 0;
				m_MotionType = ATTACK;
				m_bAttack = true;
				//当たり判定を発生させる
				m_bOnAttackCollision = true;
				//攻撃音
				//CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SWORD_ATTACK);
				//CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_SWORD_ATTACK, 0.7f);
			}

		}
	}
	else
	{
		m_nAttackStartCnt = 0;

	}

}
//-----------------------------------------
//攻撃処理
//-----------------------------------------
void CEnemy001::Attack()
{
	CPlayer *pPlayer = CManager::GetGame()->GetPlayer();
	m_fAttackCollisionTime++;
	if (m_fAttackCollisionTime >= AttackCollisionStartTime &&
		m_fAttackCollisionTime <= AttackCollisionEndTime)
	{
		////プレイヤーに対しての当たり判定
		if (pPlayer)
		{
			int nSize = pPlayer->GetParts().size();
			if (nSize > 0)
			{
				float fRadius = pPlayer->GetParts(0)->GetMaxPos().x*3.0f;
				bool bHit = false;
				D3DXVECTOR3 pos = pPlayer->GetPos();
				bHit = m_pDamageCollision->IsCollision(pos, fRadius);
				if (bHit)
				{
					pPlayer->HitDamege(Power);
					m_fAttackCollisionTime = 0.0f;
					m_bOnAttackCollision = false;
					//ヒットエフェクト
					std::random_device random;	// 非決定的な乱数生成器
					std::mt19937_64 mt(random());            // メルセンヌ・ツイスタの64ビット版、引数は初期シード
					std::uniform_real_distribution<> randAng(-D3DX_PI, D3DX_PI);
					D3DXVECTOR3 PlayerVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					float fHitHeight = 0.0f;
					if (m_pParts[0])
					{
						fHitHeight = m_pParts[0]->GetMtxWorld()._42 + m_pParts[0]->GetMaxPos().y;
					}
					PlayerVec = pos - m_pos;			//敵と弾のベクトル

					float fEnemyAng = atan2(PlayerVec.x, PlayerVec.z) + D3DX_PI;
					D3DXVECTOR3 Addmove = D3DXVECTOR3(
						sinf(fEnemyAng)*fRadius,
						fHitHeight,
						cosf(fEnemyAng)*fRadius);

					float fAng = randAng(mt);

					CEffect::Create(Addmove + pos, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,0.0f },
					{ 1.0,AttackEffect.g,AttackEffect.b,0.5f }, false, 0.0f, 0.01f, true, CTexture::HitEffect, fAng, true);
					CEffect::Create(Addmove + pos, { 0.0f,0.0f,0.0f }, { 0.4f,0.2f,0.0f },
					{ 1.0,AttackEffect.g,AttackEffect.b,0.7f }, false, 0.0f, 0.015f, true, CTexture::HitEffect, fAng, true);

					CEffect::Create(Addmove + pos, { 0.0f,0.0f,0.0f }, { 1.5f,1.0f,0.0f },
					{ 1.0f,AttackEffect.g,0.0f,1.0f }, false, 0.0f, 0.03f, true, CTexture::Effect, fAng, false, true);
					CEffect::Create(Addmove + pos, { 0.0f,0.0f,0.0f }, { 1.5f,1.0f,0.0f },
					{ 1.0f,AttackEffect.g,0.0f,1.0f }, false, 0.0f, 0.03f, true, CTexture::Effect, fAng, false, true);

				}
			}

		}

	}

}
//-----------------------------------------
//移動処理
//-----------------------------------------
void CEnemy001::AIMove()
{
	if (m_fPlayerVecLength < MAX_COLRADIUS)
	{
		m_bMove = true;
		//自動で動くできるやつをオフにする
		m_bAIMove = false;
		m_bAIMoveStop = true;

	}
	//範囲外の時
	else
	{
		m_bMove = false;
		m_bAIMoveStop = false;

		s_bCntStop = true;
	}
	//自動移動ができるなら
	if (m_bMove)
	{
		m_rot.y = m_fAng + (D3DX_PI);
		//発射する方向を求める
		m_MoveSpeed.x = sinf(m_fAng)*MoveSpeed;
		m_MoveSpeed.z = cosf(m_fAng)*MoveSpeed;
		m_MotionType = MOVE;

		m_fWalkSoundInterval -= 0.1f;

		if (m_fWalkSoundInterval <= 0.0f)
		{
			//CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENEMYWALK);
			//CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_ENEMYWALK, 0.6f);
			m_fWalkSoundInterval = 2.3f;
		}
		m_pos += m_MoveSpeed;

	}
	else
	{
		m_MotionType = NEUTRAL;
		m_MoveSpeed.x = 0.0f;
		m_MoveSpeed.y = 0.0f;
		m_MoveSpeed.z = 0.0f;
	}

}
//----------------------------------------------------------
//体力の増減
//----------------------------------------------------------
void CEnemy001::AddLife(int nLife)
{
	if (m_bDamage == true && m_bHitCollision == true)
	{

		int nDamege = nLife;
		m_fHitPoint += nDamege;
		int nDrawDamage = abs(nDamege);
		CSmallScore::Create({m_pos.x,m_pos.y+30.0f,m_pos.z}, { 10.0f,20.0f,0.0f }, { 1.0f, 0.3f, 0.3f, 0.0f }, nDrawDamage);
	}
}

void CEnemy001::AddLife(int nPower, int nType)
{
	if (m_bDamage == true)
	{
		float fDamege = 0;
		switch (nType)
		{
		case FIRE:
			fDamege = (nPower + m_nDefense)*1.5f;

			break;
		case BULLIZAD:
			fDamege = nPower + m_nDefense;

			break;
		}
		m_fHitPoint += fDamege;

	}

}
