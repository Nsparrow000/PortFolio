//=============================================-
//キャラクターのクラス
//=============================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "scene.h"
class CModel;
class CLayer_Structure;
class CMotion;
class CShadow;
class CCharacter : public CScene
{
public:
	typedef enum
	{
		IDLE = 0,//アイドル状態
		MOVE,//移動状態
		ATTACK,//攻撃状態
		DAMEGE,//ダメージを受ける状態
	}state_type;

	CCharacter(OBJTYPE nPriority);
	~CCharacter();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	bool IsCollision(D3DXVECTOR3 *pMyPos, const D3DXVECTOR3& Hitpos, const float& fRadius, const float& MoveSpeed);//当たり判定
	void IsModelCollision(const bool& bPlayer);
	void HPChange(float fNum);//体力の増減
	float GetLife() { return m_fHitPoint; }
	void StateChange(state_type StateType);//状態の変化
	void SetDeath(bool Uninit) { m_IsDeath = Uninit; }
	bool GetDeath(void) { return m_IsDeath; }
	D3DXMATRIX GetMtx() { return m_mtxWorld; }
	D3DXVECTOR3 GetRot() { return m_rot; }
	CModel *GetParts(int nPartsNum);//キャラクターのパーツ取得処理
	std::vector <CModel*> GetParts() { return m_pParts; }
protected:

	D3DXVECTOR3 m_pos;//位置
	D3DXVECTOR3 m_lastpos;//1フレーム前の位置
	D3DXVECTOR3 m_rot;//向き
	D3DXMATRIX m_mtxWorld;//ワールドマトリックス
	state_type m_StateType;//状態の種類
	CLayer_Structure *m_pLayer;
	CMotion *m_pMotion;//モーション
	std::vector <CModel*> m_pParts;//キャラクターのパーツ
	CShadow *m_pShadow;//影

	int m_nAttackType;//攻撃の種類
	int m_nPartsMax;//パーツの最大数
	float m_fHitPoint;//体力
	float m_fRadius;//当たり判定の範囲
	float m_fMoveSpeed;
	bool m_IsDeath;//死亡判定
	bool m_IsStop;//動作の停止判定
	bool m_IsCharacterDraw;//キャラクターの描画判定
	bool m_IsDelay;//動作の遅延
};

#endif // !_CHARACTER_H_