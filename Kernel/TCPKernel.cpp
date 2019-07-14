#include"TCPKernel.h"
#include<stdlib.h>
#include<math.h>
#include<time.h>


void VedioItask::RunItask()
{
	//������Է�����
	HANDLE hary[] = {m_pv->m_hSemaphore,m_pvr->m_hEvent};
	DWORD  dwIndex;
	while(1)
	{
		//���źŻ��¼�
		//����ȵ��ź��˾����� ����ȵ��¼��˾ͽ���
		dwIndex = WaitForMultipleObjects(2,hary,FALSE,INFINITE);
		dwIndex -=  WAIT_OBJECT_0;

		if(dwIndex == 0) //�Լ���0
		{
			//���������Լ��Ĺ���
			m_pKernel->m_pTcpNet->SendData(m_pvr->m_sock,
				m_pv->m_szAuthorBuffer+m_pvr->m_noffset*_DEF_VIDEOSIZE,_DEF_VIDEOSIZE);
			m_pvr->m_noffset = (m_pvr->m_noffset+1)%_DEF_BUFFERNUM;
		}
		else if(dwIndex == 1) //������1
			break;
	}
}
int roomnum = 7;
void GetTeamTask::RunItask()
{
	int num=0;
	while(1)
	{
		//�ж�ƥ�������û���Լ�
			int m_num=0;
			auto ite = m_pKernel->m_mapleveltoPlayerQueue[m_gp->level].begin();
			while(ite != m_pKernel->m_mapleveltoPlayerQueue[m_gp->level].end())
			{

				if(strncmp((*ite)->m_szName,m_gp->m_szName,_DEF_SIZE) != 0)
				{
					m_num++;
				}
				if(num == m_pKernel->m_mapleveltoPlayerQueue[m_gp->level].size())
				{
					m_gp->isget=true;
					break;
					//���û�� ��֤���Լ��ڱ��˵Ĵ�ƥ����У���ֻ����ȴ�ƥ��ɹ�
				}
				ite ++;
			}
			
			if(m_gp->isget == false)//����У���֤���Լ�û���ڱ��˵Ĵ�ƥ����У�
			{
				//�͹ؼ��Σ����߳���ƥ�����ƥ�䣬���ƥ�䵽�ˣ��ͷ��ڴ�ƥ�����,����ƥ�����ɾ����Ȼ�����
				m_pKernel->my_lock.Lock();

				auto ite = m_pKernel->m_mapleveltoPlayerQueue[m_gp->level].begin();
				while(ite != m_pKernel->m_mapleveltoPlayerQueue[m_gp->level].end())
				{
		
					if(m_pKernel->m_mapleveltoPlayerQueue[m_gp->level].size() > 1)
					{
						if(strncmp((*ite)->m_szName,m_gp->m_szName,_DEFSIZE) != 0)
						{
							//���뵽��ƥ�����
							m_pKernel->m_mapGetTeamPlayer[m_gp].push_back((*ite));
							//��ƥ�����ɾ��
							m_pKernel->m_mapleveltoPlayerQueue[m_gp->level].erase(ite);
							num++;
							if(num == 5)//���Ѽ����Լ��Ѿ����﷿������߿��Խ�����Ϸ
								break;
						}
					
					}
					ite ++;
				}
				
				if(num < 5)
				{
						auto ite = m_pKernel->m_mapleveltoPlayerQueue[m_gp->level+1].begin();
						while(ite != m_pKernel->m_mapleveltoPlayerQueue[m_gp->level+1].end())
						{
		
							if(m_pKernel->m_mapleveltoPlayerQueue[m_gp->level+1].size() > 1)
							{
								if(strncmp((*ite)->m_szName,m_gp->m_szName,_DEFSIZE) != 0)
								{
									//���뵽��ƥ�����
									m_pKernel->m_mapGetTeamPlayer[m_gp].push_back((*ite));
									//��ƥ�����ɾ��
									m_pKernel->m_mapleveltoPlayerQueue[m_gp->level+1].erase(ite);
									num++;
									if(num == 5)
										break;
								}
							
							}
							ite ++;
						}
						
						if(m_gp->level >=2) //������һ������ң���ʼ�ȼ�Ϊ1������Ҫ���ڵ��ڶ�
						{
										auto ite = m_pKernel->m_mapleveltoPlayerQueue[m_gp->level-1].begin();
										while(ite != m_pKernel->m_mapleveltoPlayerQueue[m_gp->level-1].end())
										{
		
									if(m_pKernel->m_mapleveltoPlayerQueue[m_gp->level-1].size() > 1)
									{
										if(strncmp((*ite)->m_szName,m_gp->m_szName,_DEFSIZE) != 0)
										{
											//���뵽��ƥ�����
											m_pKernel->m_mapGetTeamPlayer[m_gp].push_back((*ite));
											//��ƥ�����ɾ��
											m_pKernel->m_mapleveltoPlayerQueue[m_gp->level-1].erase(ite);
											num++;
											if(num == 5)
												break;
										}
									
									}
									ite ++;
								}
						}
				}
				m_pKernel->my_lock.UnLock();
				

				STRU_GETTEAM_RS sgr;
				sgr.m_nType=_DEF_PROTOCOL_GETTEAM_RS;
				for(int i=6;i<_DEF_ROOMNUM;i++)
				{
					if(m_pKernel->m_listRoom[i].m_curplayer == 0)
					{
						sgr.m_roomid=i;
						break;
					}

				}
				//�ڴ�ƥ�����ɾ��ͬʱ���Լ���Map��ɾ��
			   //���ͬ�ȼ��Լ����µȼ��Ķ���������û���ҵ�6���� ��ôֻҪ��ǰ�������������˾Ϳ�ʼ��Ϸ
				auto ite8 = m_pKernel->m_mapGetTeamPlayer[m_gp].begin();
				while(ite8 != m_pKernel->m_mapGetTeamPlayer[m_gp].end())
				{
				
					if(m_pKernel->m_mapGetTeamPlayer[m_gp].size() >= 1)
					{
						sgr.szResult=true;
					}
					else
					{
						sgr.szResult=false;
					}
					m_pKernel->m_pTcpNet->SendData((*ite8)->m_sock,(char*)&sgr,sizeof(sgr));

					//
					auto itemap = m_pKernel->m_mapGetTeamPlayer.begin();
					while(itemap != m_pKernel->m_mapGetTeamPlayer.end())
					{
						if(strncmp(itemap->first->m_szName,(*ite8)->m_szName,_DEFSIZE) == 0)
						{
							auto itemap1=m_pKernel->m_mapGetTeamPlayer[itemap->first].begin();
							while(itemap1!=m_pKernel->m_mapGetTeamPlayer[itemap->first].end())
							{
								m_pKernel->m_mapGetTeamPlayer[itemap->first].erase(itemap1);
							}
							m_pKernel->m_mapGetTeamPlayer.erase(itemap);
							itemap++;
							break;
						}
						itemap++;
					
					}
					
					ite8++;
					
				}
				m_pKernel->m_pTcpNet->SendData(m_gp->m_sock,(char*)&sgr,sizeof(sgr));
				m_pKernel->m_mapGetTeamPlayer.erase(m_gp);
				
			}
			//�жϴ�ƥ������Ƿ�С�������� �����С����������Ϳ�ʼ��Ϸ

	
	}
	
}
TCPKernel::TCPKernel()
{
	m_pTcpNet = new TCPNet(this);

	//���������˵Ĵ���ÿ������������Ϣ�������ʼ��
	for(int i=0;i<_DEF_ROOMNUM;i++)
	{
		m_listRoom[i].m_hSemaphore = NULL;
		m_listRoom[i].m_roomid=i+1;
		m_listRoom[i].m_curplayer=0;
		m_listRoom[i].m_maxplayer=6;
		m_listRoom[i].m_max=0;
		memset(m_listRoom[i].answer,0,sizeof(m_listRoom[i].answer));
		for(int j=0;j<_DEF_ROOMNUM;j++)
		{
			m_listRoom[i].issit[j] = false;
			m_listRoom[i].sitidc[j] = 0;
		}
		m_listRoom[i].start_flag = false;
		m_listRoom[i].m_curReadyplayer = 0;
		m_listRoom[i].m_winplayer = 0;

	}
}

TCPKernel::~TCPKernel()
{
	if(m_pTcpNet)
	{
		delete m_pTcpNet;
		m_pTcpNet = NULL;
	}
}


bool TCPKernel::Open()
{
	if(!m_sql.ConnectMySql("localhost","root","alei21.","1027"))
		return false;
	if(!m_pTcpNet->InitNetWork())
		return false;
	if(!m_threadpool.CreateThreadPool(10,1000))
		return false;
	if(!m_getteamthreadpool.CreateThreadPool(100,1000))
		return false;
	return true;
}


void TCPKernel::Close()
{
	m_pTcpNet->UnInitNetWork();
	m_threadpool.DestroyThreadPool();
	m_getteamthreadpool.DestroyThreadPool();

}

//----------------------------------------------------------------------------------------
#define _BEGIN_PROTOCOL_MAP  static void (TCPKernel:: * _PMArray[_DEF_PACK_COUNT])(SOCKET sock,char* szbuf);\
class PMclass\
{\
public: \
	static PMclass* create()\
	{\
		

#define _END_PROTOCOL_MAP return 0;\
	}\
	static PMclass* st ;\
};\
PMclass* PMclass::st=PMclass::create();
#define PM(x,y)   _PMArray[  ( (x) - _DEF_PROTOCOL_BASE )] = (y);

