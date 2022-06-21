//=============================================
//武器のクラス
//=============================================
#ifndef _WEAPON_H_
#define _WEAPON_H_
#include "main.h"
class CModel;

class CWeapon
{
public:
	struct HitCollision
	{
		D3DXVECTOR3 m_pos;//位置
		D3DXVECTOR3 m_rot;//向き
		D3DXMATRIX m_mtxWorld;//ワールドマトリックス
	};
	CWeapon();
	~CWeapon();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void HItCollisionSet();
	void SetCanHit(const bool& bCanHit) { m_bCanHitCollision = bCanHit; }//武器の当たり判定を発生させるかの設定
	bool GetCanHit() { return m_bCanHitCollision; }//武器の当たり判定を発生させるかの設定
	void SetPower(float fPower) { m_fPower = fPower; }
	//当たった側の位置情報と当たった側の当たり判定の大きさ
	bool HitSet(D3DXVECTOR3 *Hitpos,const float& HitSize);//武器の当たり判定の設定

private:
protected:
	HitCollision m_HitCollision;//当たり判定
	D3DXVECTOR3 m_pos;//位置
	D3DXVECTOR3 m_lastpos;//1フレーム前の位置
	D3DXVECTOR3 m_rot;//向き
	D3DXMATRIX m_mtxWorld;//ワールドマトリックス
	CModel*m_pWeapon;//武器のモデル
	CModel *m_pParent;//親モデルのポインタ
	float m_fHitSize;//当たり判定の大きさ
	float m_fPower;
	bool m_bWeaponDraw;//武器の描画設定
	bool m_bCanHitCollision;//攻撃判定ができるか
	bool m_bIsHit;//今武器が敵に当たっているか
};


#endif // !_WEAPON_H_
