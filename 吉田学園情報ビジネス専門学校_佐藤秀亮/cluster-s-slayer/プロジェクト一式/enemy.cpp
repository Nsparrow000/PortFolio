//-----------------------------
//敵の処理
//-----------------------------
#include "Enemy.h"
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
#include "smallscore.h"
#define MIN_MOVE (80.0)		//敵が動く最小の範囲
#define MAX_MOVE (100.0)	//敵が動く最大の範囲
#define MIN_TIME (180.0)		//敵が動くまでの時間の最小の範囲
#define MAX_TIME (240.0)	//敵が動くまでの時間の最大の範囲
#define MAX_SPEED (1.8)
#define KNOCK_BACK_SPEED (3.0)
#define MAX_HIT_TIME (30)//敵の無敵時間
#define MAX_ENEMY_LIFE (10)//敵の命
#define MAX_ENEMY_DEFENSE (2)//敵の防御力
#define MAX_ATTACKSTARTTIME (20)//攻撃開始までの時間
#define ADDCP (10)		//無敵判定の時間
static const float GravitySpeed = 0.7f;
int CEnemy::m_nMaxEnemy = 0;
CEnemy::CEnemy(OBJTYPE nPriority) : CCharacter(nPriority)
{
	m_bAIMove = false;
	m_bAIMoveStop = false;
	s_bCntStop = true;
	m_bHit = false;
	m_bInvincible = false;
	m_pMotion = nullptr;
	m_nAttackStartCnt = MAX_ATTACKSTARTTIME;
	m_bMotionStop = false;
	m_bMove = false;
	m_bHitCollision = true;
	m_bHitRushAttack = false;
	m_pShadow = nullptr;
	m_bCanHitRushAttack = true;
	m_fGravity = 0.0f;
	m_nMaxEnemy++;
	m_bIsKill = false;
	m_bNearPlayer = false;
}