//Э��ӳ���
_BEGIN_PROTOCOL_MAP
	PM(_DEF_PROTOCOL_REGISTER_RQ,&TCPKernel::RegisterRq) //ע��
	PM(_DEF_PROTOCOL_LOGIN_RQ,&TCPKernel::LoginRq) //��¼
	PM(_DEF_PROTOCOL_ONLINE_RQ,&TCPKernel::SendOnlinePlayerListRq) //�����б�����
	PM(_DEF_PROTOCOL_QUIT_RQ,&TCPKernel::QuitRq) //�˳�
	PM(_DEF_PROTOCOL_CHAT_RQ,&TCPKernel::ChatRq) //����
	PM(_DEF_PROTOCOL_GETROOM_RQ,&TCPKernel::GetRoomRq) //������Ϣ��ȡ����
	PM(_DEF_PROTOCOL_QUITROOM_RQ,&TCPKernel::QuitRoomRq) //�˳���������
	PM(_DEF_PROTOCOL_CHATROOM_RQ,&TCPKernel::ChatRoomRq) //��������
	PM(_DEF_PROTOCOL_GETPOINT_RQ,&TCPKernel::GetPointRq) //�յ�
	PM(_DEF_PROTOCOL_START_RQ,&TCPKernel::StartRq) //��ʼ��Ϸ
	PM(_DEF_PROTOCOL_READY_RQ,&TCPKernel::ReadyRq) //׼�� ȡ��׼��
	PM(_DEF_PROTOCOL_INSERTPERSON_RQ,&TCPKernel::InsertPersonRq) //ע��ɹ���ӵ�person����
	PM(_DEF_PROTOCOL_ALTERPERSON_RQ,&TCPKernel::AlterPersonRq) //�༭������Ϣ
	PM(_DEF_PROTOCOL_GETFRIEND_RQ,&TCPKernel::GetFriendRq) //��Ӻ�������
	PM(_DEF_PROTOCOL_GETFRIEND_RS,&TCPKernel::GetFriendRs) //��Ӻ��ѻظ�
	PM(_DEF_PROTOCOL_LOOKPERSON_RQ,&TCPKernel::LookPersonRq) //�鿴�������
	PM(_DEF_PROTOCOL_DELETEFRIEND_RQ,&TCPKernel::DeleteFriendRq) //��Ӻ��ѻظ�
	PM(_DEF_PROTOCOL_PERSONCHAT_RQ,&TCPKernel::PersonChatRq) //˽��
	PM(_DEF_PROTOCOL_INVITEPLAYER_RQ,&TCPKernel::InvitePlayerRq) //������Ϸ
	PM(_DEF_PROTOCOL_INVITEPLAYERYN_RQ,&TCPKernel::InvitePlayerYnRq) //�ظ������������ܻ�ܾ�
	PM(_DEF_PROTOCOL_INVITEVEDIO_RQ,&TCPKernel::InviteVedioRq) //��Ƶ
	PM(_DEF_PROTOCOL_INVITEVEDIO_RS,&TCPKernel::InviteVedioRs)
	PM(_DEF_PROTOCOL_VIDEOSTREAMINFO_RQ,&TCPKernel::VedioStreamInfoRq)
	PM(_DEF_PROTOCOL_VIDEOSTREAM_RQ,&TCPKernel::VedioStreamInfoRq)
	PM(_DEF_PROTOCOL_SENDFILE_RQ,&TCPKernel::SendFileRq) //�����ļ�ͷ
	PM(_DEF_PROTOCOL_SENDFILEBLOCK_RQ,&TCPKernel::SendFileBlockRq) //�����ļ���
	PM(_DEF_PROTOCOL_GETTEAM_RQ,&TCPKernel::GetTeamRq) //�����ļ���
_END_PROTOCOL_MAP

//--------------------------------------------------------------------------------------------

void TCPKernel::DealData(SOCKET sock,char* szbuf)
{
	//�������� -> Э��ӳ���
	//��������---Э��ӳ���
	
	//��� Ȼ����Э��ͷ, Ȼ�����deal��������
	char iType = *(char*)szbuf;
	//�ж��Ƿ���������
	PFUN pf = 0 ;
	if( iType - _DEF_PROTOCOL_BASE >= 0 && iType - _DEF_PROTOCOL_BASE < _DEF_PACK_COUNT)
	pf = _PMArray[iType - _DEF_PROTOCOL_BASE];
	//�ж��Ƿ�Ϊ�� �ǿ� �ſ��Ե���
	if(pf)
	((*this).*pf)(sock,szbuf);

	
}


//����ע�������
void TCPKernel::RegisterRq(SOCKET sock,char* szbuf)
{
	STRU_REGISTER_RQ *psrr = (STRU_REGISTER_RQ *)szbuf;
	char szsql[_DEF_SQLLEN] = {0};
	
	STRU_REGISTER_RS srr;
	srr.m_nType = _DEF_PROTOCOL_REGISTER_RS;
	memcpy(srr.m_szName,psrr->m_szName,_DEF_SIZE);
	srr.m_tel = psrr->m_tel;
	
	list<string> lststr;
	
	//������ݿ����Ƿ���ڴ���
	sprintf_s(szsql,"SELECT p_id FROM player where p_id = %lld or a_name = '%s'" ,psrr->m_tel,psrr->m_szName);
	m_sql.SelectMySql(szsql,1,lststr);

	if(lststr.size() ==0)
	{
		srr.m_szResult = _register_success;
		//�����˲��뵽���ݿ���
		sprintf_s(szsql,"insert into player (p_id,p_name,p_pw) values(%lld,'%s','%s')",psrr->m_tel,psrr->m_szName,psrr->m_szPassword);
		m_sql.UpdateMySql(szsql);
	}

	m_pTcpNet->SendData(sock,(char*)&srr,sizeof(srr));
}


//���ܵ�¼����
void TCPKernel::LoginRq(SOCKET sock,char* szbuf)
{
	//1 2  3
	//�����ݿ��в���
	//��������
	STRU_LOGIN_RQ *pslr = (STRU_LOGIN_RQ*)szbuf;
	char szsql[_DEF_SQLLEN] = {0};
	list<string> lststr;
	
	STRU_LOGIN_RS slr;
	slr.m_nType = _DEF_PROTOCOL_LOGIN_RS;
	memcpy(slr.m_szName,pslr->m_szName,_DEF_SIZE);
	slr.m_szResult = _login_usernoexists;
	
		
	sprintf_s(szsql,"SELECT p_pw FROM player where  p_name = '%s'" ,pslr->m_szName);
	m_sql.SelectMySql(szsql,1,lststr);
	if(lststr.size() >0)
	{
		slr.m_szResult = _login_passworderr;

		string strpassword = lststr.front();
		lststr.pop_front();

		if(0 == strcmp(strpassword.c_str(),pslr->m_szPassword))
		{
			slr.m_szResult = _login_success;

			//��¼�û���Ϣ  �Ա㷢�͵�ǰ������Ϣ
			Player_Info *pInfo = new Player_Info;
			pInfo->m_hEvent = WSACreateEvent();
			pInfo->m_sock = sock;
			m_lstPlayer.push_back(pInfo);
			m_mapSocketToPlayerInfo[sock] = pInfo;

			//���뵱ǰ���������������
			OnlinePlayerServer* p = (OnlinePlayerServer*)malloc(sizeof(OnlinePlayerServer));
			memcpy(p->m_szName,pslr->m_szName,_DEF_SIZE);
			p->sock = sock;
				
			int flag = 0; //û���ҵ���֮ƥ����������
			auto ite = m_lstOnlinePlayer.begin();
			while(ite != m_lstOnlinePlayer.end())
			{
				//����������Ѿ����������¼��ҵ�������ô�Ͳ�����push��
				if(strcmp((*ite)->m_szName,p->m_szName) == 0)
				{
					flag = 1;
					break;
				}
				ite ++;
			}
			if(flag == 0)
				m_lstOnlinePlayer.push_back(p);

			for(int p=0;p<_DEF_PLAYER;p++)
			{
				slr.sit[p] = 0;
			}

			int num=0;
			for(int p=0;p<6;p++)
			{
				for(int q=0;q<6;q++)
				{
					slr.sit[num++] = m_listRoom[p].sitidc[q];
				}
			}
		}
	}
	
	m_pTcpNet->SendData(sock,(char*)&slr,sizeof(slr));

	//�����ݿ��в������к��ѷ����ͻ���
	STRU_GETLISTFRIEND_RQ *psgr = (STRU_GETLISTFRIEND_RQ*)szbuf;
	STRU_GETLISTFRIEND_RS sgr;
	strcpy(sgr.m_szName,psgr->m_szName);
	sgr.m_nType=_DEF_PROTOCOL_GETLISTFRIEND_RS;
	list<string> lstint;
	sprintf_s(szsql,"select u_id from player where p_name='%s'",sgr.m_szName);
	m_sql.SelectMySql(szsql,1,lstint);
	int u_id = atoi(lstint.front().c_str());

	list<string> friendlist;
	sprintf_s(szsql,"select u_friendid from friend where u_id = %d",u_id);
	m_sql.SelectMySql(szsql,1,friendlist);

	list<string> friendname;
	auto ite=friendlist.begin();
	while( ite!=friendlist.end())
	{
		int friend_id = atoi(friendlist.front().c_str());

		sprintf_s(szsql,"select p_name from player where u_id = %d",friend_id);
		ite++;
		friendlist.pop_front();
		m_sql.SelectMySql(szsql,1,friendname);
		strcpy(sgr.m_friendName,friendname.front().c_str());
		friendname.pop_front();
		m_pTcpNet->SendData(sock,(char*)&sgr,sizeof(sgr));
	
	}

}


//����ǰ��������������ҷ���ǰ����
void TCPKernel::SendOnlinePlayerListRq(SOCKET sock,char* szbuf)
{
	//���������
	STRU_SHOWPLAYER_RQ * pssr = (STRU_SHOWPLAYER_RQ*)szbuf;

	STRU_SHOWPLAYER_RS ssr ;/*(STRU_SHOWPLAYER_RS*)malloc(sizeof(STRU_SHOWPLAYER_RS))*/ /*new STRU_SHOWPLAYER_RS*/;
	ssr.m_nType = _DEF_PROTOCOL_ONLINE_RS;

	list<OnlinePlayerServer*>::iterator ite = m_lstOnlinePlayer.begin();
	int i=0;
	while(ite != m_lstOnlinePlayer.end())
	{
		memcpy(ssr.online[i].m_szName,(*ite)->m_szName,_DEF_SIZE);
		//ssr.online[i].sock = (*ite)->sock;
		i++;
		ite ++;
	}

	for(i;i<_DEF_PLAYER;i++)
	{
		memset(ssr.online[i].m_szName,0,sizeof(ssr.online[i].m_szName));
		//ssr.online[i].sock = 0;
	}
	list<OnlinePlayerServer*>::iterator ite1 = m_lstOnlinePlayer.begin();
	while(ite1!=m_lstOnlinePlayer.end())
	{
	  int len = sizeof(ssr);
	  m_pTcpNet->SendData((*ite1)->sock,(char*)&ssr,len);
	  ite1++;
	}
}


//�˳�
void TCPKernel::QuitRq(SOCKET sock,char* szbuf)
{
	STRU_QUIT_RQ * sqr = (STRU_QUIT_RQ*)szbuf;

	STRU_QUIT_RS sqrr;

	sqrr.m_nType = _DEF_PROTOCOL_QUIT_RS;
	sqrr.szResult = _quit_fail;

	auto ite = m_lstOnlinePlayer.begin();
	while(ite != m_lstOnlinePlayer.end())
	{
		if(strcmp((*ite)->m_szName,sqr->m_szName) == 0)
		{
			m_lstOnlinePlayer.erase(ite);
			sqrr.szResult = _quit_success;
			break;
		}
		else ite ++;
	}
	m_pTcpNet->SendData(sock,(char*)&sqrr,sizeof(sqrr));
	
	//���������������
	STRU_SHOWPLAYER_RS ssr ;/*(STRU_SHOWPLAYER_RS*)malloc(sizeof(STRU_SHOWPLAYER_RS))*/ /*new STRU_SHOWPLAYER_RS*/;
	ssr.m_nType = _DEF_PROTOCOL_ONLINE_RS;

	list<OnlinePlayerServer*>::iterator ite1 = m_lstOnlinePlayer.begin();
	int i=0;
	while(ite1 != m_lstOnlinePlayer.end())
	{
		memcpy(ssr.online[i].m_szName,(*ite1)->m_szName,_DEF_SIZE);
		//ssr.online[i].sock = (*ite)->sock;
		i++;
		ite1 ++;
	}

	for(i;i<_DEF_PLAYER;i++)
	{
		memset(ssr.online[i].m_szName,0,sizeof(ssr.online[i].m_szName));
		//ssr.online[i].sock = 0;
	}
	list<OnlinePlayerServer*>::iterator ite2 = m_lstOnlinePlayer.begin();
	while(ite2!=m_lstOnlinePlayer.end())
	{
	  int len = sizeof(ssr);
	  m_pTcpNet->SendData((*ite2)->sock,(char*)&ssr,len);
	  ite2++;
	}

	
}


