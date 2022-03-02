#include "Main.h"

// コンストラクタ
Main::Main()
	: m_pFunc(&Main::Setup)
{
	
}

// デストラクタ
Main::~Main()
{

}

// アプリケーション処理
void Main::App()
{
	(this->*m_pFunc)();
}

// セットアップ処理
void Main::Setup()
{
	this->SetFunc(&Main::Update);
}

// アップデート処理
void Main::Update()
{

}