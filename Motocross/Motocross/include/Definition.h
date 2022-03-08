/*
	�A�v�������ʒ�`
*/

#pragma once
#include "KawataLib.h"

using namespace Kawata;

#define COURSE_LENGTH 5760	// �R�[�X�S��

/****************************************************
	class
****************************************************/

class SceneGame;

class TaskPlayer;
class TaskBackGround;

class CameraController;


/****************************************************
	enum class
****************************************************/

// �V�[����
enum class E_SceneName
{
	Title,
	Game,
	Result,
};

// �^�X�N��Draw�����i�ԍ��̏��������̂���O�ɗ���j
enum class E_TaskDrawNum
{
	BackGround = 1000,
	Default = 900,
	UI = -1000,
};

// �^�X�N��Layer�ԍ�
enum class E_TaskLayerNum
{
	Player,
};