//��������
void TCPKernel::ChatRq(SOCKET sock,char* szbuf)
{
	STRU_CHAT_RQ* scr = (STRU_CHAT_RQ*)szbuf;
	
	STRU_CHAT_RS scrr;
	scrr.m_nType = _DEF_PROTOCOL_CHAT_RS;
	strcpy_s(scrr.m_szName,scr->m_szName);
	strcpy_s(scrr.m_chatbuf,scr->m_chatbuf);

	auto ite = m_lstOnlinePlayer.begin();
	while(ite != m_lstOnlinePlayer.end())
	{
		m_pTcpNet->SendData((*ite)->sock,(char*)&scrr,sizeof(scrr));
		ite ++;
	}
}



//���ܷ�����Ϣ��ȡ����
void TCPKernel::GetRoomRq(SOCKET sock,char* szbuf)
{
	STRU_GETINTOROOM_RQ* psgr=(STRU_GETINTOROOM_RQ*)szbuf;

	//����ظ���Э���
	STRU_GETINTOROOM_RS sgr;
	sgr.m_nType=_DEF_PROTOCOL_GETROOM_RS;
	sgr.m_roomid = psgr->m_roomid;
	sgr.m_sitid = psgr->m_sitid;
	sgr.m_buttonid = psgr->m_buttonid;
	
	if(m_listRoom[psgr->m_roomid-1].start_flag == true)
	{
		sgr.szResult = _getroominfo_fail;
	}

	//��ʼ��Ϸ������ʼ��
	auto ite0 = m_listRoom[sgr.m_roomid-1].m_listInRoomPlayer.begin();
	while(ite0 != m_listRoom[sgr.m_roomid-1].m_listInRoomPlayer.end())
	{
		ite0->m_flag=0;
		ite0++;
	}

	
	//��ӵ��������˵Ĵ���ÿ���������������������
	int i = (psgr->m_roomid) -1;
	//�жϵ�ǰ����������������û�г����������
	if(m_listRoom[i].m_maxplayer > m_listRoom[i].m_curplayer)
	{
		//���������ӵĶ�Ӧ��λ��Ϊfalse ˵��û���� ������
		if(m_listRoom[i].issit[psgr->m_sitid-1] == false)
		{
			//����λ��־��Ϊtrue
			m_listRoom[i].issit[psgr->m_sitid-1] = true;
			m_listRoom[i].sitidc[psgr->m_sitid-1] = psgr->m_buttonid;
			m_listRoom[i].m_curplayer ++;

			InRoomPlayerServer p;
			strcpy_s(p.m_szName,psgr->m_playername);
			p.sock = sock;
			p.m_flag = 0;
			p.m_score = 0;
			m_listRoom[i].m_listInRoomPlayer.push_back(p);
			//�ɹ����� 
			sgr.szResult = _getroominfo_success;

			//���ͻ����� �����Ǵ������ߵ�������Ҷ������յ�
			STRU_GETSIT_RS sgrrr;
			sgrrr.m_nType = _DEF_PROTOCOL_GETSIT_RS;
			sgrrr.buttonid = psgr->m_buttonid;
			sgrrr.flag = true;
			auto ite = m_lstOnlinePlayer.begin();
			while(ite != m_lstOnlinePlayer.end())
			{
				if((*ite)->m_szName[0] != NULL)
				{
					m_pTcpNet->SendData((*ite)->sock,(char*)&sgrrr,sizeof(sgrrr));
				}
					ite ++;
			}
		}
	
		//�����Ӧ��λ�ŵı�־λΪtrue ������ ���Ϊʧ��
		else 
			sgr.szResult = _getroominfo_fail;
			
	}

	int j = 0;
	for(j;j<_DEF_ROOMNUM;j++)
	{
		memset(sgr.inroom[j].m_szName,0,sizeof(sgr.inroom[j].m_szName));
	}

	j=0;
	auto ite = m_listRoom[i].m_listInRoomPlayer.begin();
	while(ite != m_listRoom[i].m_listInRoomPlayer.end())
	{
		if((*ite).m_szName[0] != NULL)
		{
			strcpy_s(sgr.inroom[j].m_szName,(*ite).m_szName);
			j++;
		}
		ite ++;
	}
	
	if(sgr.szResult == _getroominfo_success)
	{
		ite = m_listRoom[i].m_listInRoomPlayer.begin();
		while(ite != m_listRoom[i].m_listInRoomPlayer.end())
		{
			if((*ite).m_szName[0] != NULL)
			{
				m_pTcpNet->SendData((*ite).sock,(char*)&sgr,sizeof(sgr));
			}
			ite ++;
		}
		//������뷿��ɹ� �������ӵ�����Ϊ���ҵ�map;
		//��ʼ��û�п�ʼ��Ϸ����0 ���������1  �˳���Ϊ2
		string adc;
		adc=psgr->m_playername;
		m_listRoom[psgr->m_roomid-1].m_drawer[adc]=0;
		
	}

	else 
		m_pTcpNet->SendData(sock,(char*)&sgr,sizeof(sgr));
}


//�˳���������
void TCPKernel::QuitRoomRq(SOCKET sock,char* szbuf)
{
	//����Ҫ�˳��ķ���� ���������
	STRU_QUITROOM_RQ* psgr=(STRU_QUITROOM_RQ*)szbuf;
	int i = psgr->m_roomid-1;
	DWORD dw = GetLastError();
	char szsql[_DEF_SQLLEN] = {0};//��SQL�����д洢
	list<string> lststr;//�Դ����ݿ����ҵ��Ľ��д洢
	int p_score;
	bool f;
	//����˳������������Ϸ�е�,��ô���ǽ��������Ϊ���ܣ������ݿ��п۳�50��
	if(m_listRoom[psgr->m_roomid-1].start_flag==true)
	{
		auto ite7 = m_listRoom[psgr->m_roomid-1].m_listInRoomPlayer.begin();
        while(ite7 != m_listRoom[psgr->m_roomid-1].m_listInRoomPlayer.end())
	     {
		   //������ֱȶ���ͬ������ �ͽ�����ҵķ��������ݿ��п۳�50�ֲ���50������
		    if( strcmp((*ite7).m_szName,psgr->m_szName) == 0 )
		     {   
                        memset(szsql,0,_DEF_SIZE);
						sprintf_s(szsql,"SELECT p_score FROM person where  p_name = '%s'" ,ite7->m_szName);
						f=m_sql.SelectMySql(szsql,1,lststr);
				       if(lststr.size()>0)
					   {
                          p_score=atoi(lststr.front().c_str());
						  if(p_score>=50)
						  {   
							  memset(szsql,0,_DEF_SIZE);
							  sprintf_s(szsql,"update person set p_score=p_score-50 where p_name='%s'",ite7->m_szName); 
							  f= m_sql.UpdateMySql(szsql);
							  //�жϵȼ��Ƿ���Ҫ�ı�
							  if(p_score/100!=(p_score-50)/100)
							  {
							     memset(szsql,0,_DEF_SIZE);
							     sprintf_s(szsql,"update person set p_level=p_level-1 where p_name='%s'",ite7->m_szName); 
							     f= m_sql.UpdateMySql(szsql);
							  }
							  else
							  {
								  //���ı�
							  }
						     
						  }
						  else
						  {
							  memset(szsql,0,_DEF_SIZE);
							  sprintf_s(szsql,"update person set p_score=0 where p_name='%s'",ite7->m_szName);
						      f= m_sql.UpdateMySql(szsql);
						  }
						  //����ҵ��ܾ��������޸�
						      memset(szsql,0,_DEF_SIZE);
							  sprintf_s(szsql,"update person set p_total=p_total+1 where p_name='%s'",ite7->m_szName);
						      f= m_sql.UpdateMySql(szsql);
					   }
						
			    }
			        ite7 ++;
		     }
		
	      }

	//����˳��������׼����
	if(psgr->flag_ready == true)
	{
		m_listRoom[psgr->m_roomid-1].m_curReadyplayer--;
		auto itee = m_listRoom[psgr->m_roomid-1].m_drawer.begin();
		{
			char quit_name[_DEF_SIZE] = {0};
			strncpy(quit_name,(*itee).first.c_str(),_DEF_SIZE);
			if(strncmp(quit_name,psgr->m_szName,_DEF_SIZE) == 0)
			{
				m_listRoom[psgr->m_roomid-1].m_drawer.erase(itee);
			}
			else
				itee ++;
		}
	}

	//����ظ�Э���
	STRU_QUITROOM_RS sqr;
	sqr.m_nType = _DEF_PROTOCOL_QUITROOM_RS;
	strncpy(sqr.m_szName,psgr->m_szName,_DEF_SIZE);
	sqr.szResult = _quit_fail;
	int j = 0;
	for(j;j<_DEF_ROOMNUM;j++)
	{
		memset(sqr.inroom[j].m_szName,0,sizeof(sqr.inroom[j].m_szName));
	}

	//���������˵ĸ��������Ӧ�ķ�����ɾ��,�����������Ӧ��map��Ϊ2
	auto ite100 = m_listRoom[psgr->m_roomid-1].m_listInRoomPlayer.begin();
	while(ite100 != m_listRoom[psgr->m_roomid-1].m_listInRoomPlayer.end())
	{
		//������ֱȶ���ͬ������ �Ͱ������Ҵ�������ɾ��
		if( strcmp((*ite100).m_szName,psgr->m_szName) == 0 )
		{   //�����������Ӧ��map��Ϊ 2
			if(m_listRoom[psgr->m_roomid-1].start_flag == true)
				m_listRoom[psgr->m_roomid-1].m_drawer[(*ite100).m_szName]=2;
			else
			{
				auto ite80=m_listRoom[psgr->m_roomid-1].m_drawer.begin();
				while(ite80!=m_listRoom[psgr->m_roomid-1].m_drawer.end())
				{
					if(strncmp((*ite100).m_szName,(*ite80).first.c_str(),_DEF_SIZE) == 0)
					{
						m_listRoom[psgr->m_roomid-1].m_drawer.erase(ite80);
						break;
					}
					ite80++;
				}
			}
			//����Ҵ�������ɾ��
			m_listRoom[psgr->m_roomid-1].m_listInRoomPlayer.erase(ite100);
			sqr.szResult = _quit_success;
			m_listRoom[psgr->m_roomid-1].m_curplayer--;
			if(m_listRoom[psgr->m_roomid-1].m_curplayer == 0)
				m_listRoom[psgr->m_roomid-1].start_flag = false;
			break ;
		}
		ite100 ++;
	}

	//��mapΪ2��ɾ��
	auto ite8=m_listRoom[psgr->m_roomid-1].m_drawer.begin();
	while(ite8!=m_listRoom[psgr->m_roomid-1].m_drawer.end())
	{
		if((*ite8).second==2)
		{
			m_listRoom[psgr->m_roomid-1].m_drawer.erase(ite8++);
			
		}
		else
		{
			ite8++; 
		}
		
	}
	//����������֮��ֻʣ��һ����,�͸��䷢����ҽ�����
	if(m_listRoom[psgr->m_roomid-1].start_flag == true)
	{
		auto ite0=m_listRoom[psgr->m_roomid-1].m_listInRoomPlayer.begin();
		if(m_listRoom[psgr->m_roomid-1].m_drawer.size()==1)
		{
			STRU_START_RS ssr;
			ssr.m_nType = _DEF_PROTOCOL_START_RS;
			ssr.m_curplayer=1;
			ssr.szResult=_start_end;
			m_pTcpNet->SendData((*ite0).sock,(char*)&ssr,sizeof(ssr));
			memset(m_listRoom[psgr->m_roomid-1].answer,0,_DEF_SIZE);
			m_listRoom[psgr->m_roomid-1].m_curReadyplayer = 0;
			auto itee = m_listRoom[psgr->m_roomid-1].m_drawer.begin();
			while( itee != m_listRoom[psgr->m_roomid-1].m_drawer.end())
			{
				(*itee).second=0;
				itee ++;
			}
			m_listRoom[psgr->m_roomid-1].start_flag = false;
		}
	}

	//�ı�������˵ķ����������
	m_listRoom[psgr->m_roomid-1].issit[psgr->sitid-1] = false;
	for(int p=0;p<_DEF_ROOMNUM;p++)
	{
		if(m_listRoom[psgr->m_roomid-1].sitidc[p] == psgr->sitidc)
			m_listRoom[psgr->m_roomid-1].sitidc[p] = 0;
	}

	//���ͻظ�Э���
	//�ظ�Э����Ƿ������ÿ����Ҷ�Ҫ�յ� ������յ����Ϊ_quit_success����ظ�Э��� 
	m_pTcpNet->SendData(sock,(char*)&sqr,sizeof(sqr));

	//�����˳�������յ���Ӧ���Ǹ����б�Э��� ���Ϊ_quit_others
	sqr.szResult = _quit_others;
	
	j=0;
	auto ite = m_listRoom[i].m_listInRoomPlayer.begin();
	while(ite != m_listRoom[i].m_listInRoomPlayer.end())
	{
		if((*ite).m_szName[0] == NULL) break;
		if((*ite).m_szName[0] != NULL)
		{
			strcpy(sqr.inroom[i].m_szName,(*ite).m_szName);
			j++;
		}
		ite ++;
	}

	//���͸������е�ʣ�����
	ite = m_listRoom[i].m_listInRoomPlayer.begin();
	while(ite != m_listRoom[i].m_listInRoomPlayer.end())
	{
		if((*ite).m_szName[0] != NULL)
		{
			m_pTcpNet->SendData((*ite).sock,(char*)&sqr,sizeof(sqr));
		}
		ite ++;
	}

	//֪ͨ������һ���
	STRU_GETSIT_RS sgrrr;
	sgrrr.m_nType = _DEF_PROTOCOL_GETSIT_RS;
	sgrrr.flag = false;
	sgrrr.buttonid = psgr->sitidc;
	auto itee = m_lstOnlinePlayer.begin();
	while(itee != m_lstOnlinePlayer.end())
	{
		if((*itee)->m_szName[0] != NULL)
		{
			m_pTcpNet->SendData((*itee)->sock,(char*)&sgrrr,sizeof(sgrrr));
		}
		itee ++;
	}
}