CEnemy::~CEnemy()
{

}
//----------------------------------
//インスタンス生成
//----------------------------------
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CEnemy *pEnemy = NULL;
	pEnemy = new CEnemy(OBJTYPE_ENEMY);
	pEnemy->m_pos = pos;
	pEnemy->m_rot = rot;
	pEnemy->Init();
	return pEnemy;
}
//----------------------------------
//初期化
//----------------------------------
HRESULT CEnemy::Init()
{
	m_nDefense = MAX_ENEMY_DEFENSE;
	m_fWalkSoundInterval = 0.0f;
	m_fSoundInterval = 0.0f;
	m_bDamage = false;

	std::mt19937_64 s_mt(s_random());            // メルセンヌ・ツイスタの64ビット版、引数は初期シード
	std::uniform_real_distribution<> s_randAng(-3.14, 3.14);
	std::uniform_real_distribution<> s_randMove(MIN_MOVE, MAX_MOVE);
	std::uniform_real_distribution<> s_randTime(MIN_TIME, MAX_TIME);


	s_MaxTimer = (float)s_randTime(s_mt);
	m_fMovingRange = (float)s_randMove(s_mt);
	m_rot.y = (float)s_randAng(s_mt)+(D3DX_PI);
	s_MoveRandAng = (float)s_randAng(s_mt);
	if (!m_pShadow)
	{
		m_pShadow = CShadow::Create({ 0.0f,0.0f,0.0f }, 50.0f, CTexture::Effect);
		m_pShadow->SetPos(0.0f, 0.0f, { m_pParts[0]->GetMaxPos().x ,0.0,m_pParts[0]->GetMaxPos().z });
		m_pShadow->SetPos(m_pos);

	}

	return S_OK;
}
//----------------------------------
//終了
//----------------------------------
void CEnemy::Uninit()
{
	CCharacter::Uninit();
	m_nMaxEnemy--;
	Release();
}
//-----------------------------------------
//更新
//-----------------------------------------
void CEnemy::Update()
{

	//敵のAI移動
	//死んでいなかったら行動する
	if (m_fHitPoint <= 0)
	{
		m_MotionType = DETH;
	}
	if (m_MotionType != DETH)
	{
		AIBehavior();
		if(m_pShadow != nullptr)
		{
			int nSize = m_pParts.size();
			if (nSize != 0)
			{
				m_pShadow->SetPos(0.0f, 0.0f, { m_pParts[0]->GetMaxPos().x ,0.0,m_pParts[0]->GetMaxPos().z });
				m_pShadow->SetPos(m_pos);

			}

		}
		//重力に関する処理
		m_fGravity -= GravitySpeed;
		m_pos.y += m_fGravity;

		if(m_pos.y <= 0.0f)
		{
			m_pos.y = 0.0f;
		}

		if (m_bDraw == true)
		{
			IsModelCollision(false);

			CCollision *pCollision = new CCollision;
			//ダメージを受けたら無敵状態にする
			if (m_bDamage == true)
			{
				if (m_bEffect == false)
				{
					m_bEffect = true;
				}
				//カウンターを増やす
				m_nTimer++;
			}
			//カウンターが特定の時間に行ったら
			if (m_nTimer >= INVINCIBLE_TIME)
			{
				//通常状態に戻す
				m_bDamage = false;
				m_bEffect = false;
			}
			//プレイヤーとの距離が近かったら
			if (m_bNearPlayer)
			{
				CPlayer *pPlayer = CManager::GetGame()->GetPlayer();

				////プレイヤーに対しての当たり判定
				if (pPlayer)
				{
					float fRadius = pPlayer->GetParts(0)->GetMaxPos().x*3.0f;
					IsCollision(&m_pos, pPlayer->GetPos(), fRadius, 5.0f);
				}
				//ノックバック状態なら
				if (m_bKnockback == true)
				{
					Knockback(pPlayer->GetPos());
				}
				CScene *pScene_Enemy = CScene::GetScene(OBJTYPE_ENEMY);

				////敵同士の当たり判定
				while (pScene_Enemy != NULL)
				{
					if (pScene_Enemy != NULL && pScene_Enemy != this)
					{
						CEnemy *pEnemy = (CEnemy*)pScene_Enemy;
						bool bIsDeath = pEnemy->GetDeath();
						bool bNear = pEnemy->GetNearPlayer();
						if (!bIsDeath && bNear)
						{
							D3DXVECTOR3 EnemyPos = pEnemy->GetPos();
							int nSize = pEnemy->GetParts().size();
							if (nSize != 0)
							{
								float fRadius = pEnemy->GetParts(0)->GetMaxPos().x;
								if (IsCollision(&m_pos, EnemyPos, fRadius, m_fMoveSpeed*2.0f))
								{
									break;
								}

							}

						}
					}
					pScene_Enemy = pScene_Enemy->GetSceneNext(pScene_Enemy);
				}

			}

			CScene *pScene_Wall = CScene::GetScene(OBJTYPE_WALL);
			//壁との当たり判定
			while (pScene_Wall != nullptr)
			{
				CWall *pWall = (CWall*)pScene_Wall;
				CScene *pNext_Wall = CScene::GetSceneNext(pScene_Wall);
				//当たり判定
				bool bHit = pWall->TestCollision(&m_pos, &m_lastPos, m_pParts[0]->GetMaxPos().x);
				pScene_Wall = pNext_Wall;

			}


			//モーション再生
			if (m_pMotion != NULL)
			{
				int nSize = m_pParts.size();
				if (nSize != 0)
				{
					m_pMotion->PlayMotion(m_pParts.size(), &m_pParts[0], m_MotionType, m_MotionLastType, m_bMotionStop, m_bAttack, m_bNeutral, false);
				}
			}

			delete pCollision;
		}

	}
	else
	{
		//モーション再生
		if (m_pMotion != NULL)
		{
			m_pMotion->NoLoopPlayMotion(m_pParts.size(), &m_pParts[0], m_MotionType, m_MotionLastType);
		}
		if (m_pMotion->IsMotionEnd())
		{
			m_IsDeath = true;
			//消える音
			CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENEMY_DEATH);
			CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_ENEMY_DEATH,0.1f);
			CManager::GetGame()->GetParticle()->PlayRandomCircle({ m_pos.x,m_pos.y + 30.0f,m_pos.z }, CParticle::HEELEFFECT, CTexture::GlitterEffect);
		}
		//敵の行動パターン実装予定
		if (m_IsDeath == true)
		{
			int nKillCnt = CManager::GetKilledEnemyCount();
			CManager::SetKilledEnemyCount(nKillCnt + 1);
			Uninit();
		}
	}

}
//-----------------------------------------
//描画
//-----------------------------------------
void CEnemy::Draw()
{

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ
	D3DXMATRIX mtxRotModel, mtxTransModel;//計算用マトリックス
	//各パーツのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixRotationYawPitchRoll(&mtxRotModel, m_rot.y, m_rot.x, m_rot.z);
	//向きを反映
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRotModel);
	//位置を反映
	D3DXMatrixTranslation(&mtxTransModel, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTransModel);

	//各パーツのワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	{
		int nSize = m_pParts.size();
		//モデル描画
		for (int nCnt = 0; nCnt < nSize; nCnt++)
		{
			if (m_pParts[nCnt] != nullptr)
			{
				m_pParts[nCnt]->SetDraw(m_bDraw);
				m_pShadow->SetDraw(m_bDraw);
				m_pParts[nCnt]->Draw();
			}
		}
	}
	//前回の位置情報の保存
	m_lastPos = m_pos;

}
void CEnemy::Colision()
{
}
void CEnemy::AIAttack()
{
}
//-----------------------------------------
//移動処理
//-----------------------------------------
void CEnemy::AIMove()
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
		m_MoveSpeed.x = sinf(m_fAng)*MAX_SPEED;
		m_MoveSpeed.z = cosf(m_fAng)*MAX_SPEED;
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
void CEnemy::AddLife(int nLife)
{
	if (m_bDamage == true && m_bHitCollision == true)
	{
		int nDamege = nLife+ m_nDefense;
		m_fHitPoint += nDamege;

	}
}
//----------------------------------------------------------
//ブラックホールに引き寄せられる処理
//----------------------------------------------------------
void CEnemy::HoleAlign(D3DXVECTOR3 holePos, float fHitSize, float AlignSpeed)
{
	D3DXVECTOR3 vec = m_pos - holePos;
	float Length = sqrtf(vec.x*vec.x + vec.z*vec.z);
	float Size = fHitSize + m_fRadius;
	float moveRot = atan2f(vec.x, vec.z);
	if (Length <= Size)
	{
		m_pos.x -= sinf(moveRot)*AlignSpeed;
		m_pos.z -= cosf(moveRot)*AlignSpeed;
	}
}
//----------------------------------------------------------
//体力の増減
//----------------------------------------------------------
void CEnemy::AddLifeSkill(int nLife)
{
	int nDamege = nLife;
	m_fHitPoint += nDamege;
	int nDrawDamage = abs(nDamege);
	CSmallScore::Create({ m_pos.x,m_pos.y + 30.0f,m_pos.z }, { 10.0f,20.0f,0.0f }, { 1.0f, 0.3f, 0.3f, 0.0f }, nDrawDamage);

}
//----------------------------------------------------------
//体力の増減
//----------------------------------------------------------
void CEnemy::AddLife(int nPower, int nType)
{
	if (m_bDamage)
	{
		float fDamege = 0;
		//ダメージ式を求める
		fDamege = nPower + m_nDefense;

		m_fHitPoint += fDamege;

	}

}

