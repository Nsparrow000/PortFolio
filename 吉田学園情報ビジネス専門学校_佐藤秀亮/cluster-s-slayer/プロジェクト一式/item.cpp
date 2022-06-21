//--------------------------------------
//アイテムの処理
//--------------------------------------
#include "item.h"
#include "manager.h"
#include "game.h"
#include "player.h"
#include "model.h"
#include "Renderer.h"
#include "billboard.h"
static const float InhaleHitSize = 1000.0f;//吸い込まれる範囲
static const float InhaleSpeed = 11.0f;//吸い込まれる速度
static const int LeaveTimeMin = 5;//出現位置から離れる時間
static const int LeaveTimeMax = 10;//出現位置から離れる時間

//--------------------------------------------
//コンストラクタ
//--------------------------------------------
CItem::CItem(OBJTYPE nPriority) :CScene(nPriority)
{
	m_bIsHit = false;
	m_IsDeath = false;
	m_IsInhale = false;
	m_bIsLeave = true;
	m_nLeaveTimer = 0;
	m_nLeaveTime = 0;
}
//--------------------------------------------
//デストラクタ
//--------------------------------------------
CItem::~CItem()
{
}
//--------------------------------------------
//初期化
//--------------------------------------------
HRESULT CItem::Init(void)
{
	static std::random_device random;	// 非決定的な乱数生成器
	std::mt19937_64 mt(random());            // メルセンヌ・ツイスタの64ビット版、引数は初期シード
	std::uniform_real_distribution<> randAng(0.0f, D3DX_PI);
	std::uniform_real_distribution<> randAng2(0.0f, D3DX_PI);
	std::uniform_real_distribution<> randTime(LeaveTimeMin, LeaveTimeMax);
	//出現位置から離れる計算に使う角度を求める
	m_fLeaveAng = (float)randAng(mt) - (float)randAng2(mt);
	//出現位置から離れる時間
	m_nLeaveTime = randTime(mt);
	return S_OK;
}
//--------------------------------------------
//終了
//--------------------------------------------
void CItem::Uninit(void)
{
	if (pItemObj)
	{
		pItemObj->Uninit();
		pItemObj = nullptr;
	}
	Release();
}
//--------------------------------------------
//更新
//--------------------------------------------
void CItem::Update(void)
{
	//出現位置から離れる処理
	if (m_bIsLeave)
	{
		Leave();
	}
	else
	{
		//プレイヤー情報を取得
		CPlayer *pPlayer = CManager::GetGame()->GetPlayer();
		////プレイヤーに対しての当たり判定
		if (pPlayer)
		{
			float fRadius = pPlayer->GetParts(0)->GetMaxPos().x*3.0f;
			float HitSize = fRadius + InhaleHitSize;
			D3DXVECTOR3 pos = pPlayer->GetPos();
			D3DXVECTOR3 vec = m_pos - pos;
			float Length = sqrtf((vec.x*vec.x) + (vec.z*vec.z));
			//アイテムがプレイヤーに吸い込まれていなかったら&&
			if (!m_IsInhale)
			{
				//吸い込み範囲に入ったら
				if (Length < HitSize)
				{
					//今吸い込まれている判定にする
					m_IsInhale = true;
				}

			}
			else
			{
				//アイテムがプレイヤーに吸い込まれる処理
				Inhale(vec);
			}
			//アイテムがプレイヤーに当たっていなかったら
			if (!m_bIsHit)
			{
				//当たり判定を取得
				m_bIsHit = IsColision(pos, fRadius);
			}
			else
			{
				//アイテムを消す
				m_IsDeath = true;
			}
		}

	}
	//消去判定がオンなら
	if (m_IsDeath)
	{
		Uninit();
	}
}

//--------------------------------------------
//描画
//--------------------------------------------
void CItem::Draw()
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

}
//--------------------------------------------
//当たり判定
//--------------------------------------------
bool CItem::IsColision(const D3DXVECTOR3& HitPos, const float& radius)
{
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//自分と相手のベクトル
	vec = HitPos - m_pos;
	float fLength = 0.0f;
	//相手と自分の距離を求める
	fLength = sqrtf((vec.z*vec.z) + (vec.x*vec.x));
	//当たり判定の大きさを求める
	float fCollisionRadius = m_fRadius + radius;
	//相手と自分の距離が自分の当たり判定の大きさより小さくなったら
	if (fLength <= fCollisionRadius)
	{
		//当たったという判定を返す
		return true;
	}
	return false;
}
//--------------------------------------------
//アイテムがプレイヤーに吸い込まれる処理
//--------------------------------------------
void CItem::Inhale(const D3DXVECTOR3& vec)
{
	//移動する向きを求める
	float MoveAng = atan2(vec.x, vec.z);
	m_pos.x -= sinf(MoveAng)*InhaleSpeed;
	m_pos.z -= cosf(MoveAng)*InhaleSpeed;
	if (pItemObj)
	{
		pItemObj->Setpos(m_pos, { m_fRadius ,m_fRadius ,0.0f});
	}
}
//--------------------------------------------
//出現位置から離れる処理
//--------------------------------------------
void CItem::Leave()
{

	m_pos.x += sinf(m_fLeaveAng)*InhaleSpeed;
	m_pos.z += cosf(m_fLeaveAng)*InhaleSpeed;
	if (pItemObj)
	{
		pItemObj->Setpos(m_pos, { m_fRadius ,m_fRadius ,0.0f });
	}
	m_nLeaveTimer++;
	if (m_nLeaveTimer > m_nLeaveTime)
	{
		m_nLeaveTimer = 0;
		m_bIsLeave = false;
	}
}