static int is_first=0; //�Ƿ��ǵ�һ���¶Ե���


//���ܷ�����������
void TCPKernel::ChatRoomRq(SOCKET sock,char* szbuf)
{
	STRU_CHATROOM_RQ* scr = (STRU_CHATROOM_RQ*)szbuf;
	
	STRU_CHATROOM_RS scrr;
	scrr.m_nType = _DEF_PROTOCOL_CHATROOM_RS;
	strcpy(scrr.m_szName,scr->m_szName);
	strcpy(scrr.m_chatbuf,scr->m_chatbuf);
	scrr.m_istrue = false;
	scrr.m_score = 0;
	scrr.m_dscore = 0;
	strcpy(scrr.m_dszName,m_listRoom[scr->m_roomid-1].p);
	int ncount=0;
	for(int j=0;j<_DEF_SIZE;j++)
	{
		if(scr->m_szName[j] != ' ')
		{
			ncount++;
		}
		if(scr->m_szName[j] == ' ')
			break;
	
	}


	//���flag��false û�п�ʼ��Ϸ �������� ֱ�ӷ� scr->flag
	if(m_listRoom[scr->m_roomid-1].start_flag == true) //���������ݽ����жϺͼ���
	{
		//����֮ǰ�����ж��Ƿ�¶�
		if( strcmp(scr->m_chatbuf,m_listRoom[scr->m_roomid-1].answer) == 0 )
		{
			scrr.m_istrue = true;
			int m_ncount;
			if(strlen(m_listRoom[scr->m_roomid-1].p) > ncount)
			{
				m_ncount=strlen(m_listRoom[scr->m_roomid-1].p);
			}
			else
			{
				m_ncount=ncount;
			}

			//������˲¶� ��Ҫ������˼ӷ�
			auto ite2=m_listRoom[scr->m_roomid-1].m_listInRoomPlayer.begin();
			while(ite2!=m_listRoom[scr->m_roomid-1].m_listInRoomPlayer.end())
			{
				//�ҵ�������
				if(strncmp((*ite2).m_szName,m_listRoom[scr->m_roomid-1].p,_DEF_SIZE)==0)
				{
					break;
				}
				ite2++;
			}

			auto ite = m_listRoom[scr->m_roomid-1].m_listInRoomPlayer.begin();
			while(ite != m_listRoom[scr->m_roomid-1].m_listInRoomPlayer.end())
			{
				if((strncmp(scr->m_szName,ite->m_szName,ncount)==0))
				{

				    //��������û�¶Թ� ���Ҳ��ǻ�����
					if((ite->m_flag==0)&&(strncmp(scr->m_szName,m_listRoom[scr->m_roomid-1].p,m_ncount)!=0))
					{
						is_first++;
						//���������ǵ�һ���˲¶�
						if(is_first == 1)
						{
							ite->m_score+=5;
							ite->m_flag=1;

							(*ite2).m_score+=2;

							m_listRoom[scr->m_roomid-1].m_winplayer++;
							
						}
						//���������Ѿ����ǵ�һ���˲¶���
						else if(is_first > 1)
						{
							ite->m_score+=3;
							ite->m_flag=1;

							(*ite2).m_score+=2;

							m_listRoom[scr->m_roomid-1].m_winplayer++;

						}
					}
					
					scrr.m_dscore=(*ite2).m_score;
					scrr.m_score=ite->m_score;
					break;
				}

				ite ++;
			}

		    //m_listRoom[scr->m_roomid-1].m_winplayer++;

		}


		//���������ݼ���
		char bufchat2[3];
		char buf2[3];
		memset(bufchat2,0,sizeof(bufchat2));
		memset(buf2,0,sizeof(buf2));

		int i,j;
		for(i=0;i<strlen(scrr.m_chatbuf);i=i+2)
		{
			bufchat2[0] = scrr.m_chatbuf[i];
			bufchat2[1] = scrr.m_chatbuf[i+1];
			bufchat2[2] = '\0';

			for(j=0;j<strlen(m_listRoom[scr->m_roomid-1].answer);j=j+2)
			{
				buf2[0] = m_listRoom[scr->m_roomid-1].answer[j];
				buf2[1] = m_listRoom[scr->m_roomid-1].answer[j+1];
				buf2[2] = '\0';
				if(strcmp(bufchat2,buf2) == 0)
				{
					scrr.m_chatbuf[i] = '*';
					scrr.m_chatbuf[i+1] = '*';
				}
			}

		}
	}

	auto ite = m_listRoom[scr->m_roomid-1].m_listInRoomPlayer.begin();
	while(ite != m_listRoom[scr->m_roomid-1].m_listInRoomPlayer.end())
	{
		m_pTcpNet->SendData((*ite).sock,(char*)&scrr,sizeof(scrr));
		ite ++;
	}

	//�ڷ����������Ϣ���ٷ�����Ϸ������
	if(m_listRoom[scr->m_roomid-1].m_winplayer == m_listRoom[scr->m_roomid-1].m_curplayer-1)
	{
		//�������߿ͻ�����Ϸ����
		STRU_GAMEOVER_RS srf;
		srf.m_nType=_DEF_PROTOCOL_GAMEOVER_RS;
        auto ite1 = m_listRoom[scr->m_roomid-1].m_listInRoomPlayer.begin();
	    while(ite1 != m_listRoom[scr->m_roomid-1].m_listInRoomPlayer.end())
	    {
	        m_pTcpNet->SendData((*ite1).sock,(char*)&srf,sizeof(srf));
	        ite1 ++;
	    }
		//��һ�ֿ�ʼ��������
		m_listRoom[scr->m_roomid-1].m_winplayer=0;
		is_first = 0;
	}


}


//�յ�
void TCPKernel::GetPointRq(SOCKET sock,char* szbuf)
{
	STRU_GETPOINT_RQ *psgr = (STRU_GETPOINT_RQ*)szbuf;
	
	STRU_GETPOINT_RS sgr;
	sgr.m_nType = _DEF_PROTOCOL_GETPOINT_RS;
	sgr.point = psgr->point;
	sgr.movepoint = psgr->movepoint;
	sgr.color = psgr->color;

	int i = psgr->m_roomid-1;
	auto ite  = m_listRoom[i].m_listInRoomPlayer.begin();
	while(ite != m_listRoom[i].m_listInRoomPlayer.end())
	{
		if(strncmp(psgr->m_szName,(*ite).m_szName,_DEF_SIZE) != 0)
			m_pTcpNet->SendData((*ite).sock,(char*)&sgr,sizeof(sgr));
		ite ++;
	}
}


