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
	// キーグラフィックを生成する
	for (auto itr = iInputKeyList.begin(); itr != iInputKeyList.end();)
	{
		TaskBase* key = new TaskKey((*itr));
		m_pTaskKeyList.push_back(key);
		++itr;
	}
	// Now, Next, NextNextを表示
	auto itr = m_pTaskKeyList.begin();
	dynamic_cast<TaskKey*>(*itr)->FadeIn(E_KeyPossition::Now);
	++itr;
	dynamic_cast<TaskKey*>(*itr)->FadeIn(E_KeyPossition::Next);
	++itr;
	dynamic_cast<TaskKey*>(*itr)->FadeIn(E_KeyPossition::NextNext);
}

void QTEUIManager::SetNextInputKey()
{
	// Nowキーを削除
	auto itr = m_pTaskKeyList.begin();
	dynamic_cast<TaskKey*>(*itr)->SuccessEffect();
	// Nextがあればnowに移動
	if (2 <= m_pTaskKeyList.size())
	{
		++itr;
		dynamic_cast<TaskKey*>(*itr)->TranslatetoNext();
	}
	// NextNextがあればNextに移動
	if (3 <= m_pTaskKeyList.size())
	{
		++itr;
		dynamic_cast<TaskKey*>(*itr)->TranslatetoNext();
	}
	// まだ入力キーがあればNextNextに表示
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