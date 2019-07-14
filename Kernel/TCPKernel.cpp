#include"TCPKernel.h"
#include<stdlib.h>
#include<math.h>
#include<time.h>


void VedioItask::RunItask()
{
	//等着向对方推流
	HANDLE hary[] = {m_pv->m_hSemaphore,m_pvr->m_hEvent};
	DWORD  dwIndex;
	while(1)
	{
		//等信号或事件
		//如果等到信号了就推流 如果等到事件了就结束
		dwIndex = WaitForMultipleObjects(2,hary,FALSE,INFINITE);
		dwIndex -=  WAIT_OBJECT_0;

		if(dwIndex == 0) //自己是0
		{
			//将流推向自己的观众
			m_pKernel->m_pTcpNet->SendData(m_pvr->m_sock,
				m_pv->m_szAuthorBuffer+m_pvr->m_noffset*_DEF_VIDEOSIZE,_DEF_VIDEOSIZE);
			m_pvr->m_noffset = (m_pvr->m_noffset+1)%_DEF_BUFFERNUM;
		}
		else if(dwIndex == 1) //对面是1
			break;
	}
}
int roomnum = 7;
void GetTeamTask::RunItask()
{
	int num=0;
	while(1)
	{
		//判断匹配池中有没有自己
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
					//如果没有 就证明自己在别人的待匹配池中，就只负责等待匹配成功
				}
				ite ++;
			}
			
			if(m_gp->isget == false)//如果有，就证明自己没有在别人的带匹配池中，
			{
				//就关键段，让线程在匹配池中匹配，如果匹配到了，就放在待匹配池中,并从匹配池中删除，然后解锁
				m_pKernel->my_lock.Lock();

				auto ite = m_pKernel->m_mapleveltoPlayerQueue[m_gp->level].begin();
				while(ite != m_pKernel->m_mapleveltoPlayerQueue[m_gp->level].end())
				{
		
					if(m_pKernel->m_mapleveltoPlayerQueue[m_gp->level].size() > 1)
					{
						if(strncmp((*ite)->m_szName,m_gp->m_szName,_DEFSIZE) != 0)
						{
							//加入到待匹配池中
							m_pKernel->m_mapGetTeamPlayer[m_gp].push_back((*ite));
							//从匹配池中删除
							m_pKernel->m_mapleveltoPlayerQueue[m_gp->level].erase(ite);
							num++;
							if(num == 5)//队友加上自己已经到达房间的上线可以进行游戏
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
									//加入到待匹配池中
									m_pKernel->m_mapGetTeamPlayer[m_gp].push_back((*ite));
									//从匹配池中删除
									m_pKernel->m_mapleveltoPlayerQueue[m_gp->level+1].erase(ite);
									num++;
									if(num == 5)
										break;
								}
							
							}
							ite ++;
						}
						
						if(m_gp->level >=2) //找下面一级的玩家（初始等级为1）所以要大于等于二
						{
										auto ite = m_pKernel->m_mapleveltoPlayerQueue[m_gp->level-1].begin();
										while(ite != m_pKernel->m_mapleveltoPlayerQueue[m_gp->level-1].end())
										{
		
									if(m_pKernel->m_mapleveltoPlayerQueue[m_gp->level-1].size() > 1)
									{
										if(strncmp((*ite)->m_szName,m_gp->m_szName,_DEFSIZE) != 0)
										{
											//加入到待匹配池中
											m_pKernel->m_mapGetTeamPlayer[m_gp].push_back((*ite));
											//从匹配池中删除
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
				//在待匹配池中删除同时在自己的Map中删除
			   //如果同等级以及上下等级的都遍历后还是没有找到6个人 那么只要当前人数大于两个人就开始游戏
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
			//判断待匹配池中是否小于六个人 ，如果小于六个人你就开始游戏

	
	}
	
}
TCPKernel::TCPKernel()
{
	m_pTcpNet = new TCPNet(this);

	//给服务器端的储存每个房间的玩家信息的数组初始化
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

//协议映射表
_BEGIN_PROTOCOL_MAP
	PM(_DEF_PROTOCOL_REGISTER_RQ,&TCPKernel::RegisterRq) //注册
	PM(_DEF_PROTOCOL_LOGIN_RQ,&TCPKernel::LoginRq) //登录
	PM(_DEF_PROTOCOL_ONLINE_RQ,&TCPKernel::SendOnlinePlayerListRq) //在线列表请求
	PM(_DEF_PROTOCOL_QUIT_RQ,&TCPKernel::QuitRq) //退出
	PM(_DEF_PROTOCOL_CHAT_RQ,&TCPKernel::ChatRq) //聊天
	PM(_DEF_PROTOCOL_GETROOM_RQ,&TCPKernel::GetRoomRq) //房间信息获取请求
	PM(_DEF_PROTOCOL_QUITROOM_RQ,&TCPKernel::QuitRoomRq) //退出房间请求
	PM(_DEF_PROTOCOL_CHATROOM_RQ,&TCPKernel::ChatRoomRq) //房间聊天
	PM(_DEF_PROTOCOL_GETPOINT_RQ,&TCPKernel::GetPointRq) //收点
	PM(_DEF_PROTOCOL_START_RQ,&TCPKernel::StartRq) //开始游戏
	PM(_DEF_PROTOCOL_READY_RQ,&TCPKernel::ReadyRq) //准备 取消准备
	PM(_DEF_PROTOCOL_INSERTPERSON_RQ,&TCPKernel::InsertPersonRq) //注册成功添加到person表中
	PM(_DEF_PROTOCOL_ALTERPERSON_RQ,&TCPKernel::AlterPersonRq) //编辑个人信息
	PM(_DEF_PROTOCOL_GETFRIEND_RQ,&TCPKernel::GetFriendRq) //添加好友请求
	PM(_DEF_PROTOCOL_GETFRIEND_RS,&TCPKernel::GetFriendRs) //添加好友回复
	PM(_DEF_PROTOCOL_LOOKPERSON_RQ,&TCPKernel::LookPersonRq) //查看玩家资料
	PM(_DEF_PROTOCOL_DELETEFRIEND_RQ,&TCPKernel::DeleteFriendRq) //添加好友回复
	PM(_DEF_PROTOCOL_PERSONCHAT_RQ,&TCPKernel::PersonChatRq) //私信
	PM(_DEF_PROTOCOL_INVITEPLAYER_RQ,&TCPKernel::InvitePlayerRq) //邀请游戏
	PM(_DEF_PROTOCOL_INVITEPLAYERYN_RQ,&TCPKernel::InvitePlayerYnRq) //回复邀请结果：接受或拒绝
	PM(_DEF_PROTOCOL_INVITEVEDIO_RQ,&TCPKernel::InviteVedioRq) //视频
	PM(_DEF_PROTOCOL_INVITEVEDIO_RS,&TCPKernel::InviteVedioRs)
	PM(_DEF_PROTOCOL_VIDEOSTREAMINFO_RQ,&TCPKernel::VedioStreamInfoRq)
	PM(_DEF_PROTOCOL_VIDEOSTREAM_RQ,&TCPKernel::VedioStreamInfoRq)
	PM(_DEF_PROTOCOL_SENDFILE_RQ,&TCPKernel::SendFileRq) //发送文件头
	PM(_DEF_PROTOCOL_SENDFILEBLOCK_RQ,&TCPKernel::SendFileBlockRq) //发送文件块
	PM(_DEF_PROTOCOL_GETTEAM_RQ,&TCPKernel::GetTeamRq) //发送文件块
_END_PROTOCOL_MAP

//--------------------------------------------------------------------------------------------

void TCPKernel::DealData(SOCKET sock,char* szbuf)
{
	//处理数据 -> 协议映射表
	//处理数据---协议映射表
	
	//拆包 然后获得协议头, 然后调用deal函数处理
	char iType = *(char*)szbuf;
	//判断是否在数组中
	PFUN pf = 0 ;
	if( iType - _DEF_PROTOCOL_BASE >= 0 && iType - _DEF_PROTOCOL_BASE < _DEF_PACK_COUNT)
	pf = _PMArray[iType - _DEF_PROTOCOL_BASE];
	//判断是否为空 非空 才可以调用
	if(pf)
	((*this).*pf)(sock,szbuf);

	
}


//接受注册请求包
void TCPKernel::RegisterRq(SOCKET sock,char* szbuf)
{
	STRU_REGISTER_RQ *psrr = (STRU_REGISTER_RQ *)szbuf;
	char szsql[_DEF_SQLLEN] = {0};
	
	STRU_REGISTER_RS srr;
	srr.m_nType = _DEF_PROTOCOL_REGISTER_RS;
	memcpy(srr.m_szName,psrr->m_szName,_DEF_SIZE);
	srr.m_tel = psrr->m_tel;
	
	list<string> lststr;
	
	//检查数据库中是否存在此人
	sprintf_s(szsql,"SELECT p_id FROM player where p_id = %lld or a_name = '%s'" ,psrr->m_tel,psrr->m_szName);
	m_sql.SelectMySql(szsql,1,lststr);

	if(lststr.size() ==0)
	{
		srr.m_szResult = _register_success;
		//将此人插入到数据库中
		sprintf_s(szsql,"insert into player (p_id,p_name,p_pw) values(%lld,'%s','%s')",psrr->m_tel,psrr->m_szName,psrr->m_szPassword);
		m_sql.UpdateMySql(szsql);
	}

	m_pTcpNet->SendData(sock,(char*)&srr,sizeof(srr));
}


//接受登录请求
void TCPKernel::LoginRq(SOCKET sock,char* szbuf)
{
	//1 2  3
	//在数据库中查找
	//检验密码
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

			//记录用户信息  以便发送当前各桌信息
			Player_Info *pInfo = new Player_Info;
			pInfo->m_hEvent = WSACreateEvent();
			pInfo->m_sock = sock;
			m_lstPlayer.push_back(pInfo);
			m_mapSocketToPlayerInfo[sock] = pInfo;

			//加入当前大厅在线玩家链表
			OnlinePlayerServer* p = (OnlinePlayerServer*)malloc(sizeof(OnlinePlayerServer));
			memcpy(p->m_szName,pslr->m_szName,_DEF_SIZE);
			p->sock = sock;
				
			int flag = 0; //没有找到与之匹配的玩家姓名
			auto ite = m_lstOnlinePlayer.begin();
			while(ite != m_lstOnlinePlayer.end())
			{
				//如果链表里已经存在这个登录玩家的姓名那么就不用再push了
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

	//在数据库中查找所有好友发给客户端
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


//给当前大厅在线所有玩家发当前链表
void TCPKernel::SendOnlinePlayerListRq(SOCKET sock,char* szbuf)
{
	//接受请求包
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


//退出
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
	
	//发送在线玩家链表
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


//大厅聊天
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



//接受房间信息获取请求
void TCPKernel::GetRoomRq(SOCKET sock,char* szbuf)
{
	STRU_GETINTOROOM_RQ* psgr=(STRU_GETINTOROOM_RQ*)szbuf;

	//构造回复的协议包
	STRU_GETINTOROOM_RS sgr;
	sgr.m_nType=_DEF_PROTOCOL_GETROOM_RS;
	sgr.m_roomid = psgr->m_roomid;
	sgr.m_sitid = psgr->m_sitid;
	sgr.m_buttonid = psgr->m_buttonid;
	
	if(m_listRoom[psgr->m_roomid-1].start_flag == true)
	{
		sgr.szResult = _getroominfo_fail;
	}

	//开始游戏分数初始化
	auto ite0 = m_listRoom[sgr.m_roomid-1].m_listInRoomPlayer.begin();
	while(ite0 != m_listRoom[sgr.m_roomid-1].m_listInRoomPlayer.end())
	{
		ite0->m_flag=0;
		ite0++;
	}

	
	//添加到服务器端的储存每个房间的玩家情况的数组中
	int i = (psgr->m_roomid) -1;
	//判断当前的桌上在线人数有没有超过最大人数
	if(m_listRoom[i].m_maxplayer > m_listRoom[i].m_curplayer)
	{
		//如果这个桌子的对应座位号为false 说明没有人 可以坐
		if(m_listRoom[i].issit[psgr->m_sitid-1] == false)
		{
			//把座位标志置为true
			m_listRoom[i].issit[psgr->m_sitid-1] = true;
			m_listRoom[i].sitidc[psgr->m_sitid-1] = psgr->m_buttonid;
			m_listRoom[i].m_curplayer ++;

			InRoomPlayerServer p;
			strcpy_s(p.m_szName,psgr->m_playername);
			p.sock = sock;
			p.m_flag = 0;
			p.m_score = 0;
			m_listRoom[i].m_listInRoomPlayer.push_back(p);
			//成功坐下 
			sgr.szResult = _getroominfo_success;

			//发送换肤包 换肤是大厅在线的所有玩家都可以收到
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
	
		//如果对应座位号的标志位为true 坐不下 结果为失败
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
		//如果进入房间成功 将玩家添加到待成为画家的map;
		//初始都没有开始游戏都是0 画完的人是1  退出的为2
		string adc;
		adc=psgr->m_playername;
		m_listRoom[psgr->m_roomid-1].m_drawer[adc]=0;
		
	}

	else 
		m_pTcpNet->SendData(sock,(char*)&sgr,sizeof(sgr));
}


//退出房间请求
void TCPKernel::QuitRoomRq(SOCKET sock,char* szbuf)
{
	//接收要退出的房间号 和玩家姓名
	STRU_QUITROOM_RQ* psgr=(STRU_QUITROOM_RQ*)szbuf;
	int i = psgr->m_roomid-1;
	DWORD dw = GetLastError();
	char szsql[_DEF_SQLLEN] = {0};//对SQL语句进行存储
	list<string> lststr;//对从数据库中找到的进行存储
	int p_score;
	bool f;
	//如果退出的玩家是在游戏中的,那么我们将此玩家视为逃跑，从数据库中扣除50分
	if(m_listRoom[psgr->m_roomid-1].start_flag==true)
	{
		auto ite7 = m_listRoom[psgr->m_roomid-1].m_listInRoomPlayer.begin();
        while(ite7 != m_listRoom[psgr->m_roomid-1].m_listInRoomPlayer.end())
	     {
		   //如果发现比对相同的姓名 就将此玩家的分数从数据库中扣除50分不足50分清零
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
							  //判断等级是否需要改变
							  if(p_score/100!=(p_score-50)/100)
							  {
							     memset(szsql,0,_DEF_SIZE);
							     sprintf_s(szsql,"update person set p_level=p_level-1 where p_name='%s'",ite7->m_szName); 
							     f= m_sql.UpdateMySql(szsql);
							  }
							  else
							  {
								  //不改变
							  }
						     
						  }
						  else
						  {
							  memset(szsql,0,_DEF_SIZE);
							  sprintf_s(szsql,"update person set p_score=0 where p_name='%s'",ite7->m_szName);
						      f= m_sql.UpdateMySql(szsql);
						  }
						  //将玩家的总局数进行修改
						      memset(szsql,0,_DEF_SIZE);
							  sprintf_s(szsql,"update person set p_total=p_total+1 where p_name='%s'",ite7->m_szName);
						      f= m_sql.UpdateMySql(szsql);
					   }
						
			    }
			        ite7 ++;
		     }
		
	      }

	//如果退出的玩家是准备的
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

	//构造回复协议包
	STRU_QUITROOM_RS sqr;
	sqr.m_nType = _DEF_PROTOCOL_QUITROOM_RS;
	strncpy(sqr.m_szName,psgr->m_szName,_DEF_SIZE);
	sqr.szResult = _quit_fail;
	int j = 0;
	for(j;j<_DEF_ROOMNUM;j++)
	{
		memset(sqr.inroom[j].m_szName,0,sizeof(sqr.inroom[j].m_szName));
	}

	//将服务器端的该玩家在相应的房间中删除,将玩家名所对应的map置为2
	auto ite100 = m_listRoom[psgr->m_roomid-1].m_listInRoomPlayer.begin();
	while(ite100 != m_listRoom[psgr->m_roomid-1].m_listInRoomPlayer.end())
	{
		//如果发现比对相同的姓名 就把这个玩家从链表中删除
		if( strcmp((*ite100).m_szName,psgr->m_szName) == 0 )
		{   //将玩家名所对应的map置为 2
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
			//将玩家从链表中删除
			m_listRoom[psgr->m_roomid-1].m_listInRoomPlayer.erase(ite100);
			sqr.szResult = _quit_success;
			m_listRoom[psgr->m_roomid-1].m_curplayer--;
			if(m_listRoom[psgr->m_roomid-1].m_curplayer == 0)
				m_listRoom[psgr->m_roomid-1].start_flag = false;
			break ;
		}
		ite100 ++;
	}

	//将map为2的删除
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
	//如果玩家逃跑之后只剩下一个人,就给其发送玩家结束包
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

	//改变服务器端的房间里的数组
	m_listRoom[psgr->m_roomid-1].issit[psgr->sitid-1] = false;
	for(int p=0;p<_DEF_ROOMNUM;p++)
	{
		if(m_listRoom[psgr->m_roomid-1].sitidc[p] == psgr->sitidc)
			m_listRoom[psgr->m_roomid-1].sitidc[p] = 0;
	}

	//发送回复协议包
	//回复协议包是房间里的每个玩家都要收到 该玩家收到结果为_quit_success这个回复协议包 
	m_pTcpNet->SendData(sock,(char*)&sqr,sizeof(sqr));

	//不是退出的玩家收到的应该是更新列表协议包 结果为_quit_others
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

	//发送给链表中的剩余玩家
	ite = m_listRoom[i].m_listInRoomPlayer.begin();
	while(ite != m_listRoom[i].m_listInRoomPlayer.end())
	{
		if((*ite).m_szName[0] != NULL)
		{
			m_pTcpNet->SendData((*ite).sock,(char*)&sqr,sizeof(sqr));
		}
		ite ++;
	}

	//通知在线玩家换肤
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



static int is_first=0; //是否是第一个猜对的人


//接受房间聊天请求
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


	//如果flag是false 没有开始游戏 正常聊天 直接发 scr->flag
	if(m_listRoom[scr->m_roomid-1].start_flag == true) //对聊天内容进行判断和加密
	{
		//加密之前进行判断是否猜对
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

			//如果有人猜对 就要给这个人加分
			auto ite2=m_listRoom[scr->m_roomid-1].m_listInRoomPlayer.begin();
			while(ite2!=m_listRoom[scr->m_roomid-1].m_listInRoomPlayer.end())
			{
				//找到画的人
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

				    //如果这个人没猜对过 并且不是画的人
					if((ite->m_flag==0)&&(strncmp(scr->m_szName,m_listRoom[scr->m_roomid-1].p,m_ncount)!=0))
					{
						is_first++;
						//如果这个人是第一个人猜对
						if(is_first == 1)
						{
							ite->m_score+=5;
							ite->m_flag=1;

							(*ite2).m_score+=2;

							m_listRoom[scr->m_roomid-1].m_winplayer++;
							
						}
						//如果这个人已经不是第一个人猜对了
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


		//把聊天内容加密
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

	//在发送完加密信息后再发送游戏结束包
	if(m_listRoom[scr->m_roomid-1].m_winplayer == m_listRoom[scr->m_roomid-1].m_curplayer-1)
	{
		//发包告诉客户端游戏结束
		STRU_GAMEOVER_RS srf;
		srf.m_nType=_DEF_PROTOCOL_GAMEOVER_RS;
        auto ite1 = m_listRoom[scr->m_roomid-1].m_listInRoomPlayer.begin();
	    while(ite1 != m_listRoom[scr->m_roomid-1].m_listInRoomPlayer.end())
	    {
	        m_pTcpNet->SendData((*ite1).sock,(char*)&srf,sizeof(srf));
	        ite1 ++;
	    }
		//下一局开始后继续标记
		m_listRoom[scr->m_roomid-1].m_winplayer=0;
		is_first = 0;
	}


}


//收点
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


//请求开始游戏
void TCPKernel::StartRq(SOCKET sock,char* szbuf)
{
	srand((unsigned )time(0));
	STRU_START_RQ *psgr = (STRU_START_RQ*)szbuf;	
	char szsql[_DEF_SQLLEN] = {0};//对SQL语句进行存储
	list<string> lststr;//对从数据库中找到的u_id进行存储
	int u_id;//存储u_id;
	int p_score=0;
	map<int,string> m_scoretoname;
    int grad;
	//构造回复包
	STRU_START_RS ssr;
	ssr.m_nType = _DEF_PROTOCOL_START_RS;
	//发送当前房间玩家人数是为了 初始化计分框
	int i = psgr->m_roomid-1;
	ssr.m_curplayer = m_listRoom[i].m_curplayer;
	char winname[_DEF_SIZE];
    int leng1;
	int i2;
	bool f;
	//准备
	//1.房间里只有一个人 并且准备无法开始游戏 最少两个人
	//2.当房间里有两个人和两个人以上的时候 准备人数应该和当前房间人数相同 否则不能开始
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

		//获取题目
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
		
			if( strcmp(answerbuf,"：") == 0 )
			{
				count ++;
				if(count == n)
				{
					memset(answerbuf,0,sizeof(answerbuf));
					while(1)
					{
						fread(answerbuf,2,1,file);
						if( strcmp(answerbuf,"：") == 0 ) break;
						strcat(answer,answerbuf);
					}
					break;
				}
			}
		}
	
		DWORD dw = GetLastError();

		//把相应房间的谜底记录
		strcpy(m_listRoom[i].answer,answer);
	
		memset(ssr.m_chatbuf,0,sizeof(ssr.m_chatbuf));
		strcat(ssr.m_chatbuf,"你的谜题是：");
		strcat(ssr.m_chatbuf,answer);

		//找第一为0的 身份为画家 给他发谜底
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
					(*ite).second=1; //选中为画家将其标志改为1；
					break;
				}
				ite++;
			}

			//类型转换
			int leng;
			leng=name.length();
			int i1;
			for( i1=0;i1<leng;i1++)
			{
				m_listRoom[i].p[i1]=name[i1];
			}
			m_listRoom[i].p[i1]='\0';
			strncpy(ssr.szDrawplayername,m_listRoom[i].p,_DEF_SIZE);

			//根据名字匹配找到画家的sock 并给画家发送谜题;
			auto ite2=m_listRoom[i].m_listInRoomPlayer.begin();
			while(ite2!=m_listRoom[i].m_listInRoomPlayer.end())
			{
				if(strncmp((*ite2).m_szName,m_listRoom[i].p,_DEF_SIZE)==0)
				{
					m_pTcpNet->SendData((*ite2).sock,(char*)&ssr,sizeof(ssr));
				}
				ite2++;
			}

			//其他玩家收到游戏开始的通知
			memset(ssr.m_chatbuf,0,sizeof(ssr.m_chatbuf));
			strcat(ssr.m_chatbuf,"游戏开始！");
		}
		else if(ncount == m_listRoom[psgr->m_roomid-1].m_drawer.size()) //一轮游戏结束 
		{
			ssr.szResult = _start_end;
			//将房间里的猜对人置位零分 并且标志置回
			for(int i=0;i<_DEF_ROOMNUM;i++)
			{

				auto ite = m_listRoom[i].m_listInRoomPlayer.begin();
				while(ite != m_listRoom[i].m_listInRoomPlayer.end())
				{   //将名字与分数以键值对的形式存在map中方便找到分数最高的，来标记胜利的局数
					//map不用排序就可以知道最大的可是如果分数相同就会发生覆盖

                    //m_scoretoname[ite->m_score]=ite->m_szName;
					//找到最高分
                    if(m_listRoom[i].m_max<(*ite).m_score)
					{
						m_listRoom[i].m_max=(*ite).m_score;
					}
					else
					{

					}
					//在置为0分之前将分数写到数据库中
					//利用SQL语句将找到U_id 然后再person表中找到u_id完成对总分，等级，和胜局以及总局数的改写
					sprintf_s(szsql,"SELECT u_id FROM player where  p_name = '%s'" ,ite->m_szName);
					//从MySQL中拿到的是一行u_id的信息要进行操作
	                m_sql.SelectMySql(szsql,1,lststr);
				    if(lststr.size()>0)
					{
						u_id=atoi(lststr.front().c_str());
						//操作完成后对lststr进行清空
						lststr.pop_front();
						//从person表中找到u_id所对应的总分并进行添加；
						sprintf_s(szsql,"SELECT p_score FROM person where  u_id = %d",u_id);
						m_sql.SelectMySql(szsql,1,lststr);
						//得到总分进行相加存入数据库
						p_score=atoi(lststr.front().c_str())+(ite->m_score);
						//对数据库进行更改
						if(lststr.size()>0)
						{
						  memset(szsql,0,_DEF_SIZE);
						 sprintf_s(szsql,"update person set p_score=%lld where u_id=%d",p_score,u_id);
						f= m_sql.UpdateMySql(szsql);
						 //对游戏局数进行更改
						  memset(szsql,0,_DEF_SIZE);
						 sprintf_s(szsql,"update person set p_total=p_total+1 where u_id=%d",u_id);
						 f=m_sql.UpdateMySql(szsql);
						 //对等级进行更改判断是否升级 p_score是加完之后的分数 lststr是加完之后的分数
                          if(p_score/100==atoi(lststr.front().c_str())/100+1)
						  {    
							   memset(szsql,0,_DEF_SIZE);
							   sprintf_s(szsql,"update person set p_level=p_level+1 where u_id=%d",u_id);
							   m_sql.UpdateMySql(szsql);
						  }
						  else
						  {
							  //不用更改;
						  }
						  //判断是否胜利


						}
					}
				/*	ite->m_score=0;
					ite->m_flag=0;
					ite++;*/
					ite++;
					lststr.pop_front();
				}

				//利用map的自动排序可以知道map尾就是分数最高的也就是胜利的
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

				//找到每个房间中赢得人将赢得局数加一
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
			
			//一轮结束将标识位清空
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

	//这时一局游戏结束
	if(ssr.szResult == _start_end)
	{   
		//将标志置回
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


//准备 取消准备
void TCPKernel::ReadyRq(SOCKET sock,char* szbuf)
{
	STRU_READY_RQ *psgr = (STRU_READY_RQ*)szbuf;
	if(psgr->flag_ready == true) //请求准备
		m_listRoom[psgr->m_roomid-1].m_curReadyplayer++;
	else 
		m_listRoom[psgr->m_roomid-1].m_curReadyplayer--;

	STRU_READY_RS srr;
	srr.m_nType = _DEF_PROTOCOL_READY_RS;
	srr.flag_ready = psgr->flag_ready;
	m_pTcpNet->SendData(sock,(char*)&srr,sizeof(srr));
}


//注册成功添加到person表中
void TCPKernel::InsertPersonRq(SOCKET sock,char* szbuf)
{
	STRU_INSERTPERSON_RQ *psgr = (STRU_INSERTPERSON_RQ*)szbuf;
	
	//也要把这个人的个人信息添加到person数据库中
	list<string> lstint;
	char szsql[_DEF_SQLLEN] = {0};
	sprintf_s(szsql,"select u_id from player where p_name='%s'",psgr->m_szName);
	m_sql.SelectMySql(szsql,1,lstint);

	//如果找到了
	memset(szsql,0,_DEF_SQLLEN);
	if(lstint.size() != 0)
	{
		sprintf_s(szsql,"insert into person (u_id,p_name,p_id) values ('%d','%s','%lld')",
			atoi(lstint.front().c_str()),psgr->m_szName,psgr->m_tel);
		m_sql.UpdateMySql(szsql);
	}

}


//编辑个人信息
void TCPKernel::AlterPersonRq(SOCKET sock,char* szbuf)
{
	STRU_ALTERPERSON_RQ *psgr = (STRU_ALTERPERSON_RQ*)szbuf;

	STRU_ALTERPERSON_RS sar;
	sar.m_nType = _DEF_PROTOCOL_ALTERPERSON_RS;
	sar.szResult = _alterperson_success;

	//先找到这个人的唯一标识 u_id
	list<string> lstint;
	char szsql[_DEF_SQLLEN] = {0};
	sprintf_s(szsql,"select u_id from player where p_name='%s'",psgr->m_szName);
	m_sql.SelectMySql(szsql,1,lstint);
	int u_id = atoi(lstint.front().c_str());

	
	//1.先看看电话号改没改 电话号为0就不改电话号了
	//如果改电话号 那player表和person表的电话号都需要改
	if(psgr->m_tel != 0)
	{
		memset(szsql,0,_DEF_SQLLEN);
		sprintf_s(szsql,"update person set p_id = %lld where u_id = %d",psgr->m_tel,u_id);
		m_sql.UpdateMySql(szsql);
		memset(szsql,0,_DEF_SQLLEN);
		sprintf_s(szsql,"update player set p_id = %lld where u_id = %d",psgr->m_tel,u_id);
		m_sql.UpdateMySql(szsql);
	}

	//2.性别：0男1女
	if(psgr->sex == 0)
	{
		memset(szsql,0,_DEF_SQLLEN);
		sprintf_s(szsql,"update person set p_sex = '男' where u_id = %d",u_id);
		m_sql.UpdateMySql(szsql);
	}
	else if(psgr->sex == 1)
	{
		memset(szsql,0,_DEF_SQLLEN);
		sprintf_s(szsql,"update person set p_sex = '女' where u_id = %d",u_id);
		m_sql.UpdateMySql(szsql);
	}
	//3.出生日期
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

	if(psgr->b_yespw == TRUE) //修改密码
	{
		//1.匹配旧密码
		list<string> lstpw;
		memset(szsql,0,_DEF_SQLLEN);
		sprintf_s(szsql,"select p_pw from player where u_id = %d",u_id);
		m_sql.SelectMySql(szsql,1,lstpw);
		
		//如果输入的旧密码和数据库中的旧密码一致
		if(strncmp(lstpw.front().c_str(),psgr->m_szoldpw,_DEF_SIZE) == 0)
		{
			//2.看新密码和确认密码是否一致
			if(strncmp(psgr->m_sznewpw,psgr->m_sznewpw2,_DEF_SIZE) == 0)
			{
				//3.一致改写player表中的密码
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


//添加好友
void TCPKernel::GetFriendRq(SOCKET sock,char* szbuf)
{
	STRU_GETFRIEND_RQ * psgr=(STRU_GETFRIEND_RQ*)szbuf;

	STRU_GETFRIEND_RS sgr;
	strcpy(sgr.m_friendName,psgr->m_friendName);
	strcpy(sgr.m_szName,psgr->m_szName);
	sgr.m_nType=_DEF_PROTOCOL_GETFRIEND_RS;
	sgr.m_isagree=2;
	//找到对应的好友，然后给好友发送好友请求
	list<OnlinePlayerServer*>::iterator ite = m_lstOnlinePlayer.begin();
	while(ite != m_lstOnlinePlayer.end())
	{
		if(strncmp(psgr->m_friendName,(*ite)->m_szName,_DEF_SIZE) == 0)
			m_pTcpNet->SendData((*ite)->sock,(char*)&sgr,sizeof(sgr));
		ite ++;
	}
	
}


//添加好友回复
void TCPKernel::GetFriendRs(SOCKET sock,char* szbuf)
{
	STRU_GETFRIEND_RS * psgr=(STRU_GETFRIEND_RS*)szbuf;

	STRU_GETFRIEND_RS sgr;
	strcpy(sgr.m_friendName,psgr->m_friendName);
	strcpy(sgr.m_szName,psgr->m_szName);
	sgr.m_nType=_DEF_PROTOCOL_GETFRIEND_RS;
	sgr.m_isagree=psgr->m_isagree;
	
	if(psgr->m_isagree == 0)//同意添加
	{
		//把两个人分别添加到数据库中
		//先找到这个人的唯一标识 u_id
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
		//通过u_id添加好友
		sprintf_s(szsql,"insert into friend values(%d,%d)",u_id,friend_id);
		b = m_sql.UpdateMySql(szsql);
		memset(szsql,0,_DEF_SQLLEN);
		sprintf_s(szsql,"insert into friend values(%d,%d)",friend_id,u_id);
		b = m_sql.UpdateMySql(szsql);

		//给两个客户端发协议包
		list<OnlinePlayerServer*>::iterator ite = m_lstOnlinePlayer.begin();
		while(ite != m_lstOnlinePlayer.end())
		{
			//给请求添加好友的人发送回复包
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

			//给发送包的人发送请求包
			if(strncmp(sgr.m_szName,(*ite)->m_szName,_DEF_SIZE) == 0)
			{
				m_pTcpNet->SendData((*ite)->sock,(char*)&sgr,sizeof(sgr));
				ite ++;
				continue;
			}
			ite ++;
		}

	
	}
	else if(psgr->m_isagree == 1)// 拒绝添加
	{
		
		//给两个客户端发协议包
		list<OnlinePlayerServer*>::iterator ite = m_lstOnlinePlayer.begin();
		while(ite != m_lstOnlinePlayer.end())
		{
			//给好友发送请求包
			if(strncmp(psgr->m_friendName,(*ite)->m_szName,_DEF_SIZE) == 0)
			{
				char temp_name[_DEF_SIZE];
				strcpy(temp_name,psgr->m_friendName);
				strcpy(psgr->m_friendName,psgr->m_szName);
				strcpy(psgr->m_szName,temp_name);
				m_pTcpNet->SendData((*ite)->sock,(char*)&psgr,sizeof(psgr));
			}


			//给自己发送回复包
			if(strncmp(psgr->m_szName,(*ite)->m_szName,_DEF_SIZE) == 0)
			{
				m_pTcpNet->SendData((*ite)->sock,(char*)&psgr,sizeof(psgr));
			}
			ite ++;
		}

	
	}

}


//查看大厅玩家资料
void TCPKernel::LookPersonRq(SOCKET sock,char* szbuf)
{
	STRU_LOOKPERSON_RQ * psgr=(STRU_LOOKPERSON_RQ*)szbuf;

	//先找到这个人的唯一标识 u_id
	list<string> lstint;
	char szsql[_DEF_SQLLEN] = {0};
	sprintf_s(szsql,"select u_id from player where p_name='%s'",psgr->m_szWantLookName);
	m_sql.SelectMySql(szsql,1,lstint);
	int u_id = atoi(lstint.front().c_str());

	STRU_LOOKPERSON_RS slr;
	slr.m_nType = _DEF_PROTOCOL_LOOKPERSON_RS;
	strncpy(slr.m_szWantLookName,psgr->m_szWantLookName,_DEF_SIZE);
	
	//电话号码
	memset(szsql,0,_DEF_SQLLEN);
	list<string> lsttel;
	sprintf_s(szsql,"select p_id from person where u_id = %d",u_id);
	m_sql.SelectMySql(szsql,1,lsttel);
	strncpy(slr.p_id,lsttel.front().c_str(),_DEF_SIZE);

	//性别
	memset(szsql,0,_DEF_SQLLEN);
	list<string> lstsex;
	sprintf_s(szsql,"select p_sex from person where u_id = %d",u_id);
	m_sql.SelectMySql(szsql,1,lstsex);
	if(strncmp(lstsex.front().c_str(),"男",_DEF_SIZE) == 0) //性别：男0女1
		slr.sex = 0;
	else 
		slr.sex = 1;

	//出生年月日
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

	//分数 等级
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

	//胜率
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

	//把两个人分别添加到数据库中
		//先找到这个人的唯一标识 u_id

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
		//通过u_id删除好友
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

			//给两个客户端发协议包
		list<OnlinePlayerServer*>::iterator ite = m_lstOnlinePlayer.begin();
		while(ite != m_lstOnlinePlayer.end())
		{
			//给请求添加好友的人发送回复包
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

			//给发送包的人发送请求包
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

	//提取本人的名字
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


//邀请好友
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
		//发给邀请的人
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
		//发给被邀请的人
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
		//找到大厅中被邀请这个人的sock
		auto ite1 = m_lstOnlinePlayer.begin();
		while(ite1 != m_lstOnlinePlayer.end())
		{
			if(strncmp((*ite1)->m_szName,psir->m_szNameInvite,_DEF_SIZE) == 0)
			{
				break;
			}
			ite1 ++;
		}

		//查看这个房间是否已经开始游戏
		if(m_listRoom[psir->m_roomid-1].start_flag == false)
		{
			//查看人满了没有：人没满
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

	//拒绝接受邀请
	else if(psir->szResult == _inviteplayer_refuse)
	{
		sir.szResult = _inviteplayer_refuse;
		auto ite = m_listRoom[psir->m_roomid-1].m_listInRoomPlayer.begin();
		while(ite != m_listRoom[psir->m_roomid-1].m_listInRoomPlayer.end())
		{
			//通知邀请的人：你被拒绝了
			if(strncmp((*ite).m_szName,psir->m_szName,_DEF_SIZE) == 0)
			{
				m_pTcpNet->SendData((*ite).sock,(char*)&sir,sizeof(sir));
				break;
			}
			ite ++;
		}

	}


}


//视频
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
		//存入m_lstVedioPlayer
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

		//给线程池投递任务
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
	
	//找这个sock的主播信息结构体
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

	//释放相应个数的信号量
	ReleaseSemaphore(pv->m_hSemaphore,1,NULL);

}

//文件
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
		//如果这人在线 才可以发送文件
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
	//接收到匹配请求
	STRU_GETTEAM_RQ *pssr = (STRU_GETTEAM_RQ *)szbuf;

	GetTeamPlayer* gtp = (GetTeamPlayer*)malloc(sizeof(GetTeamPlayer));
	strncpy(gtp->m_szName,pssr->m_szName,_DEF_SIZE);
	gtp->m_sock=sock;

	//把请求匹配的人放进匹配池中  hashmap 先到数据库中找到该人地等级 
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
	//然后根据等级放到map中
	m_mapleveltoPlayerQueue[m_level].push_back(gtp);
	//把此人放到带匹配池中
	m_mapGetTeamPlayer[gtp];

	//给该请求匹配的人创建一根线程。通过这个线程去匹配池中找到合适的玩家，为了不冲突，需要加锁
	Itask* pnew2 = new GetTeamTask(gtp,this);
	m_getteamthreadpool.Push(pnew2);


}