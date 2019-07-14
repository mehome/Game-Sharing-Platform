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
//������Ϣ�ṹ��
struct  Room_info
{
	HANDLE m_hSemaphore;

	int m_roomid;
	int m_maxplayer;
	int m_curplayer;
	int m_curReadyplayer;
	int m_winplayer;//��¼�¶Ե�����
	int m_max;//����һ����Ϸ���õ�������

	bool start_flag;
	char answer[_DEF_SIZE];
	bool issit[_DEF_ROOMNUM];
	int sitidc[_DEF_ROOMNUM];
	list<InRoomPlayerServer> m_listInRoomPlayer;

	std::map <string,int> m_drawer; //��������->������
	char p[_DEF_SIZE];//�����˵�����
};

struct Player_Info
{
	SOCKET m_sock;
	HANDLE m_hEvent;
};

//��Ƶ
struct PlayerVedio_Info
{
	char m_szName[_DEF_SIZE];
	SOCKET m_sock;
	HANDLE m_hSemaphore;
	char   m_szAuthorBuffer[_DEF_AUTHORSIZE];//����Ƶ��buffer
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
	void StartRq(SOCKET sock,char* szbuf); //����ʼ��Ϸ
	void ReadyRq(SOCKET sock,char* szbuf); //׼�� ȡ��׼��
	void InsertPersonRq(SOCKET sock,char* szbuf); //ע��ɹ���ӵ�person����
	void AlterPersonRq(SOCKET sock,char* szbuf); //�༭������Ϣ
	void GetFriendRq(SOCKET sock,char* szbuf); //��Ӻ�������
	void GetFriendRs(SOCKET sock,char* szbuf); //��Ӻ��ѻظ�
	void LookPersonRq(SOCKET sock,char* szbuf); //�鿴�����������
	void DeleteFriendRq(SOCKET sock,char* szbuf);//ɾ����������
	void PersonChatRq(SOCKET sock,char* szbuf); //˽��
	void InvitePlayerRq(SOCKET sock,char* szbuf); //������Ϸ
	void InvitePlayerYnRq(SOCKET sock,char* szbuf); //�ظ������� �ܾ����ǽ���
	void InviteVedioRq(SOCKET sock,char* szbuf); //ѯ���Ƿ����������Ƶ
	void InviteVedioRs(SOCKET sock,char* szbuf); 
	void VedioStreamInfoRq(SOCKET sock,char* szbuf);
	void SendFileRq(SOCKET sock,char* szbuf); //�����ļ�����
	void SendFileBlockRq(SOCKET sock,char* szbuf); //�����ļ�������
	void GetTeamRq(SOCKET sock,char* szbuf);//ƥ�����

public:
	CMySql m_sql;
	CMyThreadpool m_threadpool;
	CMyThreadpool m_getteamthreadpool;
	INet* m_pTcpNet;
	
	std::list<OnlinePlayerServer* > m_lstOnlinePlayer; //��ʾ���������������
	Room_info m_listRoom[_DEF_ROOMNUM]; //��¼���з�����Ϣ������ _DEF_ROOMNUM = 6
	std::map<PlayerVedio_Info* ,PlayerVedioReceive_Info* > m_mapVedioPlayer; //��Ƶ����Ҷ�

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