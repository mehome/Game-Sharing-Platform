#include <Windows.h>

class MyLock
{
public:
	MyLock()
	{
		InitializeCriticalSection(&m_cs);
	}
	~MyLock()
	{
		DeleteCriticalSection(&m_cs);
	}
	void Lock()
	{
		EnterCriticalSection(&m_cs);
	}
	void UnLock()
	{
		LeaveCriticalSection(&m_cs);
	}
private:
	CRITICAL_SECTION m_cs;
};