//����ʼ��Ϸ
void TCPKernel::StartRq(SOCKET sock,char* szbuf)
{
	srand((unsigned )time(0));
	STRU_START_RQ *psgr = (STRU_START_RQ*)szbuf;	
	char szsql[_DEF_SQLLEN] = {0};//��SQL�����д洢
	list<string> lststr;//�Դ����ݿ����ҵ���u_id���д洢
	int u_id;//�洢u_id;
	int p_score=0;
	map<int,string> m_scoretoname;
    int grad;
	//����ظ���
	STRU_START_RS ssr;
	ssr.m_nType = _DEF_PROTOCOL_START_RS;
	//���͵�ǰ�������������Ϊ�� ��ʼ���Ʒֿ�
	int i = psgr->m_roomid-1;
	ssr.m_curplayer = m_listRoom[i].m_curplayer;
	char winname[_DEF_SIZE];
    int leng1;
	int i2;
	bool f;
	//׼��
	//1.������ֻ��һ���� ����׼���޷���ʼ��Ϸ ����������
	//2.���������������˺����������ϵ�ʱ�� ׼������Ӧ�ú͵�ǰ����������ͬ �����ܿ�ʼ
	if(m_listRoom[i].m_curplayer < 2)
	{
		ssr.szResult = _start_people;
		ssr.m_chatbuf[0] = '\0';
		ssr.szDrawplayername[0] = '\0';
	}
	else if(m_listRoom[i].m_curplayer > m_listRoom[i].m_curReadyplayer)
	{
		ssr.szResult = _start_fail;
		ssr.m_chatbuf[0] = '\0';
		ssr.szDrawplayername[0] = '\0';
	}
	else 
	{
		ssr.szResult = _start_success;
		ssr.szDrawplayername[0] = '\0';

		auto iite = m_listRoom[psgr->m_roomid-1].m_listInRoomPlayer.begin();
		while(iite != m_listRoom[psgr->m_roomid-1].m_listInRoomPlayer.end())
		{
			(*iite).m_flag = 0;
			iite ++;
		}
	
		m_listRoom[psgr->m_roomid-1].start_flag = true;

		//��ȡ��Ŀ
		FILE* file = fopen("../answer.txt","r");
		char answerbuf[4]; 
		char answer[_DEF_SIZE];
		memset(answer,0,sizeof(answer));
		int n = rand()%30;

		int count = 0;
		fseek(file,SEEK_SET,0);
		while(1)
		{
			memset(answerbuf,0,sizeof(answerbuf));
			fread(answerbuf,2,1,file);
		
			if( strcmp(answerbuf,"��") == 0 )
			{
				count ++;
				if(count == n)
				{
					memset(answerbuf,0,sizeof(answerbuf));
					while(1)
					{
						fread(answerbuf,2,1,file);
						if( strcmp(answerbuf,"��") == 0 ) break;
						strcat(answer,answerbuf);
					}
					break;
				}
			}
		}
	
		DWORD dw = GetLastError();

		//����Ӧ������յ׼�¼
		strcpy(m_listRoom[i].answer,answer);
	
		memset(ssr.m_chatbuf,0,sizeof(ssr.m_chatbuf));
		strcat(ssr.m_chatbuf,"��������ǣ�");
		strcat(ssr.m_chatbuf,answer);

		//�ҵ�һΪ0�� ���Ϊ���� �������յ�
		int ncount=0;
		map<string,int>::iterator ite3;
		ite3=m_listRoom[psgr->m_roomid-1].m_drawer.begin();
		while(ite3!=m_listRoom[psgr->m_roomid-1].m_drawer.end())
		{
			if((*ite3).second != 0)
			{  
				ncount++;
			}
			ite3++;
		}

		if(ncount < m_listRoom[psgr->m_roomid-1].m_drawer.size())
		{
			map<string,int>::iterator ite;
			string name;
			ite=m_listRoom[psgr->m_roomid-1].m_drawer.begin();
			while(ite!=m_listRoom[psgr->m_roomid-1].m_drawer.end())
			{
				if((*ite).second == 0)
				{  
					name=(*ite).first;
					(*ite).second=1; //ѡ��Ϊ���ҽ����־��Ϊ1��
					break;
				}
				ite++;
			}

			//����ת��
			int leng;
			leng=name.length();
			int i1;
			for( i1=0;i1<leng;i1++)
			{
				m_listRoom[i].p[i1]=name[i1];
			}
			m_listRoom[i].p[i1]='\0';
			strncpy(ssr.szDrawplayername,m_listRoom[i].p,_DEF_SIZE);

			//��������ƥ���ҵ����ҵ�sock �������ҷ�������;
			auto ite2=m_listRoom[i].m_listInRoomPlayer.begin();
			while(ite2!=m_listRoom[i].m_listInRoomPlayer.end())
			{
				if(strncmp((*ite2).m_szName,m_listRoom[i].p,_DEF_SIZE)==0)
				{
					m_pTcpNet->SendData((*ite2).sock,(char*)&ssr,sizeof(ssr));
				}
				ite2++;
			}

			//��������յ���Ϸ��ʼ��֪ͨ
			memset(ssr.m_chatbuf,0,sizeof(ssr.m_chatbuf));
			strcat(ssr.m_chatbuf,"��Ϸ��ʼ��");
		}
		else if(ncount == m_listRoom[psgr->m_roomid-1].m_drawer.size()) //һ����Ϸ���� 
		{
			ssr.szResult = _start_end;
			//��������Ĳ¶�����λ��� ���ұ�־�û�
			for(int i=0;i<_DEF_ROOMNUM;i++)
			{

				auto ite = m_listRoom[i].m_listInRoomPlayer.begin();
				while(ite != m_listRoom[i].m_listInRoomPlayer.end())
				{   //������������Լ�ֵ�Ե���ʽ����map�з����ҵ�������ߵģ������ʤ���ľ���
					//map��������Ϳ���֪�����Ŀ������������ͬ�ͻᷢ������

                    //m_scoretoname[ite->m_score]=ite->m_szName;
					//�ҵ���߷�
                    if(m_listRoom[i].m_max<(*ite).m_score)
					{
						m_listRoom[i].m_max=(*ite).m_score;
					}
					else
					{

					}
					//����Ϊ0��֮ǰ������д�����ݿ���
					//����SQL��佫�ҵ�U_id Ȼ����person�����ҵ�u_id��ɶ��ܷ֣��ȼ�����ʤ���Լ��ܾ����ĸ�д
					sprintf_s(szsql,"SELECT u_id FROM player where  p_name = '%s'" ,ite->m_szName);
					//��MySQL���õ�����һ��u_id����ϢҪ���в���
	                m_sql.SelectMySql(szsql,1,lststr);
				    if(lststr.size()>0)
					{
						u_id=atoi(lststr.front().c_str());
						//������ɺ��lststr�������
						lststr.pop_front();
						//��person�����ҵ�u_id����Ӧ���ֲܷ�������ӣ�
						sprintf_s(szsql,"SELECT p_score FROM person where  u_id = %d",u_id);
						m_sql.SelectMySql(szsql,1,lststr);
						//�õ��ֽܷ�����Ӵ������ݿ�
						p_score=atoi(lststr.front().c_str())+(ite->m_score);
						//�����ݿ���и���
						if(lststr.size()>0)
						{
						  memset(szsql,0,_DEF_SIZE);
						 sprintf_s(szsql,"update person set p_score=%lld where u_id=%d",p_score,u_id);
						f= m_sql.UpdateMySql(szsql);
						 //����Ϸ�������и���
						  memset(szsql,0,_DEF_SIZE);
						 sprintf_s(szsql,"update person set p_total=p_total+1 where u_id=%d",u_id);
						 f=m_sql.UpdateMySql(szsql);
						 //�Եȼ����и����ж��Ƿ����� p_score�Ǽ���֮��ķ��� lststr�Ǽ���֮��ķ���
                          if(p_score/100==atoi(lststr.front().c_str())/100+1)
						  {    
							   memset(szsql,0,_DEF_SIZE);
							   sprintf_s(szsql,"update person set p_level=p_level+1 where u_id=%d",u_id);
							   m_sql.UpdateMySql(szsql);
						  }
						  else
						  {
							  //���ø���;
						  }
						  //�ж��Ƿ�ʤ��


						}
					}
				/*	ite->m_score=0;
					ite->m_flag=0;
					ite++;*/
					ite++;
					lststr.pop_front();
				}

				//����map���Զ��������֪��mapβ���Ƿ�����ߵ�Ҳ����ʤ����
				//auto ite4=m_scoretoname.end();
				////grad=(*ite4).first;
				//auto iteb=m_scoretoname.begin();
				//while(iteb!=m_scoretoname.end())
				//{
				//	if((*iteb).first==(*ite4).first)
				//	{
				//	
			 //        leng1=(*iteb).second.length();
			 //       for( i2=0;i2<leng1;i2++)
			 //       {
				//      winname[i2]=(*iteb).second[i2];
			 //        }
			 //        winname[i2]='\0';
				//    sprintf_s(szsql,"update person set p_win=p_win+1 where p_name='%s'",winname);
				//	memset(winname,0,_DEF_SIZE);
				//	}
				//	iteb++;
				//}

				//�ҵ�ÿ��������Ӯ���˽�Ӯ�þ�����һ
				auto ite4 = m_listRoom[i].m_listInRoomPlayer.begin();
				while(ite4 != m_listRoom[i].m_listInRoomPlayer.end())
				{
					if(m_listRoom[i].m_max==(*ite4).m_score)
					{    
						 memset(szsql,0,_DEF_SIZE);
						 sprintf_s(szsql,"update person set p_win=p_win+1 where p_name='%s'",(*ite4).m_szName);
						f= m_sql.UpdateMySql(szsql);
					}
					ite4++;
				}
			
			//һ�ֽ�������ʶλ���
			auto ite5 = m_listRoom[i].m_listInRoomPlayer.begin();
			while(ite5 != m_listRoom[i].m_listInRoomPlayer.end())
			{
					ite5->m_score=0;
					ite5->m_flag=0;
					ite5++;
			}
			      m_listRoom[i].m_max=0;
			}

		}
	}

	auto ite = m_listRoom[i].m_listInRoomPlayer.begin();
	while(ite != m_listRoom[i].m_listInRoomPlayer.end())
	{
		m_pTcpNet->SendData((*ite).sock,(char*)&ssr,sizeof(ssr));
		ite ++;
	}

	//��ʱһ����Ϸ����
	if(ssr.szResult == _start_end)
	{   
		//����־�û�
		memset(m_listRoom[psgr->m_roomid-1].answer,0,_DEF_SIZE);
		//m_listRoom[psgr->m_roomid-1].m_curReadyplayer = 0;
		auto itee = m_listRoom[psgr->m_roomid-1].m_drawer.begin();
		while( itee != m_listRoom[psgr->m_roomid-1].m_drawer.end())
		{
			(*itee).second=0;
			itee ++;
		}
		m_listRoom[psgr->m_roomid-1].start_flag = false;
	}
}


//׼�� ȡ��׼��
void TCPKernel::ReadyRq(SOCKET sock,char* szbuf)
{
	STRU_READY_RQ *psgr = (STRU_READY_RQ*)szbuf;
	if(psgr->flag_ready == true) //����׼��
		m_listRoom[psgr->m_roomid-1].m_curReadyplayer++;
	else 
		m_listRoom[psgr->m_roomid-1].m_curReadyplayer--;

	STRU_READY_RS srr;
	srr.m_nType = _DEF_PROTOCOL_READY_RS;
	srr.flag_ready = psgr->flag_ready;
	m_pTcpNet->SendData(sock,(char*)&srr,sizeof(srr));
}