//-----------------------------------------
//無敵時間の処理
//-----------------------------------------
bool CEnemy::bHitAttack()
{
	//ヒットされたら無敵時間用の判定をオンにする
	if (m_bHit == true)
	{
		m_bInvincible = true;
		m_bHit = false;
		//ノックバック状態にする
		m_bKnockback = true;
	}
	//ヒットしていたら攻撃あたらないようにする
	if (m_bInvincible == true)
	{
		m_MotionType = DETH;
		m_bAttack = false;
		m_nCntHitInterval++;
		if (m_nCntHitInterval >= MAX_HIT_TIME)
		{
			m_nCntHitInterval = 0;

			m_bInvincible = false;
			m_bKnockback = false;


		}
	}
	return m_bInvincible;
}
//-----------------------------------------
//ノックバックの処理
//-----------------------------------------
void CEnemy::Knockback(D3DXVECTOR3& Playerpos)
{
	D3DXVECTOR3 PlayerVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PlayerVec = Playerpos - m_pos;			//敵と弾のベクトル
	m_fAng = atan2(PlayerVec.x, PlayerVec.z);//敵の向き
	//動き出す方向を求める
	float fSpeed_x = sinf(m_fAng)*KNOCK_BACK_SPEED;
	float fSpeed_z = cosf(m_fAng)*KNOCK_BACK_SPEED;
	//プレイヤーを動かす
	m_pos.x -= fSpeed_x;
	m_pos.z -= fSpeed_z;
	m_nCntHitInterval++;
	if (m_nCntHitInterval >= MAX_HIT_TIME)
	{
		m_nCntHitInterval = 0;
		m_bKnockback = false;
	}

}


