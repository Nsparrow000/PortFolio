#include "input.h"

#define NUM_KEY_MAX (256)//キーの最大数


LPDIRECTINPUT8 g_pInput = NULL;//DirectInputオブジェクトのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;//入力デバイスへのポインタ

//グローバル変数
BYTE g_aKeyState[NUM_KEY_MAX];//キーボードの入力情報（プレス情報）
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];
BYTE g_GetKeyboardRelese[NUM_KEY_MAX];

//初期化処理
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイスの生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDevKeyboard->Acquire();
	return S_OK;
}

//終了処理
void UninitKeyboard(void)
{
	//入力デバイスの開放
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}
	//DirectInputオブジェクトの開放
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntkey;
	//入力デバイスからデータ取得

	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntkey = 0; nCntkey < NUM_KEY_MAX; nCntkey++)
		{
			
			g_aKeyStateTrigger[nCntkey] = (g_aKeyState[nCntkey] ^ aKeyState[nCntkey])& aKeyState[nCntkey];
			g_GetKeyboardRelese[nCntkey] = (g_aKeyState[nCntkey] ^ aKeyState[nCntkey])& g_aKeyState[nCntkey];

			g_aKeyState[nCntkey] = aKeyState[nCntkey];
			
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();
	}
}

bool GetKeyboardTrigger(int nKey)
{
	return(g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

bool GetKeyboardRelese(int nKey)
{
	return(g_GetKeyboardRelese[nKey] & 0x80) ? true : false;
}

bool GetKeyboardPress(int nKey)
{
	return(g_aKeyState[nKey] & 0x80) ? true : false;
}


