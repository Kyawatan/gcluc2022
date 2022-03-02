#include <windows.h>
#include "Main.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define APP_NAME L"モトクロス"

// グローバル変数
Main* g_pMain = NULL;

// エントリー関数
INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ INT)
{
	g_pMain = new Main;
	if (g_pMain != NULL)
	{
		// ウィンドウ生成とDirectX初期化、あとはアプリ終了までループ処理
		Kawata::MainLoop(hInstance, WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME);
		delete g_pMain;
	}
	return 0;
}

void App()
{
	g_pMain->App(); // アプリケーション処理に飛ぶ
}