//-----------------------------------------
//敵AIの挙動の処理
//-----------------------------------------
void CEnemy::AIBehavior()
{
	D3DXVECTOR3	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pos = CManager::GetGame()->GetPlayer()->GetPos();				//位置
	D3DXVECTOR3 PlayerVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PlayerVec = pos - m_pos;			//敵と弾のベクトル
	float fLengthX = 0.0f;
	m_fPlayerVecLength = sqrtf((PlayerVec.z*PlayerVec.z) + (PlayerVec.x*PlayerVec.x));
	m_fAng = atan2(PlayerVec.x, PlayerVec.z);//敵の向き
	float fAngle = atan2(PlayerVec.x, PlayerVec.z);//敵の向き
	//敵の出現
	if (m_fPlayerVecLength < MAX_DRAWRADIUS)
	{
		m_bDraw = true;
	}
	else
	{
		m_bDraw = false;
	}

	//敵の追従
	if (!m_bAttack)
	{
		AIMove();
	}
	//プレイヤーが攻撃範囲に入ったら
	AIAttack();

	if (!m_bAIMoveStop&& m_bDraw)
	{
		//敵が自動で動ける状態だったら
		if (m_bAIMove)
		{
			m_MotionType = MOVE;

			//どこまで動くかカウントする
			s_MovingCnt += MAX_SPEED;
			m_rot.y = s_MoveRandAng + (D3DX_PI);

			//動き出す方向を求める
			m_MoveSpeed.x = sinf(s_MoveRandAng)*MAX_SPEED;
			m_MoveSpeed.z = cosf(s_MoveRandAng)*MAX_SPEED;
			m_pos += m_MoveSpeed;

		}
		else
		{
			s_CntTimer += 1.0f;
			//カウンターが指定の範囲までカウントされたら敵が動く
			if (s_CntTimer >= s_MaxTimer&&s_bCntStop)
			{
				s_CntTimer = 0.0f;
				//敵が自動で動くための判定をオンにする
				m_bAIMove = true;

				static std::random_device s_random;	// 非決定的な乱数生成器
				std::mt19937_64 s_mt(s_random());            // メルセンヌ・ツイスタの64ビット版、引数は初期シード
				std::uniform_real_distribution<> s_randMove(MIN_MOVE, MAX_MOVE);

				m_fMovingRange = (float)s_randMove(s_mt);
				//向きを設定
				m_rot.y = s_MoveRandAng + (D3DX_PI);
				s_bCntStop = false;
			}

			s_MovingCnt = 0.0f;
			m_MoveSpeed.x = 0.0f;
			m_MoveSpeed.z = 0.0f;

		}
		//移動する指定の範囲に来たらr
		if (s_MovingCnt > m_fMovingRange)
		{
			static std::random_device s_random;	// 非決定的な乱数生成器

			std::mt19937_64 s_mt(s_random());            // メルセンヌ・ツイスタの64ビット版、引数は初期シード
			std::uniform_real_distribution<> s_randAng(-3.14, 3.14);
			std::uniform_real_distribution<> s_randTime(MIN_TIME, MAX_TIME);

			s_MoveRandAng = (float)s_randAng(s_mt);
			s_MaxTimer = (float)s_randTime(s_mt);
			m_fMovingRange = 0.0f;
			s_MovingCnt = 0.0f;
			//カウントを０にしてまた数え始める
			s_bCntStop = true;
			s_CntTimer = 0.0f;
			m_MoveSpeed.x = 0.0f;
			m_MoveSpeed.z = 0.0f;
			m_bAIMove = false;
		}

	}
	//敵が勝手に動く処理

}