//ע��ɹ���ӵ�person����
void TCPKernel::InsertPersonRq(SOCKET sock,char* szbuf)
{
	STRU_INSERTPERSON_RQ *psgr = (STRU_INSERTPERSON_RQ*)szbuf;
	
	//ҲҪ������˵ĸ�����Ϣ��ӵ�person���ݿ���
	list<string> lstint;
	char szsql[_DEF_SQLLEN] = {0};
	sprintf_s(szsql,"select u_id from player where p_name='%s'",psgr->m_szName);
	m_sql.SelectMySql(szsql,1,lstint);

	//����ҵ���
	memset(szsql,0,_DEF_SQLLEN);
	if(lstint.size() != 0)
	{
		sprintf_s(szsql,"insert into person (u_id,p_name,p_id) values ('%d','%s','%lld')",
			atoi(lstint.front().c_str()),psgr->m_szName,psgr->m_tel);
		m_sql.UpdateMySql(szsql);
	}

}


//�༭������Ϣ
void TCPKernel::AlterPersonRq(SOCKET sock,char* szbuf)
{
	STRU_ALTERPERSON_RQ *psgr = (STRU_ALTERPERSON_RQ*)szbuf;

	STRU_ALTERPERSON_RS sar;
	sar.m_nType = _DEF_PROTOCOL_ALTERPERSON_RS;
	sar.szResult = _alterperson_success;

	//���ҵ�����˵�Ψһ��ʶ u_id
	list<string> lstint;
	char szsql[_DEF_SQLLEN] = {0};
	sprintf_s(szsql,"select u_id from player where p_name='%s'",psgr->m_szName);
	m_sql.SelectMySql(szsql,1,lstint);
	int u_id = atoi(lstint.front().c_str());

	
	//1.�ȿ����绰�Ÿ�û�� �绰��Ϊ0�Ͳ��ĵ绰����
	//����ĵ绰�� ��player���person��ĵ绰�Ŷ���Ҫ��
	if(psgr->m_tel != 0)
	{
		memset(szsql,0,_DEF_SQLLEN);
		sprintf_s(szsql,"update person set p_id = %lld where u_id = %d",psgr->m_tel,u_id);
		m_sql.UpdateMySql(szsql);
		memset(szsql,0,_DEF_SQLLEN);
		sprintf_s(szsql,"update player set p_id = %lld where u_id = %d",psgr->m_tel,u_id);
		m_sql.UpdateMySql(szsql);
	}

	//2.�Ա�0��1Ů
	if(psgr->sex == 0)
	{
		memset(szsql,0,_DEF_SQLLEN);
		sprintf_s(szsql,"update person set p_sex = '��' where u_id = %d",u_id);
		m_sql.UpdateMySql(szsql);
	}
	else if(psgr->sex == 1)
	{
		memset(szsql,0,_DEF_SQLLEN);
		sprintf_s(szsql,"update person set p_sex = 'Ů' where u_id = %d",u_id);
		m_sql.UpdateMySql(szsql);
	}
	//3.��������
	char birth[_DEF_SIZE] = {0};
	char year[_DEF_SIZE] = {0};
	char month[_DEF_SIZE] = {0};
	char day[_DEF_SIZE] = {0};
	strcat(birth,itoa(psgr->m_Year,year,10));
	strcat(birth,"-");
	strcat(birth,itoa(psgr->m_Month,month,10));
	strcat(birth,"-");
	strcat(birth,itoa(psgr->m_Day,day,10));
	
	memset(szsql,0,_DEF_SQLLEN);
	sprintf_s(szsql,"update person set p_birth = '%s' where u_id = %d",birth,u_id);
	m_sql.UpdateMySql(szsql);

	if(psgr->b_yespw == TRUE) //�޸�����
	{
		//1.ƥ�������
		list<string> lstpw;
		memset(szsql,0,_DEF_SQLLEN);
		sprintf_s(szsql,"select p_pw from player where u_id = %d",u_id);
		m_sql.SelectMySql(szsql,1,lstpw);
		
		//�������ľ���������ݿ��еľ�����һ��
		if(strncmp(lstpw.front().c_str(),psgr->m_szoldpw,_DEF_SIZE) == 0)
		{
			//2.���������ȷ�������Ƿ�һ��
			if(strncmp(psgr->m_sznewpw,psgr->m_sznewpw2,_DEF_SIZE) == 0)
			{
				//3.һ�¸�дplayer���е�����
				memset(szsql,0,_DEF_SQLLEN);
				sprintf_s(szsql,"update player set p_pw = %lld where u_id = %d",atoi(psgr->m_sznewpw),u_id);
				m_sql.UpdateMySql(szsql);
			}
			else
				sar.szResult = _alterperson_pwfail;
		}
		else
			sar.szResult = _alterperson_pwfail;
	}

	m_pTcpNet->SendData(sock,(char*)&sar,sizeof(sar));
}


//��Ӻ���
void TCPKernel::GetFriendRq(SOCKET sock,char* szbuf)
{
	STRU_GETFRIEND_RQ * psgr=(STRU_GETFRIEND_RQ*)szbuf;

	STRU_GETFRIEND_RS sgr;
	strcpy(sgr.m_friendName,psgr->m_friendName);
	strcpy(sgr.m_szName,psgr->m_szName);
	sgr.m_nType=_DEF_PROTOCOL_GETFRIEND_RS;
	sgr.m_isagree=2;
	//�ҵ���Ӧ�ĺ��ѣ�Ȼ������ѷ��ͺ�������
	list<OnlinePlayerServer*>::iterator ite = m_lstOnlinePlayer.begin();
	while(ite != m_lstOnlinePlayer.end())
	{
		if(strncmp(psgr->m_friendName,(*ite)->m_szName,_DEF_SIZE) == 0)
			m_pTcpNet->SendData((*ite)->sock,(char*)&sgr,sizeof(sgr));
		ite ++;
	}
	
}


//��Ӻ��ѻظ�
void TCPKernel::GetFriendRs(SOCKET sock,char* szbuf)
{
	STRU_GETFRIEND_RS * psgr=(STRU_GETFRIEND_RS*)szbuf;

	STRU_GETFRIEND_RS sgr;
	strcpy(sgr.m_friendName,psgr->m_friendName);
	strcpy(sgr.m_szName,psgr->m_szName);
	sgr.m_nType=_DEF_PROTOCOL_GETFRIEND_RS;
	sgr.m_isagree=psgr->m_isagree;
	
	if(psgr->m_isagree == 0)//ͬ�����
	{
		//�������˷ֱ���ӵ����ݿ���
		//���ҵ�����˵�Ψһ��ʶ u_id
		list<string> lstint;
		char szsql[_DEF_SQLLEN] = {0};
		sprintf_s(szsql,"select u_id from player where p_name='%s'",sgr.m_szName);
		m_sql.SelectMySql(szsql,1,lstint);
		int u_id = atoi(lstint.front().c_str());
		
		list<string> friend_lstint;
		sprintf_s(szsql,"select u_id from player where p_name='%s'",sgr.m_friendName);
		m_sql.SelectMySql(szsql,1,friend_lstint);
		int friend_id = atoi(friend_lstint.front().c_str());

		bool b;
		//ͨ��u_id��Ӻ���
		sprintf_s(szsql,"insert into friend values(%d,%d)",u_id,friend_id);
		b = m_sql.UpdateMySql(szsql);
		memset(szsql,0,_DEF_SQLLEN);
		sprintf_s(szsql,"insert into friend values(%d,%d)",friend_id,u_id);
		b = m_sql.UpdateMySql(szsql);

		//�������ͻ��˷�Э���
		list<OnlinePlayerServer*>::iterator ite = m_lstOnlinePlayer.begin();
		while(ite != m_lstOnlinePlayer.end())
		{
			//��������Ӻ��ѵ��˷��ͻظ���
			if(strncmp(sgr.m_friendName,(*ite)->m_szName,_DEF_SIZE) == 0)
			{
				char temp_name[_DEF_SIZE];
				strcpy(temp_name,sgr.m_friendName);
				strcpy(sgr.m_friendName,sgr.m_szName);
				strcpy(sgr.m_szName,temp_name);
				m_pTcpNet->SendData((*ite)->sock,(char*)&sgr,sizeof(sgr));
				ite ++;
				continue;
			}

			//�����Ͱ����˷��������
			if(strncmp(sgr.m_szName,(*ite)->m_szName,_DEF_SIZE) == 0)
			{
				m_pTcpNet->SendData((*ite)->sock,(char*)&sgr,sizeof(sgr));
				ite ++;
				continue;
			}
			ite ++;
		}

	
	}
	else if(psgr->m_isagree == 1)// �ܾ����
	{
		
		//�������ͻ��˷�Э���
		list<OnlinePlayerServer*>::iterator ite = m_lstOnlinePlayer.begin();
		while(ite != m_lstOnlinePlayer.end())
		{
			//�����ѷ��������
			if(strncmp(psgr->m_friendName,(*ite)->m_szName,_DEF_SIZE) == 0)
			{
				char temp_name[_DEF_SIZE];
				strcpy(temp_name,psgr->m_friendName);
				strcpy(psgr->m_friendName,psgr->m_szName);
				strcpy(psgr->m_szName,temp_name);
				m_pTcpNet->SendData((*ite)->sock,(char*)&psgr,sizeof(psgr));
			}


			//���Լ����ͻظ���
			if(strncmp(psgr->m_szName,(*ite)->m_szName,_DEF_SIZE) == 0)
			{
				m_pTcpNet->SendData((*ite)->sock,(char*)&psgr,sizeof(psgr));
			}
			ite ++;
		}

	
	}

}


