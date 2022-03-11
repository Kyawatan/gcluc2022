#pragma once
#include <list>
#include "Definition.h"
#include "TaskKey.h"


class QTEUIManager
{
public:
	QTEUIManager();
	~QTEUIManager();

	// ���̓L�[�ԍ���S�Ď󂯎��A�L�[UI�𐶐��E�\��
	void SetInputKeyList(std::list<int> iInputKeyList);

	// �L�[UI�iNow, Next, NextNext�j�����̃L�[�ɍX�V
	void SetNextInputKey();

	// ���X�g�Ɏc���Ă���L�[�����ׂč폜
	void DeleteInputKeyList();

	// ���̓~�X����Now�L�[��Ԃ�����
	void SetFailure();

private:
	std::list<TaskBase*> m_pTaskKeyList;
};