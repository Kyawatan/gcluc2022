#include "Main.h"

// �R���X�g���N�^
Main::Main()
	: m_pFunc(&Main::Setup)
{
	
}

// �f�X�g���N�^
Main::~Main()
{

}

// �A�v���P�[�V��������
void Main::App()
{
	(this->*m_pFunc)();
}

// �Z�b�g�A�b�v����
void Main::Setup()
{
	this->SetFunc(&Main::Update);
}

// �A�b�v�f�[�g����
void Main::Update()
{

}