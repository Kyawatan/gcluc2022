#include "QTEUIManager.h"


QTEUIManager::QTEUIManager()
	: m_pTaskKeyList(NULL)
{

}

QTEUIManager::~QTEUIManager()
{

}

void QTEUIManager::SetInputKeyList(std::list<int> iInputKeyList)
{
	// �L�[�O���t�B�b�N�𐶐�����
	for (auto itr = iInputKeyList.begin(); itr != iInputKeyList.end();)
	{
		TaskBase* key = new TaskKey((*itr));
		m_pTaskKeyList.push_back(key);
		++itr;
	}
	// Now, Next, NextNext��\��
	auto itr = m_pTaskKeyList.begin();
	dynamic_cast<TaskKey*>(*itr)->FadeIn(E_KeyPossition::Now);
	++itr;
	dynamic_cast<TaskKey*>(*itr)->FadeIn(E_KeyPossition::Next);
	++itr;
	dynamic_cast<TaskKey*>(*itr)->FadeIn(E_KeyPossition::NextNext);
}

void QTEUIManager::SetNextInputKey()
{
	// Now�L�[���폜
	auto itr = m_pTaskKeyList.begin();
	dynamic_cast<TaskKey*>(*itr)->SuccessEffect();
	// Next�������now�Ɉړ�
	if (2 <= m_pTaskKeyList.size())
	{
		++itr;
		dynamic_cast<TaskKey*>(*itr)->TranslatetoNext();
	}
	// NextNext�������Next�Ɉړ�
	if (3 <= m_pTaskKeyList.size())
	{
		++itr;
		dynamic_cast<TaskKey*>(*itr)->TranslatetoNext();
	}
	// �܂����̓L�[�������NextNext�ɕ\��
	if (4 <= m_pTaskKeyList.size())
	{
		++itr;
		dynamic_cast<TaskKey*>(*itr)->FadeIn(E_KeyPossition::NextNext);
	}
	m_pTaskKeyList.pop_front();
}

void QTEUIManager::DeleteInputKeyList()
{
	for (auto itr = m_pTaskKeyList.begin(); itr != m_pTaskKeyList.end();)
	{
		dynamic_cast<TaskKey*>(*itr)->FadeOut();
		++itr;
	}
	m_pTaskKeyList.clear();
}

void QTEUIManager::SetFailure()
{
	auto itr = m_pTaskKeyList.begin();
	dynamic_cast<TaskKey*>(*itr)->FailureEffect();
}