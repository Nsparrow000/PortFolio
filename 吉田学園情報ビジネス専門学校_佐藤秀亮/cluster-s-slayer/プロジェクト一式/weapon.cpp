//=============================================
//武器のクラス
//=============================================
#include "weapon.h"
#include "manager.h"
#include "Renderer.h"
#include "model.h"
#include "Particle.h"

//---------------------------------------------
//コンストラクタ
//---------------------------------------------
CWeapon::CWeapon()
{
	m_bWeaponDraw = true;
	m_bIsHit = false;
	m_bCanHitCollision = false;
	m_pWeapon = nullptr;
	m_fPower = 0.0f;
}

//-----------------------------------------------
//デストラクタ
//---------------------------------------------
CWeapon::~CWeapon()
{
}

//-----------------------------------------------
//初期化
//---------------------------------------------
HRESULT CWeapon::Init()
{
	return S_OK;
}

//-----------------------------------------------
//終了
//---------------------------------------------
void CWeapon::Uninit()
{
	if (m_pWeapon)
	{
		m_pWeapon->Uninit();
		m_pWeapon = nullptr;
	}
}

//-----------------------------------------------
//更新
//---------------------------------------------
void CWeapon::Update()
{
	//攻撃当たり判定がオンなら
	if (m_bCanHitCollision)
	{
		//当たり判定の位置の確認
		//CParticle *pParticle = new CParticle;
		//pParticle->PlayRandomCircle({ m_HitCollision.m_mtxWorld._41, m_HitCollision.m_mtxWorld._42, m_HitCollision.m_mtxWorld._43 }, CParticle::CIRCLEPARTICLE);

	}
}

//-----------------------------------------------
//描画
//---------------------------------------------
void CWeapon::Draw()
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

	//モデルの描画
	if (m_bWeaponDraw)
	{
		m_pWeapon->Draw();
	}

	//攻撃当たり判定の情報設定
	if (m_bCanHitCollision)
	{
		HItCollisionSet();
	}

	//前回の位置情報の保存
	m_lastpos = m_pos;

}
//-----------------------------------------------
//当たり判定の位置情報を設定
//--------------------------------------------
void CWeapon::HItCollisionSet()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ

	D3DXMATRIX mtxRotModel, mtxTransModel;//計算用マトリックス
	D3DXMATRIX mtxParent;//親のマトリックス

	//各パーツのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_HitCollision.m_mtxWorld);

	//向きを設定
	D3DXMatrixRotationYawPitchRoll(&mtxRotModel, m_HitCollision.m_rot.y, m_HitCollision.m_rot.x, m_HitCollision.m_rot.z);

	//向きを反映
	D3DXMatrixMultiply(&m_HitCollision.m_mtxWorld, &m_HitCollision.m_mtxWorld, &mtxRotModel);

	//位置を設定
	D3DXMatrixTranslation(&mtxTransModel, m_HitCollision.m_pos.x, m_HitCollision.m_pos.y, m_HitCollision.m_pos.z);

	//位置を反映
	D3DXMatrixMultiply(&m_HitCollision.m_mtxWorld, &m_HitCollision.m_mtxWorld, &mtxTransModel);

	//当たり判定に武器のマトリックスを設定
	if (m_pWeapon != nullptr)
	{
		mtxParent = m_pWeapon->GetMatrix();
	}
	else
	{
		//最新のマトリックスを取得
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	//算出した各パーツのワールドマトリックスと親のマトリックスを掛け合わせる
	D3DXMatrixMultiply(&m_HitCollision.m_mtxWorld,
		&m_HitCollision.m_mtxWorld,
		&mtxParent);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_HitCollision.m_mtxWorld);
}
//-----------------------------------------------
//当たり判定の位置情報を設定（当たった側の位置情報、当たった側の当たり判定の大きさ）
//--------------------------------------------
bool CWeapon::HitSet(D3DXVECTOR3 * Hitpos, const float& HitSize)
{
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//自分と相手のベクトル
	vec = *Hitpos - m_HitCollision.m_pos;
	float fLength = 0.0f;
	//相手と自分の距離を求める
	fLength = sqrtf((vec.z*vec.z) + (vec.x*vec.x));
	//当たり判定の大きさを求める
	float fCollisionRadius = m_fHitSize + HitSize;
	//相手と自分の距離が自分の当たり判定の大きさより小さくなったら
	if (fLength <= fCollisionRadius)
	{
		//当たったという判定を返す
		return true;
	}
	return false;
}
