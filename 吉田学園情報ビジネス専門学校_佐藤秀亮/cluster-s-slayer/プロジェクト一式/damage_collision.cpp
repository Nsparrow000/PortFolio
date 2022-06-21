//=============================================
//swordのクラス
//=============================================
#include "damage_collision.h"
#include "manager.h"
#include "Renderer.h"
#include "model.h"
#include "enemy.h"
#include "effect.h"
#include "game.h"
#include "player.h"
#include "Particle.h"
//---------------------------------------------
//コンストラクタ
//---------------------------------------------
CDamage_Collision::CDamage_Collision()
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_lastpos = { 0.0f,0.0f,0.0f };
	m_rot = { 0.0f,0.0f,0.0f };
	m_fSize = 0.0f;
}

//-----------------------------------------------
//デストラクタ
//---------------------------------------------
CDamage_Collision::~CDamage_Collision()
{
}

//-----------------------------------------------
//初期化
//---------------------------------------------
HRESULT CDamage_Collision::Init()
{

	return S_OK;
}

//-----------------------------------------------
//終了
//---------------------------------------------
void CDamage_Collision::Uninit()
{

}

//-----------------------------------------------
//更新
//---------------------------------------------
void CDamage_Collision::Update()
{
}

//-----------------------------------------------
//描画
//---------------------------------------------
void CDamage_Collision::Draw()
{
	//前回の位置情報の保存
	m_lastpos = { m_mtxWorld._41,m_mtxWorld._42 ,m_mtxWorld._43 };

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ

	D3DXMATRIX mtxRotModel, mtxTransModel;//計算用マトリックス
	D3DXMATRIX mtxParent;//親のマトリックス

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

	//当たり判定に武器のマトリックスを設定
	if (m_pParent != nullptr)
	{
		mtxParent = m_pParent->GetMatrix();
	}
	else
	{
		//最新のマトリックスを取得
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	//算出した各パーツのワールドマトリックスと親のマトリックスを掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);


}
//----------------------------------------------
//インスタンス生成
//----------------------------------------------
CDamage_Collision * CDamage_Collision::Create(const D3DXVECTOR3 pos,const float fSize,CModel *pParent)
{
	CDamage_Collision *pSword = nullptr;
	pSword = new CDamage_Collision;
	pSword->m_pos = pos;
	pSword->m_fSize = fSize;
	pSword->m_pParent = pParent;
	pSword->Init();

	return pSword;
}
//----------------------------------------------
//当たり判定
//----------------------------------------------
bool CDamage_Collision::IsCollision(const D3DXVECTOR3 & Hitpos, const float & fRadius)
{
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//自分と相手のベクトル
	D3DXVECTOR3 lastvec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//自分と相手のベクトル

	D3DXVECTOR3 pos = { m_mtxWorld._41,m_mtxWorld._42 ,m_mtxWorld._43 };
	vec = pos - Hitpos;
	lastvec = m_lastpos - Hitpos;
	float fLength = 0.0f;
	float fLastLength = 0.0f;

	//相手と自分の距離を求める
	fLength = sqrtf((vec.z*vec.z) + (vec.x*vec.x));
	fLastLength = sqrtf((lastvec.z*lastvec.z) + (lastvec.x*lastvec.x));
	float fCollisionRadius = m_fSize + fRadius;
	//相手と自分の距離が自分の当たり判定の大きさより小さくなったら
	if (fLength <= fCollisionRadius /*&&fLastLength > fCollisionRadius*/)
	{
		//当たったという判定を返す
		return true;
	}
	return false;
}
