//=============================================-
//プレイヤーのクラス
//=============================================
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "character.h"
#define MAX_ASSISTCRYSTAL (2)
#define MAX_HEAL_LEVEL (5)
class CEnemy;
class CSword;
class CGuard;
class CMissile;
class CAssistCrystal;
class CAssistCrystal_Model;

class CPlayer : public CCharacter
{
public:
	enum N_MOTION
	{
		N_NEUTRAL = 0,
		N_MOVE,
		N_ATTACK_1,
		N_ATTACK_2,
		N_ATTACK_3,
		N_MAGIC_ATTACK,//魔法攻撃
		N_SKILL002,
		N_DETH,
		N_DODGE,
		N_MAX
	};//通常状態のモーション

	enum COMBO
	{
		COMBOWAIT = 0,
		COMBO_1,
		COMBO_2,
		COMBO_3,
		COMBO_MAX
	};
	enum MOVEARROW
	{
		MOVEUP = 1,
		MOVEDOWN,
		MOVERIGHT,
		MOVELEFT
	};
	enum SkillType
	{
		ATKup,
		Heal,
		OverHeal,
		Sheild,
		Beam,
		BlackHole,
		Rocket,
		RushAttack,
		Skill_Max
	};

	CPlayer(OBJTYPE nPriority = CScene::OBJTYPE_NONE);
	~CPlayer();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static CPlayer *Create();
	void KeyboardMove();//移動
	void PadMove();//移動
	void Attack();//通常攻撃
	void AttackCtrl();//通常攻撃の操作
	void MouseCameraCtrl();//マウスのカメラ操作
	void MagicAttack();//魔法攻撃
	void RockOn();//ロックオン
	void Dodge();//回避
	bool IsNear(D3DXVECTOR3 EnemyPos,float Radius);
	void NearEnemyFace();//近い敵の方向にプレイヤーを向かせる
	void LevelUp(int nType);//レベルアップの処理
	void HitDamege(int nPower);
	int GetLevel() { return m_nLevel; }
	float GetMaxExp() { return m_fMaxExp; }
	D3DXVECTOR3 GetNearEnemyPos();
	bool IsNearEnemyState();//今何の敵が近くにいるかを算出
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	bool GetCanRushAttack() { return m_bCanRushAttack; }
	int GetSkillLevel(int nSkillType);
private:
	void WeaponSet(const char *pcFileName);//武器のセット
	void FollowingPlayerCamera();//カメラがプレイヤーに追従する処理
	bool PushDifferentMoveKey(int nKey);//違う方向キーを押しているかの判定
	void Drawtext();
	bool IsMoveKeyCheck();//移動用のキーを押しているかの判定を返す処理
	bool FixedTimeInterval(float fMaxTime);//一定時間間隔をあける処理
	void PlayerRushAttack();//追撃
	void EachSkillManager();//各スキルの処理のまとめ
	CSword *m_pSword;//剣
	CGuard *m_pDamegeGuard;
	CMissile *m_pMissile;
	CAssistCrystal *m_pAssistCrystal[MAX_ASSISTCRYSTAL];

	POINT m_Cursol;
	void AttackMove(float fMoveVolume);
	CEnemy *m_pNearEnemy;//プレイヤーから近い敵の情報
	bool m_bNearEnemy;//敵が近くにいるのかの判定
	bool m_nMovePush;//移動キーを押しているかの判定
	bool m_bCanAttack;//攻撃ができるか？
	bool m_bCanAttackMove;//攻撃ができるか？
	bool m_bRevertCamera;
	bool m_bMoveStop;//移動を制限する
	bool m_bAttackWait;//最大コンボから最初のコンボに戻るときの待機のフラグ
	bool m_bAttackReception;//攻撃操作受付
	bool m_bCanAutoHeel;//オートヒール可能か
	bool m_bCanRushAttack;//追撃可能か
	bool m_bCanBlackHole;
	bool m_bCanDamegeGuard;
	bool m_bCanAssistCrystal;
	bool m_bCanDamege;
	bool m_bCanMissile;

	float m_fAttackWaitTime;//攻撃操作待機時間
	float m_fAttackMoveTime;//攻撃移動する時間
	float m_fSoundInterval;
	float m_fStopTime;//止まっている時間
	float m_fPower;//攻撃力
	float m_fPowerDiameter;//攻撃力増加倍率
	float m_fAutoHeel;//オートヒール量
	float m_fMaxExpDiameter;//最大経験値の倍率
	float m_fMaxExp;//最大経験値
	int m_BlackHoleLevel;
	int m_ATKupLevel;
	int m_HealLevel;
	int m_OverHealLevel;
	int m_SheildLevel;
	int m_BeamLevel;
	int m_RocketLevel;
	int m_RushAttackLevel;
	int m_nMissileCnt;
	int m_nRushStartCnt;//追撃開始までのカウント
	int m_nBlackHoleCnt;
	int m_nComboType;//今どのコンボかを数える
	int m_motionType;//モーションの種類
	int m_motionLastType;//前のモーションの種類
	int m_nNumGuard;//今つけてるダメージガードの数
	int m_nLevel;//レベル
	int m_nTimer;

};

#endif // !_PLAYER_H_

