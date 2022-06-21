//=============================================================================
//
//アタックスキルmissileの処理
//
//=============================================================================
#include "missile.h"
#include "manager.h"
#include "Renderer.h"
#include "scene.h"
#include "model.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "missile_bullet.h"

#include "PresetSetEffect.h"
#include "skill_leveldata.h"

#include "sound.h"

static const float SearchRange = 200.0f;
static const float RandomRange = 150.0f;

static const float IntervalTime = 100.0f;
static const float IntervalShotTime = 20.0f;
static const float MissileSize = 50.0f;
static const float MissilePopPosY = 400.0f;

static const int SearchMax = 5;
static const int MissilePower = 5;

//=============================================================================
//コンストラクタ
//=============================================================================
CMissile::CMissile(OBJTYPE nPriority) :CScene(nPriority)
{
	m_bDraw = true;
	m_fIntervalTimer = 0.0f;
	m_State.m_SearchRange = SearchRange;
	m_nCntSearch = 0;
	m_State.m_nCntSearchMax = SearchMax;
	m_bCanSearch = true;
	m_bCanShotMissile = false;

}

//=============================================================================
//デストラクタ
//=============================================================================
CMissile::~CMissile()
{

}

//=============================================================================
//初期化
//=============================================================================
HRESULT CMissile::Init()
{
	m_State = CSkill_LevelData::GetStateMissile(CManager::GetGame()->GetPlayer()->GetSkillLevel(CPlayer::Rocket));
	//動的確保
	m_pEnemyPos = new D3DXVECTOR3[m_State.m_nCntSearchMax];

	CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_MISSILEWIND);
	CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_MISSILEWIND, 0.2f);

	return S_OK;

}

//=============================================================================
//終了
//=============================================================================
void CMissile::Uninit()
{
	Release();
}

//=============================================================================
//更新
//=============================================================================
void CMissile::Update()
{
	if (m_bCanSearch)
	{
		//敵との当たり判定
		CScene *pScene_Enemy = CScene::GetScene(CScene::OBJTYPE_ENEMY);
		while (pScene_Enemy != NULL)
		{
			if (pScene_Enemy != NULL)
			{
				CEnemy *pEnemy = (CEnemy*)pScene_Enemy;
				D3DXVECTOR3 EnemyPos = pEnemy->GetPos();
				int nSize = pEnemy->GetParts().size();
				if (nSize != 0&& pEnemy->GetParts(0))
				{
					//当たり判定
					float fRadius = pEnemy->GetParts(0)->GetMaxPos().x;
					//サーチ回数が限界に来るまで敵をサーチする
					if (m_nCntSearch < m_State.m_nCntSearchMax)
					{
						SearchEnemy(EnemyPos, fRadius);
					}
				}


			}
			pScene_Enemy = pScene_Enemy->GetSceneNext(pScene_Enemy);
		}

	}
	else if(!m_bCanSearch && !m_bCanShotMissile)
	{
		m_fIntervalTimer += 1.0f;
		if (m_fIntervalTimer >= IntervalTime)
		{
			m_fIntervalTimer = 0.0f;
			m_bCanShotMissile = true;
			m_nCntSearch = 0;
		}
	}
	if (m_nCntSearch >= m_State.m_nCntSearchMax)
	{
		m_bCanSearch = false;
	}
	if (m_bCanShotMissile)
	{
		m_fIntervalTimer += 1.0f;
		if (m_fIntervalTimer >= IntervalShotTime)
		{
			m_fIntervalTimer = 0.0f;
			//ミサイルを発射
			CMissile_Bullet::Create({ m_pEnemyPos[m_nCntSearch].x,MissilePopPosY,m_pEnemyPos[m_nCntSearch].z}, MissileSize, m_State.m_nPower);

			//次の敵の位置に移す
			m_nCntSearch++;
		}
		if (m_nCntSearch >= m_State.m_nCntSearchMax)
		{
			m_bUninit = true;
		}
	}
	if (m_bUninit)
	{
		Uninit();
	}
}

//=============================================================================
//描画
//=============================================================================
void CMissile::Draw()
{
}
////=============================================================================
//ミサイル発射
//=============================================================================
void CMissile::LaunchMissile()
{
	
}
////=============================================================================
//プレイヤーに近い敵を探す
//=============================================================================
void CMissile::SearchEnemy(const D3DXVECTOR3 EnemyPos, const float fRadius)
{
	CPlayer *pPlayer = CManager::GetGame()->GetPlayer();

	D3DXVECTOR3 PlayerVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PlayerVec = EnemyPos - pPlayer->GetPos();			//敵と弾のベクトル
	float fLength = sqrtf((PlayerVec.z*PlayerVec.z) + (PlayerVec.x*PlayerVec.x));
	if (fLength <= m_State.m_SearchRange + fRadius)
	{
		//敵の位置を保存
		m_pEnemyPos[m_nCntSearch] = EnemyPos;
		CPresetEffect::SetEffect3D(11, m_pEnemyPos[m_nCntSearch], {}, D3DXVECTOR3(MissileSize, 10.0f, MissileSize), true);
		m_nCntSearch++;
	}
	//else
	//{
	//	//ヒットエフェクト
	//	std::random_device random;	// 非決定的な乱数生成器
	//	std::mt19937_64 mt(random());            // メルセンヌ・ツイスタの64ビット版、引数は初期シード
	//	std::uniform_real_distribution<> randAng(-D3DX_PI, D3DX_PI);

	//	m_pEnemyPos[m_nCntSearch] = { pPlayer->GetPos().x+cosf(randAng(mt))* RandomRange ,0.0f,pPlayer->GetPos().z+sinf(randAng(mt))* RandomRange };
	//	CPresetEffect::SetEffect3D(11, m_pEnemyPos[m_nCntSearch], {}, D3DXVECTOR3(MissileSize, 10.0f, MissileSize), true);
	//	m_nCntSearch++;

	//}
}
//=============================================================================
//クリエイト
//=============================================================================
CMissile * CMissile::Create()
{
	CMissile *pMissile = new CMissile;
	if (pMissile)
	{
		pMissile->Init();
	}

	return pMissile;
}

