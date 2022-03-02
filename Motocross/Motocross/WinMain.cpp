#include <windows.h>
#include "Main.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define APP_NAME L"���g�N���X"

// �O���[�o���ϐ�
Main* g_pMain = NULL;

// �G���g���[�֐�
INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ INT)
{
	g_pMain = new Main;
	if (g_pMain != NULL)
	{
		// �E�B���h�E������DirectX�������A���Ƃ̓A�v���I���܂Ń��[�v����
		Kawata::MainLoop(hInstance, WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME);
		delete g_pMain;
	}
	return 0;
}

void App()
{
	g_pMain->App(); // �A�v���P�[�V���������ɔ��
}