//�鿴�����������
void TCPKernel::LookPersonRq(SOCKET sock,char* szbuf)
{
	STRU_LOOKPERSON_RQ * psgr=(STRU_LOOKPERSON_RQ*)szbuf;

	//���ҵ�����˵�Ψһ��ʶ u_id
	list<string> lstint;
	char szsql[_DEF_SQLLEN] = {0};
	sprintf_s(szsql,"select u_id from player where p_name='%s'",psgr->m_szWantLookName);
	m_sql.SelectMySql(szsql,1,lstint);
	int u_id = atoi(lstint.front().c_str());

	STRU_LOOKPERSON_RS slr;
	slr.m_nType = _DEF_PROTOCOL_LOOKPERSON_RS;
	strncpy(slr.m_szWantLookName,psgr->m_szWantLookName,_DEF_SIZE);
	
	//�绰����
	memset(szsql,0,_DEF_SQLLEN);
	list<string> lsttel;
	sprintf_s(szsql,"select p_id from person where u_id = %d",u_id);
	m_sql.SelectMySql(szsql,1,lsttel);
	strncpy(slr.p_id,lsttel.front().c_str(),_DEF_SIZE);

	//�Ա�
	memset(szsql,0,_DEF_SQLLEN);
	list<string> lstsex;
	sprintf_s(szsql,"select p_sex from person where u_id = %d",u_id);
	m_sql.SelectMySql(szsql,1,lstsex);
	if(strncmp(lstsex.front().c_str(),"��",_DEF_SIZE) == 0) //�Ա���0Ů1
		slr.sex = 0;
	else 
		slr.sex = 1;

	//����������
	char year[5] = {0};
	char month[4] = {0};
	char day[4] = {0};
	memset(szsql,0,_DEF_SQLLEN);
	list<string> lstbirth;
	sprintf_s(szsql,"select p_birth from person where u_id = %d",u_id);
	m_sql.SelectMySql(szsql,1,lstbirth);
	
	int i;
	int j=0;
	for(i=0;i<=3;i++)
	{
		year[j++] = (lstbirth.front().c_str())[i];
	}
	year[j] = '\0';

	j=0;
	for(i=5;i<=6;i++)
	{
		month[j++] = (lstbirth.front().c_str())[i];
	}
	month[j] = '\0';

	j=0;
	for(i=8;i<=9;i++)
	{
		day[j++] = (lstbirth.front().c_str())[i];
	}
	day[j] = '\0';

	slr.m_Year = atoi(year);
	slr.m_Month = atoi(month);
	slr.m_Day = atoi(day);

	//���� �ȼ�
	memset(szsql,0,_DEF_SQLLEN);
	list<string> lstscore;
	sprintf_s(szsql,"select p_score from person where u_id = %d",u_id);
	m_sql.SelectMySql(szsql,1,lstscore);
	slr.m_score = atoi(lstscore.front().c_str());

	memset(szsql,0,_DEF_SQLLEN);
	list<string> lstlevel;
	sprintf_s(szsql,"select p_level from person where u_id = %d",u_id);
	m_sql.SelectMySql(szsql,1,lstlevel);
	slr.m_level = atoi(lstlevel.front().c_str());

	//ʤ��
	memset(szsql,0,_DEF_SQLLEN);
	list<string> lstwin;
	sprintf_s(szsql,"select p_win from person where u_id = %d",u_id);
	m_sql.SelectMySql(szsql,1,lstwin);
	int m_win = atoi(lstwin.front().c_str());

	memset(szsql,0,_DEF_SQLLEN);
	list<string> lsttotal;
	sprintf_s(szsql,"select p_total from person where u_id = %d",u_id);
	m_sql.SelectMySql(szsql,1,lsttotal);
	int m_total = atoi(lsttotal.front().c_str());

	if(m_total != 0)
		slr.m_winpercent = (float)(m_win/m_total);
	else 
		slr.m_winpercent = 0.0;

	m_pTcpNet->SendData(sock,(char*)&slr,sizeof(slr));
	GetLastError();
}



void TCPKernel::DeleteFriendRq(SOCKET sock,char* szbuf)
{
	STRU_DELETEFRIEND_RQ * psdr=(STRU_DELETEFRIEND_RQ*)szbuf;

	STRU_DELETEFRIEND_RS sdr;
	strcpy(sdr.m_deleteName,psdr->m_deleteName);
	strcpy(sdr.m_szName,psdr->m_szName);
	sdr.m_nType=_DEF_PROTOCOL_DELETEFRIEND_RS;

	//�������˷ֱ���ӵ����ݿ���
		//���ҵ�����˵�Ψһ��ʶ u_id

		list<string> lstint;
		char szsql[_DEF_SQLLEN] = {0};
		sprintf_s(szsql,"select u_id from player where p_name='%s'",psdr->m_szName);
		m_sql.SelectMySql(szsql,1,lstint);
		int u_id = atoi(lstint.front().c_str());
		
		list<string> delete_lstint;
		sprintf_s(szsql,"select u_id from player where p_name='%s'",psdr->m_deleteName);
		m_sql.SelectMySql(szsql,1,delete_lstint);
		int delete_id = atoi(delete_lstint.front().c_str());



		bool b;
		bool bb;
		//ͨ��u_idɾ������
		sprintf_s(szsql,"delete from friend where u_id='%d'",u_id);
		b = m_sql.UpdateMySql(szsql);
		memset(szsql,0,_DEF_SQLLEN);
		sprintf_s(szsql,"delete from friend where u_id='%d'",delete_id);
		bb = m_sql.UpdateMySql(szsql);
		if(b==true && bb==true)
		{
			sdr.is_delete=true;
		}
		else
		{
			sdr.is_delete=false;
		}

			//�������ͻ��˷�Э���
		list<OnlinePlayerServer*>::iterator ite = m_lstOnlinePlayer.begin();
		while(ite != m_lstOnlinePlayer.end())
		{
			//��������Ӻ��ѵ��˷��ͻظ���
			if(strncmp(sdr.m_deleteName,(*ite)->m_szName,_DEF_SIZE) == 0)
			{
				char temp_name[_DEF_SIZE];
				strcpy(temp_name,sdr.m_deleteName);
				strcpy(sdr.m_deleteName,sdr.m_szName);
				strcpy(sdr.m_szName,temp_name);
				m_pTcpNet->SendData((*ite)->sock,(char*)&sdr,sizeof(sdr));
				ite ++;
				continue;
			}

			//�����Ͱ����˷��������
			if(strncmp(sdr.m_szName,(*ite)->m_szName,_DEF_SIZE) == 0)
			{
				m_pTcpNet->SendData((*ite)->sock,(char*)&sdr,sizeof(sdr));
				ite ++;
				continue;
			}
			ite ++;
		}


}


void TCPKernel::PersonChatRq(SOCKET sock,char* szbuf)
{
	STRU_PERSONCHAT_RQ * psdr=(STRU_PERSONCHAT_RQ*)szbuf;

	STRU_PERSONCHAT_RS spr;
	spr.m_nType = _DEF_PROTOCOL_PERSONCHAT_RS;
	strcpy_s(spr.m_szName,psdr->m_szName);
	strcpy_s(spr.m_chatbuf,psdr->m_chatbuf);

	//��ȡ���˵�����
	char name[_DEF_SIZE] = {0};
	for(int i=0;i<_DEF_SIZE;i++)
	{
		if(psdr->m_szName[i]!=' ')
			name[i] = psdr->m_szName[i];
		else 
		{
			name[i] = '\0';
			break;
		}
	}

	auto ite = m_lstOnlinePlayer.begin();
	while(ite != m_lstOnlinePlayer.end())
	{
		if( strncmp((*ite)->m_szName,name,_DEF_SIZE)==0 || 
			strncmp((*ite)->m_szName,psdr->m_szNameChat,_DEF_SIZE)==0)
		{
			m_pTcpNet->SendData((*ite)->sock,(char*)&spr,sizeof(spr));
		}
		ite ++;
	}


}


//�������
void TCPKernel::InvitePlayerRq(SOCKET sock,char* szbuf)
{
	STRU_INVITEPLAYER_RQ * psir=(STRU_INVITEPLAYER_RQ*)szbuf;

	STRU_INVITEPLAYER_RS sir;
	sir.m_nType = _DEF_PROTOCOL_INVITEPLAYER_RS;
	strncpy(sir.m_szName,psir->m_szName,_DEF_SIZE);
	strncpy(sir.m_szNameInvite,psir->m_szNameInvite,_DEF_SIZE);
	sir.m_roomid = psir->m_roomid;

	if(psir->m_roomid == -1)
	{
		sir.szResult = _inviteplayer_younotinroom;
		//�����������
		auto ite = m_lstOnlinePlayer.begin();
		while(ite != m_lstOnlinePlayer.end())
		{
			if(strncmp((*ite)->m_szName,sir.m_szName,_DEF_SIZE) == 0)
			{
				m_pTcpNet->SendData((*ite)->sock,(char*)&sir,sizeof(sir));
			}
			ite ++;
		}
	}
	
	else 
	{
		sir.szResult = _inviteplayer_success;
		//�������������
		auto ite = m_lstOnlinePlayer.begin();
		while(ite != m_lstOnlinePlayer.end())
		{
			if(strncmp((*ite)->m_szName,sir.m_szNameInvite,_DEF_SIZE) == 0)
			{
				m_pTcpNet->SendData((*ite)->sock,(char*)&sir,sizeof(sir));
			}
			ite ++;
		}
	}
	
}

void TCPKernel::InvitePlayerYnRq(SOCKET sock,char* szbuf)
{
	STRU_INVITEPLAYERYN_RQ * psir=(STRU_INVITEPLAYERYN_RQ*)szbuf;

	STRU_INVITEPLAYERYN_RS sir;
	sir.m_nType = _DEF_PROTOCOL_INVITEPLAYERYN_RS;
	sir.m_roomid = -1;
	sir.m_sitid = -1;

	if(psir->szResult == _inviteplayer_agree)
	{
		//�ҵ������б���������˵�sock
		auto ite1 = m_lstOnlinePlayer.begin();
		while(ite1 != m_lstOnlinePlayer.end())
		{
			if(strncmp((*ite1)->m_szName,psir->m_szNameInvite,_DEF_SIZE) == 0)
			{
				break;
			}
			ite1 ++;
		}

		//�鿴��������Ƿ��Ѿ���ʼ��Ϸ
		if(m_listRoom[psir->m_roomid-1].start_flag == false)
		{
			//�鿴������û�У���û��
			if(m_listRoom[psir->m_roomid-1].m_curplayer < m_listRoom[psir->m_roomid-1].m_maxplayer)
			{
				sir.szResult = _inviteplayer_success;
				sir.m_roomid = psir->m_roomid;
				for(int i=0;i<_DEF_ROOMNUM;i++)
				{
					if(m_listRoom[psir->m_roomid-1].issit[i] == false)
					{
						sir.m_sitid = i+1;
						break;
					}
				}
				m_pTcpNet->SendData((*ite1)->sock,(char*)&sir,sizeof(sir));
			}
			else 
			{
				sir.szResult = _inviteplayer_roomisfull;
				m_pTcpNet->SendData((*ite1)->sock,(char*)&sir,sizeof(sir));
			}

		}
		else
		{
			sir.szResult = _inviteplayer_roomisbegin;
			m_pTcpNet->SendData((*ite1)->sock,(char*)&sir,sizeof(sir));
		}
		
	}

	//�ܾ���������
	else if(psir->szResult == _inviteplayer_refuse)
	{
		sir.szResult = _inviteplayer_refuse;
		auto ite = m_listRoom[psir->m_roomid-1].m_listInRoomPlayer.begin();
		while(ite != m_listRoom[psir->m_roomid-1].m_listInRoomPlayer.end())
		{
			//֪ͨ������ˣ��㱻�ܾ���
			if(strncmp((*ite).m_szName,psir->m_szName,_DEF_SIZE) == 0)
			{
				m_pTcpNet->SendData((*ite).sock,(char*)&sir,sizeof(sir));
				break;
			}
			ite ++;
		}

	}


}


