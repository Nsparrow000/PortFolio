//--------------------------------------
//マネージャーの処理
//--------------------------------------
#ifndef _MANAGER_H
#define _MANAGER_H
#include "main.h"

//前方宣言
class CInputKeyBoard;
class CRenderer;
class CTexture;
class CLight;
class CModel;
class CPlayer;
class CModel_Spawner;
class CTitle;
class CGame;
class CFade;
class CResult;
class CXInput;
class CDirectInput;
class CMouse;
class CSound;
class CPause;
class CXload;
class CGameMenu;
static int ClearTime = 5;

class CManager
{
public:
	typedef enum
	{
		MODE_TITLE=0,			//タイトル画面
		MODE_MENU,				//ゲーム開始前のメニュー画面
		MODE_GAME,				//ゲーム画面
		MODE_RESULT,			//リザルト画面
		MODE_MAX
	}MODE;

	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit();
	void Update();
	void Draw();
	static CXload	 *GetXload() { return m_pXload; }
	static CRenderer *GetRenderer();
	static CInputKeyBoard *GetInputKeyboard();
	static CTexture *GetTexture();
	static CFade *GetFade(){ return m_Fade; }
	static CGame *GetGame(){ return m_pGame; }
	static void SetMode(MODE mode);						//モードの設定
	static MODE GetMode();
	static bool GetPause() { return m_bPause; }
	static void SetPause(bool bPause, bool bStop = false) {
		m_bPause = bPause;
		m_bStop = bStop;
	}
	static void SetGameStop(bool bStop) { m_bStop = bStop; }

	static void SetEnd(bool bEnd)							{ m_bEnd = bEnd;}
	static CXInput *GetXInput()							{ return m_XInput; }
	static CMouse *GetMouse()							{ return m_Mouse; }
	static CSound *GetSound()							{ return m_pSound; }
	static CDirectInput *GetDirectInput()				{ return m_pDirectInput; }
	static bool GetClear()								{ return m_bClear; }
	static void SetClear(bool bClear)						{ m_bClear = bClear; }
	static HWND GethWnd() { return m_hWnd; }
	static int GetGameTimeMinut() { return m_nGameTimeMinut; }
	static int GetGameTimeSecond() { return m_nGameTimeSecond; }
	static int GetKilledEnemyCount() { return m_nKilledEnemyCount; }
	static void SetKilledEnemyCount(int nCnt) { m_nKilledEnemyCount = nCnt; }
	static void SetGameTimeMinut(int nTime) { m_nGameTimeMinut = nTime; }
	static void SetGameTimeSecond(int nTime) { m_nGameTimeSecond = nTime; }

private:
	static HWND m_hWnd;//このプログラムが使っているウィンドウのハンドル
	static CRenderer		*m_pRenderer;
	static CInputKeyBoard	*m_pInputKeyboard;
	static CXInput			*m_XInput;
	static CTexture			*m_pTexture;
	static bool				m_bStop;
	static bool				m_bPause;
	static bool				m_bEnd;
	static CLight			*m_pLight[3];
	static CPlayer			*m_pPlayer;
	static CModel_Spawner	*m_pModel;
	static CTitle			*m_pTitle;
	static CGameMenu		*m_pGameMenu;
	static CGame			*m_pGame;
	static CResult			*m_pResult;
	static MODE				m_Mode;				//現在のモード
	static CFade			*m_Fade;
	static CMouse			*m_Mouse;
	static CDirectInput		*m_pDirectInput;
	static CSound			*m_pSound;
	static CPause			*m_pPause;
	static CXload			*m_pXload;			//Xファイルの読み込みクラス
	static bool	m_bClear;					//クリアしたかの判定
	static float m_fTimer;
	static int m_nGameTimeMinut;//分数
	static int m_nGameTimeSecond;//秒数

	static int m_nKilledEnemyCount;

};


#endif //_MANAGER_H