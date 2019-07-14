#ifndef _TCPKERNEL_H
#define _TCPKERNEL_H

#include"IKernel.h"
#include"TCPNet.h"
#include"CMySql.h"
#include"Packdef.h"
#include"MyThreadPool.h"
#include <list>
#include <map>
#include <queue>
#include <string>

//------------------------------------------------------------------------------------------
//房间信息结构体
struct  Room_info
{
	HANDLE m_hSemaphore;

	int m_roomid;
	int m_maxplayer;
	int m_curplayer;
	int m_curReadyplayer;
	int m_winplayer;//记录猜对的人数
	int m_max;//房间一局游戏后获得的最大分数

	bool start_flag;
	char answer[_DEF_SIZE];
	bool issit[_DEF_ROOMNUM];
	int sitidc[_DEF_ROOMNUM];
	list<InRoomPlayerServer> m_listInRoomPlayer;

	std::map <string,int> m_drawer; //待画的人->轮流画
	char p[_DEF_SIZE];//画的人的名字
};

struct Player_Info
{
	SOCKET m_sock;
	HANDLE m_hEvent;
};

//视频
struct PlayerVedio_Info
{
	char m_szName[_DEF_SIZE];
	SOCKET m_sock;
	HANDLE m_hSemaphore;
	char   m_szAuthorBuffer[_DEF_AUTHORSIZE];//存视频的buffer
	int    m_noffset;

};
struct PlayerVedioReceive_Info
{
	char m_szName[_DEF_SIZE];
	SOCKET m_sock;
	HANDLE m_hEvent;
	int    m_noffset;
};

struct GetTeamPlayer
{
	char m_szName[_DEF_SIZE];
	SOCKET m_sock; 
	int level;
	bool isget;

};
//struct ProtocolMap
//{
//    PackType m_nType;
//	PFUN     m_pfun;
//};

class TCPKernel:public IKernel
{
public:
	TCPKernel();
	virtual~TCPKernel();
public:
	bool Open();
	void Close();
	void DealData(SOCKET sock,char* szbuf);
public:
	void RegisterRq(SOCKET sock,char* szbuf);
	void LoginRq(SOCKET sock,char* szbuf);
	void SendOnlinePlayerListRq(SOCKET sock,char* szbuf);
	void QuitRq(SOCKET sock,char* szbuf);
	void ChatRq(SOCKET sock,char* szbuf);
	void GetRoomRq(SOCKET sock,char* szbuf);
	void QuitRoomRq(SOCKET sock,char* szbuf);
	void ChatRoomRq(SOCKET sock,char* szbuf);
	void GetPointRq(SOCKET sock,char* szbuf);
	void StartRq(SOCKET sock,char* szbuf); //请求开始游戏
	void ReadyRq(SOCKET sock,char* szbuf); //准备 取消准备
	void InsertPersonRq(SOCKET sock,char* szbuf); //注册成功添加到person表中
	void AlterPersonRq(SOCKET sock,char* szbuf); //编辑个人信息
	void GetFriendRq(SOCKET sock,char* szbuf); //添加好友请求
	void GetFriendRs(SOCKET sock,char* szbuf); //添加好友回复
	void LookPersonRq(SOCKET sock,char* szbuf); //查看大厅玩家资料
	void DeleteFriendRq(SOCKET sock,char* szbuf);//删除好友请求
	void PersonChatRq(SOCKET sock,char* szbuf); //私信
	void InvitePlayerRq(SOCKET sock,char* szbuf); //邀请游戏
	void InvitePlayerYnRq(SOCKET sock,char* szbuf); //回复邀请结果 拒绝还是接受
	void InviteVedioRq(SOCKET sock,char* szbuf); //询问是否接受邀请视频
	void InviteVedioRs(SOCKET sock,char* szbuf); 
	void VedioStreamInfoRq(SOCKET sock,char* szbuf);
	void SendFileRq(SOCKET sock,char* szbuf); //发送文件请求
	void SendFileBlockRq(SOCKET sock,char* szbuf); //发送文件块请求
	void GetTeamRq(SOCKET sock,char* szbuf);//匹配组队

public:
	CMySql m_sql;
	CMyThreadpool m_threadpool;
	CMyThreadpool m_getteamthreadpool;
	INet* m_pTcpNet;
	
	std::list<OnlinePlayerServer* > m_lstOnlinePlayer; //显示大厅在线玩家链表
	Room_info m_listRoom[_DEF_ROOMNUM]; //记录所有房间信息的数组 _DEF_ROOMNUM = 6
	std::map<PlayerVedio_Info* ,PlayerVedioReceive_Info* > m_mapVedioPlayer; //视频的玩家对

	std::list<Player_Info*> m_lstPlayer;
	std::map<SOCKET,Player_Info*> m_mapSocketToPlayerInfo;
	std::map<SOCKET,Room_info*> m_mapSocketToRoomInfo;
	std::map<Room_info*,std::list<Player_Info*>> m_mapRoomToPlayerList;
	std::map<int,std::list<GetTeamPlayer*>> m_mapleveltoPlayerQueue;
	std::map<GetTeamPlayer*,std::list<GetTeamPlayer*>> m_mapGetTeamPlayer;
	MyLock my_lock;
};

//--------------------------------------------------------------------------------------------

class TCPKernel;
typedef void (TCPKernel:: *PFUN)(SOCKET ,char*);

//----------------------------------------------------------------------------------------

class VedioItask :public Itask
{
public:
	VedioItask(PlayerVedioReceive_Info* pvr,PlayerVedio_Info* pv,TCPKernel* pKernel)
	{
		m_pvr = pvr;
		m_pv = pv;
		m_pKernel = pKernel; 
	}
	~VedioItask(){}
	void RunItask();
public:
	PlayerVedioReceive_Info* m_pvr;
	PlayerVedio_Info* m_pv;
	TCPKernel* m_pKernel;

};

class GetTeamTask:public Itask
{
public:
	GetTeamTask(GetTeamPlayer* gp,TCPKernel* pKernel)
	{
		m_gp = gp;
		m_pKernel = pKernel; 
	}
	~GetTeamTask(){}
	void RunItask();
public:
	GetTeamPlayer* m_gp;
	TCPKernel* m_pKernel;

};

#endif