//��Ƶ
void TCPKernel::InviteVedioRq(SOCKET sock,char* szbuf)
{
	STRU_INVITEVEDIO_RQ * psir=(STRU_INVITEVEDIO_RQ*)szbuf;
	STRU_INVITEVEDIO_RQ sir;
	sir.m_nType = psir->m_nType;
	strncpy(sir.m_szName,psir->m_szName,_DEF_SIZE);
	strncpy(sir.m_szNameInvite,psir->m_szNameInvite,_DEF_SIZE);

	auto ite = m_lstOnlinePlayer.begin();
	while(ite != m_lstOnlinePlayer.end())
	{
		if(strncmp((*ite)->m_szName,psir->m_szNameInvite,_DEF_SIZE) == 0)
			m_pTcpNet->SendData((*ite)->sock,(char*)&sir,sizeof(sir));
		ite ++;
	}
};

void TCPKernel::InviteVedioRs(SOCKET sock,char* szbuf)
{
	STRU_INVITEVEDIO_RS * psir=(STRU_INVITEVEDIO_RS*)szbuf;
	STRU_INVITEVEDIO_RS sir;
	sir.m_nType = psir->m_nType;
	strncpy(sir.m_szName,psir->m_szName,_DEF_SIZE);
	strncpy(sir.m_szNameInvite,psir->m_szNameInvite,_DEF_SIZE);
	sir.szResult = psir->szResult;

	if(psir->szResult == _invitevedio_agree)
	{
		//����m_lstVedioPlayer
		PlayerVedio_Info* pv = (PlayerVedio_Info*)malloc(sizeof(PlayerVedio_Info));
		PlayerVedioReceive_Info* pvr = (PlayerVedioReceive_Info*)malloc(sizeof(PlayerVedioReceive_Info));
		PlayerVedio_Info* pv1 = (PlayerVedio_Info*)malloc(sizeof(PlayerVedio_Info));
		PlayerVedioReceive_Info* pvr1 = (PlayerVedioReceive_Info*)malloc(sizeof(PlayerVedioReceive_Info));

		auto ite = m_lstOnlinePlayer.begin();
		while(ite != m_lstOnlinePlayer.end())
		{
			if(strncmp((*ite)->m_szName,sir.m_szName,_DEF_SIZE) == 0)
			{
				m_pTcpNet->SendData((*ite)->sock,(char*)&sir,sizeof(sir));
			
				strncpy(pv->m_szName,(*ite)->m_szName,_DEF_SIZE);
				pv->m_sock = (*ite)->sock;
				pv->m_hSemaphore = CreateSemaphore(NULL,0,1,NULL);
				memset(pv->m_szAuthorBuffer,0,sizeof(pv->m_szAuthorBuffer));
				pv->m_noffset = 0;

				strncpy(pvr->m_szName,(*ite)->m_szName,_DEF_SIZE);
				pvr->m_sock = (*ite)->sock;
				pvr->m_hEvent = WSACreateEvent();
				pvr->m_noffset = 0;
				
			}
			if(strncmp((*ite)->m_szName,sir.m_szNameInvite,_DEF_SIZE) == 0)
			{
				m_pTcpNet->SendData((*ite)->sock,(char*)&sir,sizeof(sir));

				strncpy(pv1->m_szName,(*ite)->m_szName,_DEF_SIZE);
				pv1->m_sock = (*ite)->sock;
				pv1->m_hSemaphore = CreateSemaphore(NULL,0,1,NULL);
				memset(pv1->m_szAuthorBuffer,0,sizeof(pv->m_szAuthorBuffer));
				pv1->m_noffset = 0;

				strncpy(pvr1->m_szName,(*ite)->m_szName,_DEF_SIZE);
				pvr1->m_sock = (*ite)->sock;
				pvr1->m_hEvent = WSACreateEvent();
				pvr1->m_noffset = 0;
			}
			ite ++;
		}

		m_mapVedioPlayer[pv] = pvr1;
		m_mapVedioPlayer[pv1] = pvr;

		//���̳߳�Ͷ������
		Itask* pnew1 = new VedioItask(pvr,pv1,this);
		Itask* pnew2 = new VedioItask(pvr1,pv,this);
		m_threadpool.Push(pnew1);
		m_threadpool.Push(pnew2);

	}
	else if(psir->szResult == _invitevedio_refuse)
	{
		auto ite = m_lstOnlinePlayer.begin();
		while(ite != m_lstOnlinePlayer.end())
		{
			if(strncmp((*ite)->m_szName,sir.m_szName,_DEF_SIZE) == 0)
			{
				m_pTcpNet->SendData((*ite)->sock,(char*)&sir,sizeof(sir));
				break;
			}
			ite ++;
		}
	}
};

void TCPKernel::VedioStreamInfoRq(SOCKET sock,char* szbuf)
{
	STRU_VIDEOSTREAM_RQ* psvs = (STRU_VIDEOSTREAM_RQ*)szbuf;
	
	//�����sock��������Ϣ�ṹ��
	PlayerVedio_Info* pv;
	auto ite = m_mapVedioPlayer.begin();
	while(ite != m_mapVedioPlayer.end())
	{
		if( (*ite).first->m_sock == sock )
		{	
			pv = (*ite).first;
			break;
		}
		ite ++;
	}

	memcpy(pv->m_szAuthorBuffer+pv->m_noffset*_DEF_VIDEOSIZE,szbuf,_DEF_VIDEOSIZE);
	pv->m_noffset = (pv->m_noffset+1)%_DEF_BUFFERNUM;

	//�ͷ���Ӧ�������ź���
	ReleaseSemaphore(pv->m_hSemaphore,1,NULL);

}

//�ļ�
void TCPKernel::SendFileRq(SOCKET sock,char* szbuf)
{
	STRU_SENDFILE_RQ * pssr=(STRU_SENDFILE_RQ*)szbuf;

	STRU_SENDFILE_RS ssr;
	ssr.m_nType=_DEF_PROTOCOL_SENDFILE_RS;
	strcpy_s(ssr.m_szName,pssr->m_szName);
	strcpy_s(ssr.m_szNameChat,pssr->m_szNameChat);
	memcpy(ssr.m_szFileName,pssr->m_szFileName,_DEF_SIZE);
	ssr.m_nPos=0;



	auto ite = m_lstOnlinePlayer.begin();
	while(ite != m_lstOnlinePlayer.end())
	{
		//����������� �ſ��Է����ļ�
		if( strncmp((*ite)->m_szName,ssr.m_szNameChat,_DEF_SIZE)==0)
		{
				ssr.m_lresult=true;
				break;

		}
		ite++;
	}

	auto ite1 = m_lstOnlinePlayer.begin();
	while(ite1 != m_lstOnlinePlayer.end())
	{
	
		if( strncmp((*ite1)->m_szName,ssr.m_szName,_DEF_SIZE)==0)
		{
			m_pTcpNet->SendData((*ite1)->sock,(char*)&ssr,sizeof(ssr));
			break;
		}

		ite1 ++;
	}


}

void TCPKernel::SendFileBlockRq(SOCKET sock,char* szbuf)
{
	STRU_SENDFILEBLOCK_RQ *pssr = (STRU_SENDFILEBLOCK_RQ *)szbuf;

	STRU_SENDFILEBLOCK_RS ssr;
	ssr.m_nType=_DEF_PROTOCOL_SENDFILEBLOCK_RS;
	strcpy_s(ssr.m_szName,pssr->m_szName);
	strcpy_s(ssr.m_szNameChat,pssr->m_szNameChat);
	memcpy(ssr.m_szFileName,pssr->m_szFileName,MAX_PATH);
	memcpy(ssr.m_szFileContent,pssr->m_szFileContent,MAX_FILEBLOCK_LEN);
	ssr.m_nlen=pssr->m_nlen;

	STRU_SENDFILEBLOCK_RS chat_ssr;
	chat_ssr.m_nType=_DEF_PROTOCOL_RECVFILEBLOCK_RS;
	strcpy_s(chat_ssr.m_szName,pssr->m_szName);
	strcpy_s(chat_ssr.m_szNameChat,pssr->m_szNameChat);
	memcpy(chat_ssr.m_szFileName,pssr->m_szFileName,MAX_PATH);
	memcpy(chat_ssr.m_szFileContent,pssr->m_szFileContent,MAX_FILEBLOCK_LEN);
	chat_ssr.m_nlen=pssr->m_nlen;

	auto ite = m_lstOnlinePlayer.begin();
	while(ite != m_lstOnlinePlayer.end())
	{
		if( strncmp((*ite)->m_szName,ssr.m_szName,_DEF_SIZE)==0  )
		{
			m_pTcpNet->SendData((*ite)->sock,(char* )&ssr,sizeof(ssr));
			
		}
		if(strncmp((*ite)->m_szName,ssr.m_szNameChat,_DEF_SIZE)==0 )
		{
			m_pTcpNet->SendData((*ite)->sock,(char*)&chat_ssr,sizeof(chat_ssr));
		
		}
		ite ++;
	}
	
	memset(ssr.m_szFileName,0,MAX_PATH);
	memset(ssr.m_szFileContent,0,MAX_FILEBLOCK_LEN);
}

void TCPKernel::GetTeamRq(SOCKET sock,char* szbuf)
{
	//���յ�ƥ������
	STRU_GETTEAM_RQ *pssr = (STRU_GETTEAM_RQ *)szbuf;

	GetTeamPlayer* gtp = (GetTeamPlayer*)malloc(sizeof(GetTeamPlayer));
	strncpy(gtp->m_szName,pssr->m_szName,_DEF_SIZE);
	gtp->m_sock=sock;

	//������ƥ����˷Ž�ƥ�����  hashmap �ȵ����ݿ����ҵ����˵صȼ� 
	list<string> lstint;
	char szsql[_DEF_SQLLEN] = {0};
	sprintf_s(szsql,"select u_id from player where p_name='%s'",pssr->m_szName);
	m_sql.SelectMySql(szsql,1,lstint);
	int u_id = atoi(lstint.front().c_str());

	memset(szsql,0,_DEF_SQLLEN);
	list<string> lstlevel;
	sprintf_s(szsql,"select p_level from person where u_id = %d",u_id);
	m_sql.SelectMySql(szsql,1,lstlevel);
	int m_level = atoi(lstlevel.front().c_str());
	gtp->level= m_level;
	gtp->isget = false;
	//Ȼ����ݵȼ��ŵ�map��
	m_mapleveltoPlayerQueue[m_level].push_back(gtp);
	//�Ѵ��˷ŵ���ƥ�����
	m_mapGetTeamPlayer[gtp];

	//��������ƥ����˴���һ���̡߳�ͨ������߳�ȥƥ������ҵ����ʵ���ң�Ϊ�˲���ͻ����Ҫ����
	Itask* pnew2 = new GetTeamTask(gtp,this);
	m_getteamthreadpool.Push(pnew2);


}