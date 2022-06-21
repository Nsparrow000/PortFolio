//--------------------------------------------
//敵の出現設定処理
//作成者：林
//--------------------------------------------
#include "enemy_spawn_manager.h"
#include "enemy001.h"
#include "manager.h"
static const int SpawnTime = 600;
static const float SpawnLengthMax = 1000.0f;
static const float SpawnLengthMin = 50.0f;

static const int SpawnMax = 20;
static const int SpawnLimit = 350;

//--------------------------------------------
//コンストラクタ
//--------------------------------------------
CEnemySpawnManager::CEnemySpawnManager(OBJTYPE nPriority):CScene(nPriority)
{
	m_nSpawnTime = SpawnTime;
	m_nSpawnCnt = SpawnTime;
}
//--------------------------------------------
//デストラクタ
//--------------------------------------------
CEnemySpawnManager::~CEnemySpawnManager()
{
}
//--------------------------------------------
//初期化
//--------------------------------------------
HRESULT CEnemySpawnManager::Init(void)
{
	return S_OK;
}
//--------------------------------------------
//終了
//--------------------------------------------
void CEnemySpawnManager::Uninit(void)
{
	Release();
}
//--------------------------------------------
//更新
//--------------------------------------------
void CEnemySpawnManager::Update(void)
{
	//スポーンするまでの時間をカウント
	bool bPause = CManager::GetPause();
	if (!bPause)
	{
		m_nSpawnCnt++;
		int nMaxEnemy = CEnemy::GetMaxEnemy();
		if (m_nSpawnCnt >= m_nSpawnTime && nMaxEnemy <= SpawnLimit)
		{

			//最大値行くまで敵を生成
			for (int nCnt = 0; nCnt < SpawnMax; nCnt++)
			{
				static std::random_device random;	// 非決定的な乱数生成器
				std::mt19937_64 mt(random());            // メルセンヌ・ツイスタの64ビット版、引数は初期シード
				std::uniform_real_distribution<> randAng(0.0f, D3DX_PI);
				std::uniform_real_distribution<> randAng2(0.0f, D3DX_PI);
				std::uniform_real_distribution<> randLength(SpawnLengthMin, SpawnLengthMax);
				float fAng = randAng(mt) - randAng2(mt);
				float fLength = randLength(mt);
				//スポーン地点を計算
				D3DXVECTOR3 SpawnPos =
				{ sinf(fAng)*fLength ,0.0f,cosf(fAng)*fLength };
				CEnemy001::Create(m_pos + SpawnPos, { 0.0f,0.0f,0.0f });
			}
			m_nSpawnCnt = 0;
		}

	}
}

//--------------------------------------------
//描画
//--------------------------------------------
void CEnemySpawnManager::Draw()
{
}

//----------------------------------------------
//インスタンス生成
//----------------------------------------------
CEnemySpawnManager *CEnemySpawnManager::Create(const D3DXVECTOR3& pos)
{
	CEnemySpawnManager *pEnemy = NULL;
	pEnemy = new CEnemySpawnManager;
	pEnemy->m_pos = pos;
	pEnemy->Init();
	return pEnemy;
}