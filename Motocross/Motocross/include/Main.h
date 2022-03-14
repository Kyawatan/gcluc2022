/*
	�Q�[���S�̂̊Ǘ��A�V�[���Ǘ�
*/

#pragma once
#include "Definition.h"

// �V�[����
enum class E_SceneName
{
	Title,
	Game,
	Result,
};

class Main
{
public:
	Main();
	~Main();

	void App();		// �A�v���P�[�V��������

private:
	void Setup();	// ����������
	void Update();	// �A�b�v�f�[�g����
	void (Main::* m_pFunc)(); // �֐��|�C���^
	void SetFunc(void (Main::* pFunc)()) { m_pFunc = pFunc; } // ���s����֐����Z�b�g����

	E_SceneName m_nowScene;
	SceneBase* m_pScene;
};

