//=============================================================================
// シーン処理 [player.h]
// Author : 佐藤秀亮
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "scene3d.h"

class CKeyboard;
class CMouse;
class CModel;
class CCamera;

class CPlayer : public CScene3D
{
	//モーションの種類
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,	//待機
		MOTIONTYPE_RUN,	//走る
		MOTIONTYPE_ACTION,	//攻撃（アクション）
		MOTIONTYPE_JUMP,	//ジャンプ
		MOTIONTYPE_LANDING,	//着地
		MOTIONTYPE_RETURN, //ニュートラルモーションに戻る際のモーション。txtファイルに書く必要はない
		MAX_MOTION
	} MOTIONTYPE;

	//キーの構造体
	typedef struct
	{
		float fPosX; //位置X
		float fPosY; //位置Y
		float fPosZ; //位置Z
		float fRotX; //向きX
		float fRotY; //向きY
		float fRotZ; //向きZ
	} KEY;

	//キー情報の構造体
	typedef struct
	{
		int nFrame; //フレーム
		KEY aKey[32]; //各モデルのキー要素 []内はパーツの最大数
	} KEY_INFO;

	//モーション情報の構造体
	typedef struct
	{
		bool bLoop; //ループするかどうか
		int nNumKey; //キーの総数
		int nNonOpe;
		KEY_INFO aKeyInfo[16]; //キー情報 []内はキーの最大数
	} MOTION_INFO;

	//出したいエフェクトの種類
	typedef enum
	{
		TYPE_TRAJECT,
		TYPE_MAX,
	}EFFECT_TYPE;

	typedef struct
	{
		int CreateModel;
		float Size;
		float AddSize;
		D3DCOLORVALUE Color;
		D3DCOLORVALUE ChangeColor;
		int Life;
	} TRAJECT_STATE;

public:
	CPlayer(int nPriority);
	~CPlayer();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char *aFileName);
	void Uninit();
	void Update();
	void Draw();

	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char *aFileName);

	void CreateEffect(int Pattern);
private:
	void Load(const char *aFileName);
	void Move(float fRotY);
	void SlackMove(float fRotY);
	void Stop();
	void UnderSearch(D3DXVECTOR3 pos);
	bool Land(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld);

	void Motion();
	void MotionChange(MOTIONTYPE motionType);

	D3DXVECTOR3 m_PosOld;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_rotDesh;
	D3DXVECTOR3 m_RestartPos;
	float m_fRot;
	float m_fMove;
	float m_fGravity;
	float m_fJump;
	float m_fHeight;
	bool m_bJump;
	bool m_bSlack;
	bool m_bSlackMove;
	bool m_bLandObject;
	int m_nNumModel;
	MOTIONTYPE m_motionType;
	int m_nCntMotion;
	MOTION_INFO m_aMotionInfo[MAX_MOTION];
	MOTION_INFO m_MotionInfo;
	int m_nNeutralTime;
	KEY m_aKeyDiff[32];
	int m_nKey;
	CModel *m_pModel[32]; //モデル（パーツ）
	CKeyboard *m_pKeyboard;
	CMouse *m_pMouse;
	CCamera *m_pCamera;

	TRAJECT_STATE m_Traject;	//エフェクトの状態

	bool m_bLanding;		//着地

	//軌跡処理で必要
	D3DXVECTOR3 m_SwordTrajectPos;	//現在先端の場所
	D3DXVECTOR3 m_SwordTrajectPos2;	//現在手元の場所
	D3DXVECTOR3 m_SwordTrajectOldPos;	//1フレーム前の先端の場所
	D3DXVECTOR3 m_SwordTrajectOldPos2;	//1フレーム前の手元の場所

	static bool m_MouseButtenPush;	//ボタン押してるか
	static int m_MousePushTime;	//押してる時間

	static bool m_PushDeley;	//ボタン押してるか
	static int m_DeleyTime;	//押してる時間

};
#endif