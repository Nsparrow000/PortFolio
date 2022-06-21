//--------------------------------------
//アイテムの処理
//制作者：林
//--------------------------------------
#ifndef _ITEM_H_
#define _ITEM_H_
#include "scene.h"

class CSound;
class CBillboard;

class CItem : public CScene
{
public:

	CItem(OBJTYPE nPriority = CScene::OBJTYPE_ITEM);
	~CItem();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	bool IsColision(const D3DXVECTOR3& HitPos, const float& radius);
	void Inhale(const D3DXVECTOR3& vec);//アイテムがプレイヤーに吸い込まれる処理
	void Leave();//出現位置から離れる処理
private:
protected:
	D3DXVECTOR3 m_pos;//位置
	D3DXVECTOR3 m_lastpos;//1フレーム前の位置
	D3DXVECTOR3 m_rot;//向き
	D3DXMATRIX m_mtxWorld;//ワールドマトリックス
	CBillboard * pItemObj;//アイテムの画像
	float m_fRadius;//当たり判定の範囲
	float m_fLeaveAng;//出現位置から離れる計算に使う角度
	int m_nLeaveTimer;//カウンター
	int m_nLeaveTime;//出現位置から離れる時間
	bool m_bIsLeave;//出現位置から離れる判定
	bool m_bIsHit;//当たり判定
	bool m_IsInhale;//吸い込まれるかの判定
	bool m_IsDeath;//死亡判定

};


#endif // !_PLAYER_H_

