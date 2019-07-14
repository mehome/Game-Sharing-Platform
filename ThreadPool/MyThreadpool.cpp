#include "MyThreadpool.h"


CMyThreadpool::CMyThreadpool(void):m_lCreateThreadNum(0),m_lMaxThreadNum(0)
	,m_lRunThreadNum(0)
{
	m_bFlagQuit = true;
	
}


CMyThreadpool::~CMyThreadpool(void)
{

}



bool CMyThreadpool::CreateThreadPool(long lMinThreadNum,long lMaxThreadNum,long lItaskNum)
{
	if(lMinThreadNum <=0 || lMaxThreadNum <lMinThreadNum)return false;

	//创建线程中所需的资源
	m_hSemphore = CreateSemaphore(NULL,0,lMaxThreadNum,NULL);
	if(lItaskNum <100)lItaskNum = 100;

	if(!m_qItask.InitQueue(lItaskNum/5))
	{
		return false;

	}

	//创建线程
	for(long i =0; i < lMinThreadNum;i++)
	{
		HANDLE hThread = (HANDLE)_beginthreadex(0,0,&ThreadProc,this,0,0);

		if(hThread)
			m_lstThread.push_back(hThread);
	}
	m_lMaxThreadNum = lMaxThreadNum;
	m_lCreateThreadNum = lMinThreadNum;
	return true;
}

unsigned _stdcall CMyThreadpool::ThreadProc( void * lpvoid )
{
	CMyThreadpool *pthis = (CMyThreadpool*)lpvoid;
	Itask *pItask = NULL;
	while(pthis->m_bFlagQuit)
	{
		
		if(WAIT_TIMEOUT == WaitForSingleObject(pthis->m_hSemphore,100))
			continue;
		//睡眠状态----工作状态
	//	pthis->m_lRunThreadNum++;
		InterlockedIncrement(&pthis->m_lRunThreadNum);

	    
		while(pthis->m_qItask.pop(pItask))
		{
			pItask->RunItask();
			delete pItask;
			pItask = NULL;
		}
	
		
		//工作状态----睡眠状态
		//pthis->m_lRunThreadNum--;
		InterlockedDecrement(&pthis->m_lRunThreadNum);

	}
	return 0;
}



void CMyThreadpool::DestroyThreadPool()
{
	m_bFlagQuit = false;
	auto ite = m_lstThread.begin();
	while(ite != m_lstThread.end())
	{
		if(WAIT_TIMEOUT == WaitForSingleObject(*ite,100))
			TerminateThread(*ite,-1);

		CloseHandle(*ite);
		*ite = NULL;
		ite++;
	}

	//遍历队列--
	Itask *pItask = NULL;
	while(m_qItask.pop(pItask))
	{
		delete pItask;
		pItask = NULL;
	}
		

	//销毁队列
	m_qItask.UnInitQueue();


}
bool CMyThreadpool::Push(Itask *pItask)
{
	if(!pItask)return false;


	 while(!m_qItask.push(pItask))
	 {
		 Sleep(100);
	 }


	if(m_lRunThreadNum < m_lCreateThreadNum)
	{
		ReleaseSemaphore(m_hSemphore,1,NULL);
	}
	else if(m_lCreateThreadNum < m_lMaxThreadNum)
	{
		HANDLE hThread = (HANDLE)_beginthreadex(0,0,&ThreadProc,this,0,0);

		if(hThread)
			m_lstThread.push_back(hThread);
		m_lCreateThreadNum++;
		ReleaseSemaphore(m_hSemphore,1,NULL);
	}
	else 
	{
		//。。。。。
	}
	


	return true;
}

