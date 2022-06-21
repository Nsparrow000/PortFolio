//-------------------------------------------
//アシストクリスタルの処理
//------------------------------------------
#ifndef _ASSISTCRYSTAL_H_
#define _ASSISTCRYSTAL_H_
#include "scene.h"
class CPlayer;
class CAssistCrystal_Model;
class CAssistCrystal 
{
public:
	enum Level
	{
		Level_1 = 0,
		Level_2,
		Level_3,
		Level_4,
		Level_MAX
	};
	struct State
	{
		int m_nPower;
		int m_nLaunchInterval;//弾の発射間隔
		float m_fMoveSpeed;
	};
	CAssistCrystal();
	~CAssistCrystal();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	//静的メンバー関数
	static CAssistCrystal *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CPlayer *pPlayer,const int& nLevel);
	void PlayerFollowing();
private:
	D3DXVECTOR3 m_pos;//位置
	D3DXVECTOR3 m_rot;//向き
	D3DXVECTOR3 m_scale;//大きさ
	D3DXMATRIX m_mtxWorld;//ワールドマトリックス
	CPlayer *m_pPlayer;
	CAssistCrystal_Model *m_CrystalModel;
	D3DXMATRIX	m_mtxParent;						//ワールドマトリックス
	float m_fInstallationAngle;//アシストクリスタルの設置場所
	int m_nDeleteTimer;
	int m_nLevel;
	bool m_bUninit;
};

#endif // !_ASSISTCRYSTAL_H_

