#pragma once
#include <list>
#include "MyQueue.h"
#include <process.h>


const long  MAX_ITASKLEN   = 1000;

class Itask
{
public:
	Itask(void){}
	virtual~Itask(void){}
public:
	virtual void RunItask() = 0;
};



class CMyThreadpool
{
public:
	CMyThreadpool(void);
	~CMyThreadpool(void);
public:
	 //1.
	bool CreateThreadPool(long lMinThreadNum,long lMaxThreadNum,long lItaskNum = MAX_ITASKLEN );
	//2.
	void DestroyThreadPool();
	//3.
	static unsigned _stdcall ThreadProc( void * );
	//4.
	bool Push(Itask *pItask);

private:
	std::list<HANDLE> m_lstThread;
	//std::queue<Itask*> m_qItask;
	MyQueue<Itask*> m_qItask;
	bool  m_bFlagQuit;
	HANDLE m_hSemphore;
	long m_lCreateThreadNum;
	long m_lMaxThreadNum;
	/*volatile*/ long m_lRunThreadNum;
	//MyLock  m_lock;

};

