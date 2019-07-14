
#include "MyLock.h"

template<class T>
class MyQueue
{
public:
	MyQueue()
	{
		m_queue = NULL;
		m_lWritePos = 0;
		m_lReadPos = 0;
		m_llen = 0;
	}
	~MyQueue()
	{}
public:
	//1.初始化队列
	 bool InitQueue(int nlen)
	 {
		 if(nlen <=0)return false;


		 m_llen = nlen;
		 m_queue = new T[nlen];

		 for(int i =0; i <nlen;i++)
			 m_queue[i] = NULL;

		// ZeroMemory(m_queue,sizeof(T)*nlen);
		 return true;
	 }
	//2.销毁队列
	 void UnInitQueue()
	 {

		 if(m_queue)
		 {
			 delete[] m_queue;
			 m_queue = NULL;
		 }
	 }
	//3.push
	 bool push(T& node)
	 {
		  m_Writelock.Lock();
		 if(m_queue[m_lWritePos] != NULL)
		 {
			 m_Writelock.UnLock();
			 return false;
		 }
			 

		
		 m_queue[m_lWritePos] = node;
		 m_lWritePos = (m_lWritePos+1)%m_llen;
		 m_Writelock.UnLock();
		 return true;
	 }
	//4.pop--front
	 bool pop(T& node)
	 {
		  m_Readlock.Lock();
		 if(m_queue[m_lReadPos] == NULL)
		 {
			  m_Readlock.UnLock();
			  return false;
		 }
			

		
		 node = m_queue[m_lReadPos];
		 m_queue[m_lReadPos] = NULL;
		 m_lReadPos = (m_lReadPos+1)%m_llen;
		 m_Readlock.UnLock();
		 return true;
	 }

	//5.empty
	 bool empty()
	 {
		 if(m_lWritePos ==m_lReadPos && m_queue[m_lWritePos]  == NULL)
			 return true;

		 return false;
	 }

private:
	T* m_queue;
	long m_lWritePos;
	long m_lReadPos;
	long m_llen;
	MyLock m_Writelock;
	MyLock m_Readlock;

};