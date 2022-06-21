//--------------------------------------------
//敵の出現設定処理
//作成者：林
//--------------------------------------------
#ifndef _SKILL_LEVELDATA_H_
#define _SKILL_LEVELDATA_H_
#include "main.h"
#include "blackhole.h"
#include "assistcrystal.h"
#include "rushattack.h"
#include "guard.h"
#include "missile.h"
class CModel;

class CSkill_LevelData
{
public:

	CSkill_LevelData();
	~CSkill_LevelData();
	HRESULT Init();
	void Uninit();
	//静的メンバー関数
	static CSkill_LevelData *Create();
	static void RushAttackLoad(const char * sFileName);//パーツの設定
	static void AssistCrystalLoad(const char * sFileName);//パーツの設定
	static void BlackHoleLoad(const char * sFileName);//パーツの設定
	static void GuardLoad(const char * sFileName);//パーツの設定
	static void MissileLoad(const char * sFileName);//パーツの設定

	static CBlackHole::State GetStateBlackHole(int nLevel) { return m_BlackHoleState[nLevel]; }
	static CAssistCrystal::State GetStateAssistCrystal(int nLevel) { return m_AssistCrystalState[nLevel]; }
	static CRushAttack::State GetStateRushAttack(int nLevel) { return m_RushAttackState[nLevel]; }
	static CGuard::State GetStateGuard(int nLevel) {return m_GuardState[nLevel];}
	static CMissile::State GetStateMissile(int nLevel) { return m_MissileState[nLevel]; }

private:
	static CBlackHole::State m_BlackHoleState[CBlackHole::Level_MAX + 1];
	static CAssistCrystal::State m_AssistCrystalState[CAssistCrystal::Level_MAX + 1];
	static CRushAttack::State m_RushAttackState[CRushAttack::Level_MAX + 1];
	static CGuard::State m_GuardState[CGuard::Level_MAX + 1];
	static CMissile::State m_MissileState[CMissile::Level_MAX + 1];
};


#endif // !_PLAYER_H_

