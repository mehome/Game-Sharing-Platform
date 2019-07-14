#ifndef  _PACKDEF_H
#define	 _PACKDEF_H

#include<list>
using namespace std;

typedef char PackType;


//#define BEGIN_PROTOCOL_MAP static const ProtocolMap m_ProtocolMapEntries[]= \
//{
//
//#define END_PROTOCOL_MAP 	{0,0} \
//};
//
//#define PM(X,Y)  {X,Y},


//------------------------------------------------------------------------------------------------
//注册
#define _register_telexists      0
#define _register_userexists     1
#define _register_success        2

//登录
#define _login_usernoexists      0
#define _login_passworderr       1
#define _login_success           2

//进入房间 获取房间信息
#define _getroominfo_fail        0
#define _getroominfo_success     1

//角色
#define role_player             0

//退出结果
#define _quit_success 0
#define _quit_fail    1
#define _quit_others  2

//请求开始游戏
#define _start_success 0
#define _start_people  1 //房间只有一个人 无法开始游戏
#define _start_fail    2 //房间有两个或两个人以上 但是没有全部玩家准备完成
#define _start_end     3 //一轮游戏结束

//修改个人信息
#define _alterperson_success 0
#define _alterperson_pwfail  1

//邀请大厅玩家游戏
#define _inviteplayer_younotinroom  0
#define _inviteplayer_success       1

#define _inviteplayer_agree         2
#define _inviteplayer_refuse        3
#define _inviteplayer_roomisbegin   4
#define _inviteplayer_roomisfull    5

//询问是否接受视频邀请回复结果
#define _invitevedio_agree     0
#define _invitevedio_refuse    1

//--------------------------------------------------------------------------------------------------
//默认值
#define _DEF_SIZE			64
#define _DEF_SQLLEN        300
#define _DEF_ROOMNUM         20
#define _DEF_PLAYER         36
#define _DEF_IMAGE        4096
//文件
#define MAX_PATH			         260
#define MAX_LEN				          40
#define MAX_STHRESH        100*1024*1024
#define MAX_FILEBLOCK_LEN            500
#define _DEF_STREAMSIZE			    1000
//视频
#define _DEF_AUTHORSIZE          1072*10000
#define _DEF_VIDEOSIZE                 1072
#define _DEF_BUFFERNUM                10000

//--------------------------------------------------------------------------------------------------
//协议
#define _DEF_PROTOCOL_BASE		50
#define _DEF_PACK_COUNT         70

//注册
#define _DEF_PROTOCOL_REGISTER_RQ		_DEF_PROTOCOL_BASE +1
#define _DEF_PROTOCOL_REGISTER_RS		_DEF_PROTOCOL_BASE +2

//登录
#define _DEF_PROTOCOL_LOGIN_RQ			_DEF_PROTOCOL_BASE +3
#define _DEF_PROTOCOL_LOGIN_RS			_DEF_PROTOCOL_BASE +4

//显示当前大厅在线玩家
#define _DEF_PROTOCOL_ONLINE_RQ         _DEF_PROTOCOL_BASE +5
#define _DEF_PROTOCOL_ONLINE_RS         _DEF_PROTOCOL_BASE +6

//退出按钮
#define _DEF_PROTOCOL_QUIT_RQ           _DEF_PROTOCOL_BASE +7
#define _DEF_PROTOCOL_QUIT_RS           _DEF_PROTOCOL_BASE +8

//大厅聊天
#define _DEF_PROTOCOL_CHAT_RQ           _DEF_PROTOCOL_BASE +9
#define _DEF_PROTOCOL_CHAT_RS           _DEF_PROTOCOL_BASE +10

//进入房间 信息获取请求
#define _DEF_PROTOCOL_GETROOM_RQ		_DEF_PROTOCOL_BASE +11
#define _DEF_PROTOCOL_GETROOM_RS		_DEF_PROTOCOL_BASE +12

//退出房间
#define _DEF_PROTOCOL_QUITROOM_RQ		_DEF_PROTOCOL_BASE +13
#define _DEF_PROTOCOL_QUITROOM_RS		_DEF_PROTOCOL_BASE +14

//房间聊天
#define _DEF_PROTOCOL_CHATROOM_RQ		_DEF_PROTOCOL_BASE +15
#define _DEF_PROTOCOL_CHATROOM_RS		_DEF_PROTOCOL_BASE +16

//发送点坐标
#define _DEF_PROTOCOL_GETPOINT_RQ		_DEF_PROTOCOL_BASE +17
#define _DEF_PROTOCOL_GETPOINT_RS		_DEF_PROTOCOL_BASE +18

//请求开始
#define _DEF_PROTOCOL_START_RQ		    _DEF_PROTOCOL_BASE +19
#define _DEF_PROTOCOL_START_RS		    _DEF_PROTOCOL_BASE +20

//换肤
#define _DEF_PROTOCOL_GETSIT_RS		    _DEF_PROTOCOL_BASE +21


//准备 取消准备
#define _DEF_PROTOCOL_READY_RQ          _DEF_PROTOCOL_BASE +22
#define _DEF_PROTOCOL_READY_RS          _DEF_PROTOCOL_BASE +23

//结束一个人的游戏
#define _DEF_PROTOCOL_GAMEOVER_RS       _DEF_PROTOCOL_BASE +24

//修改个人信息协议包
#define _DEF_PROTOCOL_ALTERPERSON_RQ    _DEF_PROTOCOL_BASE +25
#define _DEF_PROTOCOL_ALTERPERSON_RS    _DEF_PROTOCOL_BASE +26

//每当有一个人注册成功 就添加到数据库的person数据库中
#define _DEF_PROTOCOL_INSERTPERSON_RQ   _DEF_PROTOCOL_BASE +27

//请求添加好友 
#define _DEF_PROTOCOL_GETFRIEND_RQ       _DEF_PROTOCOL_BASE +28
#define _DEF_PROTOCOL_GETFRIEND_RS       _DEF_PROTOCOL_BASE +29


//获取数据库中的所有好友
#define _DEF_PROTOCOL_GETLISTFRIEND_RQ    _DEF_PROTOCOL_BASE +30
#define _DEF_PROTOCOL_GETLISTFRIEND_RS    _DEF_PROTOCOL_BASE +31


//查看大厅在线玩家资料
#define _DEF_PROTOCOL_LOOKPERSON_RQ       _DEF_PROTOCOL_BASE +32
#define _DEF_PROTOCOL_LOOKPERSON_RS       _DEF_PROTOCOL_BASE +33

//删除好友
#define _DEF_PROTOCOL_DELETEFRIEND_RQ     _DEF_PROTOCOL_BASE +34
#define _DEF_PROTOCOL_DELETEFRIEND_RS     _DEF_PROTOCOL_BASE +35


//私信
#define _DEF_PROTOCOL_PERSONCHAT_RQ		  _DEF_PROTOCOL_BASE +36
#define _DEF_PROTOCOL_PERSONCHAT_RS		  _DEF_PROTOCOL_BASE +37


//邀请大厅的玩家进行游戏
#define _DEF_PROTOCOL_INVITEPLAYER_RQ     _DEF_PROTOCOL_BASE +38
#define _DEF_PROTOCOL_INVITEPLAYER_RS     _DEF_PROTOCOL_BASE +39


//同意或者拒绝
#define _DEF_PROTOCOL_INVITEPLAYERYN_RQ   _DEF_PROTOCOL_BASE +40
#define _DEF_PROTOCOL_INVITEPLAYERYN_RS   _DEF_PROTOCOL_BASE +41


//询问是否接受视频邀请
#define _DEF_PROTOCOL_INVITEVEDIO_RQ      _DEF_PROTOCOL_BASE +42
#define _DEF_PROTOCOL_INVITEVEDIO_RS      _DEF_PROTOCOL_BASE +43

//上传文件
#define _DEF_PROTOCOL_SENDFILE_RQ		_DEF_PROTOCOL_BASE       +44
#define _DEF_PROTOCOL_SENDFILE_RS		_DEF_PROTOCOL_BASE 		 +45

//上传文件块
#define _DEF_PROTOCOL_SENDFILEBLOCK_RQ		_DEF_PROTOCOL_BASE 	 +46
#define _DEF_PROTOCOL_SENDFILEBLOCK_RS		_DEF_PROTOCOL_BASE 	 +47

//下载文件块
#define _DEF_PROTOCOL_RECVFILEBLOCK_RQ		_DEF_PROTOCOL_BASE   +48
#define _DEF_PROTOCOL_RECVFILEBLOCK_RS		_DEF_PROTOCOL_BASE   +49


////推流
//#define _DEF_PROTOCOL_STARTTRANSFER_RQ		 _DEF_PROTOCOL_BASE  +50
//#define _DEF_PROTOCOL_STARTTRANSFER_RS		 _DEF_PROTOCOL_BASE  +51
////停止推流														 
//#define _DEF_PROTOCOL_STOPTRANSFER_RQ		_DEF_PROTOCOL_BASE 	     +52
//#define _DEF_PROTOCOL_STOPTRANSFER_RS		_DEF_PROTOCOL_BASE 	     +53

//视频流内容														
#define _DEF_PROTOCOL_VIDEOSTREAMINFO_RQ    _DEF_PROTOCOL_BASE 	 +54
#define _DEF_PROTOCOL_VIDEOSTREAMINFO_RS    _DEF_PROTOCOL_BASE 	 +55
//视频流														 
#define _DEF_PROTOCOL_VIDEOSTREAM_RQ		_DEF_PROTOCOL_BASE 	 +56
#define _DEF_PROTOCOL_VIDEOSTREAM_RS		_DEF_PROTOCOL_BASE 	 +57

//快速组队
#define _DEF_PROTOCOL_GETTEAM_RQ		    _DEF_PROTOCOL_BASE 	 +58
#define _DEF_PROTOCOL_GETTEAM_RS		    _DEF_PROTOCOL_BASE 	 +59

//-------------------------------------------------------------------------------------------------
//结构体声明

//玩家
struct Player
{
	char m_szName[_DEF_SIZE];
	char m_szPassword[_DEF_SIZE];
	long long m_tel;
	int m_grade;     //分数
	int m_role;      //角色
	int m_tablenum;  //所在桌号
};

//大厅在线玩家结点 服务器
struct OnlinePlayerServer
{
	char m_szName[_DEF_SIZE];
	SOCKET sock;
};
struct OnlinePlayer
{
	char m_szName[_DEF_SIZE];
};

//房间在线玩家结点 服务器
struct InRoomPlayerServer
{
	char m_szName[_DEF_SIZE];
	SOCKET sock;
	int m_score;
	int m_flag;//如果同一人猜两次  只加一次分数

};
struct InRoomPlayer
{
	char m_szName[_DEF_SIZE];
};

//房间
struct	Room 
{
	int m_roomid;
	int m_maxplayer; //最大人数
	int m_curplayer; //当前人数
	list<OnlinePlayerServer> m_listplayer; //房间里的玩家链表
};

//管理员
struct Root
{
	list<struct Room> m_listroom;
	int m_roomnum;
}; 

//---------------------------------------------------------------------------------------------
//注册请求
struct STRU_REGISTER_RQ
{
	 PackType m_nType;
	 char m_szName[_DEF_SIZE];
	 long long m_tel;
	 char m_szPassword[_DEF_SIZE];
	 char  m_role;
};
//注册回复
struct STRU_REGISTER_RS
{
	 PackType m_nType;
	 char m_szName[_DEF_SIZE];
	 long long m_tel;
	 char m_szResult;
};

//登录请求
struct STRU_LOGIN_RQ
{
	 PackType m_nType;
	 char m_szName[_DEF_SIZE];
	 char m_szPassword[_DEF_SIZE];
};

//登录回复
struct STRU_LOGIN_RS
{
	 PackType m_nType;
	 char m_szName[_DEF_SIZE];
	 char m_szResult;
	 int sit[_DEF_PLAYER];
};

//显示在线大厅玩家列表 请求
struct STRU_SHOWPLAYER_RQ
{
	PackType m_nType;
};
//回复
struct STRU_SHOWPLAYER_RS
{
	PackType m_nType;
	OnlinePlayer online[_DEF_PLAYER];
};

//退出请求
struct STRU_QUIT_RQ
{
	PackType m_nType;
	char m_szName[_DEF_SIZE];
};
//退出回复
struct STRU_QUIT_RS
{
	PackType m_nType;
	char szResult;
};

//聊天发送请求
struct STRU_CHAT_RQ
{
	PackType m_nType;
	char m_szName[_DEF_SIZE];
	char m_chatbuf[_DEF_SQLLEN];
};
//聊天回复
struct STRU_CHAT_RS
{
	PackType m_nType;
	char m_szName[_DEF_SIZE];
	char m_chatbuf[_DEF_SQLLEN];
};


//点击座位进入房间 获取房间信息
struct STRU_GETINTOROOM_RQ
{
	PackType m_nType;
	int m_roomid;
	int m_sitid;
	int m_buttonid;
	char m_playername[_DEF_SIZE];
};
struct STRU_GETINTOROOM_RS
{
	PackType m_nType;
	int m_roomid;
	int m_sitid;
	int m_buttonid;
	InRoomPlayer inroom[_DEF_ROOMNUM];
	char szResult;
};
struct STRU_GETSIT_RS
{
	PackType m_nType;
	bool flag;
	int buttonid;
};

//退出房间请求
struct STRU_QUITROOM_RQ
{
	PackType m_nType;
	int m_roomid;
	int sitidc;
	int sitid;
	char m_szName[_DEF_SIZE];
	bool flag_ready;
};
struct STRU_QUITROOM_RS
{
	PackType m_nType;
	char m_szName[_DEF_SIZE]; //退出人名
	InRoomPlayer inroom[_DEF_ROOMNUM];
	char szResult;
};

//房间聊天请求
struct STRU_CHATROOM_RQ
{
	PackType m_nType;
	int m_roomid;
	char m_szName[_DEF_SIZE];
	char m_chatbuf[_DEF_SQLLEN];
	bool flag;
};

//聊天回复
struct STRU_CHATROOM_RS
{
	PackType m_nType;
	char m_szName[_DEF_SIZE];
	char m_chatbuf[_DEF_SQLLEN];
	bool m_istrue; //判断猜的对不对
	//bool m_isstart; //查看房间是否开始游戏
	int m_score;//记录该人得到的分数
	char m_dszName[_DEF_SIZE];//画的人名
	int m_dscore; //画的人分数
};


//发送点
struct STRU_GETPOINT_RQ
{
	PackType m_nType;
	char m_szName[_DEF_SIZE];
	int m_roomid;
	POINT point;
	POINT movepoint;
	COLORREF color;
};
//发送给除画图的人之外的
struct STRU_GETPOINT_RS
{
	PackType m_nType;
	POINT point;
	POINT movepoint;
	COLORREF color;
};


//请求开始游戏
struct STRU_START_RQ
{
	PackType m_nType;
	char m_szName[_DEF_SIZE];
	int m_roomid;
};
struct STRU_START_RS
{
	PackType m_nType;
	char m_chatbuf[_DEF_SQLLEN];
	int m_curplayer;
	char szResult;
	char szDrawplayername[_DEF_SIZE];
};


//准备 取消准备
struct STRU_READY_RQ
{
	PackType m_nType;
	bool flag_ready; //true为请求准备 false为请求取消准备
	int m_roomid;
};

struct STRU_READY_RS
{
	PackType m_nType;
	bool flag_ready; //true为准备 false为取消准备
};

//结束一个人的游戏
struct STRU_GAMEOVER_RS
{
	PackType m_nType;//游戏结束标志
};
struct STRU_GAMEOVER_RQ
{
	PackType m_nType;
};


//修改个人信息协议包
struct STRU_ALTERPERSON_RQ
{
	PackType m_nType;
	char m_szName[_DEF_SIZE];
	long long m_tel;
	char sex;

	int m_Year;
	int m_Month;
	int m_Day;

	BOOL b_yespw;
	char m_szoldpw[_DEF_SIZE];
	char m_sznewpw[_DEF_SIZE];
	char m_sznewpw2[_DEF_SIZE];
	//char m_image[_DEF_IMAGE];
};
struct STRU_ALTERPERSON_RS
{
	PackType m_nType;
	char szResult;
};


struct STRU_INSERTPERSON_RQ
{
	PackType m_nType;
	char m_szName[_DEF_SIZE];
	long long m_tel;
};

//添加好友请求协议包
struct STRU_GETFRIEND_RQ
{
	PackType m_nType;
	char m_szName[_DEF_SIZE];
	char m_friendName[_DEF_SIZE];
	
};
//添加好友回复协议包
struct STRU_GETFRIEND_RS
{
	PackType m_nType;
	char m_szName[_DEF_SIZE];
	char m_friendName[_DEF_SIZE];
	int m_isagree;//是否同意添加
	//0---同意 1---不同意 2---待定

};

struct STRU_GETLISTFRIEND_RQ
{
	PackType m_nType;
	char m_szName[_DEF_SIZE];
};

struct STRU_GETLISTFRIEND_RS
{
	PackType m_nType;
	char m_szName[_DEF_SIZE];
	char m_friendName[_DEF_SIZE];

};

//查看大厅玩家资料
struct STRU_LOOKPERSON_RQ
{
	PackType m_nType;
	char m_szWantLookName[_DEF_SIZE];
};
struct STRU_LOOKPERSON_RS
{
	PackType m_nType;
	char m_szWantLookName[_DEF_SIZE];
	char p_id[_DEF_SIZE];
	char sex; //性别：男0女1
	int m_Year;
	int m_Month;
	int m_Day;
	int m_score;
	int m_level;
	float m_winpercent;
};


struct STRU_DELETEFRIEND_RQ
{
	PackType m_nType;
	char m_szName[_DEF_SIZE];
	char m_deleteName[_DEF_SIZE];
};

struct STRU_DELETEFRIEND_RS
{
	PackType m_nType;
	char m_szName[_DEF_SIZE];
	char m_deleteName[_DEF_SIZE];
	bool is_delete;
};


//私信
//聊天发送请求
struct STRU_PERSONCHAT_RQ
{
	PackType m_nType;
	char m_szName[_DEF_SIZE];
	char m_szNameChat[_DEF_SIZE];
	char m_chatbuf[_DEF_SQLLEN];
};
//聊天回复
struct STRU_PERSONCHAT_RS
{
	PackType m_nType;
	char m_szName[_DEF_SIZE];
	char m_chatbuf[_DEF_SQLLEN];
};


//邀请大厅在线玩家游戏
struct STRU_INVITEPLAYER_RQ
{
	PackType m_nType;
	char m_szName[_DEF_SIZE];
	char m_szNameInvite[_DEF_SIZE];
	int m_roomid;
};
struct STRU_INVITEPLAYER_RS
{
	PackType m_nType;
	char m_szName[_DEF_SIZE];
	char m_szNameInvite[_DEF_SIZE];
	int m_roomid;
	char szResult;
};

//同意 或者拒绝 通知两个人
struct STRU_INVITEPLAYERYN_RQ
{
	PackType m_nType;
	char m_szName[_DEF_SIZE];
	char m_szNameInvite[_DEF_SIZE];
	int m_roomid;
	char szResult;
};
struct STRU_INVITEPLAYERYN_RS
{
	PackType m_nType;
	char szResult;
	int m_roomid;
	int m_sitid;
};

//询问是否接受视频邀请
struct STRU_INVITEVEDIO_RQ
{
	PackType m_nType;
	char m_szName[_DEF_SIZE];
	char m_szNameInvite[_DEF_SIZE];
};
struct STRU_INVITEVEDIO_RS
{
	PackType m_nType;
	char m_szName[_DEF_SIZE];
	char m_szNameInvite[_DEF_SIZE];
	char szResult;
};


//文件
struct STRU_SENDFILE_RQ
{
	PackType  m_nType;
	char m_szName[_DEF_SIZE];//发送人名
	char m_szNameChat[_DEF_SIZE];
	char m_szFileName[MAX_PATH];
	long long m_lFileSize;
	char    m_szMD5[MAX_LEN];

};
struct STRU_SENDFILE_RS
{
	PackType  m_nType;
	char m_szName[_DEF_SIZE];//发送人名
	char m_szNameChat[_DEF_SIZE];
	char m_szFileName[MAX_PATH];
	bool m_lresult;
	long long m_nPos;

};
struct STRU_SENDFILEBLOCK_RQ
{
	PackType  m_nType;
	char m_szName[_DEF_SIZE];//发送人名
	char m_szNameChat[_DEF_SIZE];
	char m_szFileName[MAX_PATH];
	char m_szFileContent[MAX_FILEBLOCK_LEN];
	int  m_nlen;
};
struct STRU_SENDFILEBLOCK_RS
{
	PackType  m_nType;
	char m_szName[_DEF_SIZE];//发送人名
	char m_szNameChat[_DEF_SIZE];
	char m_szFileName[MAX_PATH];
	char m_szFileContent[MAX_FILEBLOCK_LEN];
	long  m_lresult;
	long long m_nlen;
};

//开始推流请求
struct STRU_STARTTRANSFER_RQ
{
	 PackType m_nType;
	 char m_szName[_DEF_SIZE];	
	 char m_szNameChat[_DEF_SIZE];
};
//开始推流回复
struct STRU_STARTTRANSFER_RS
{
	 PackType m_nType;
	 char m_szName[_DEF_SIZE];	
	 char szResult;
};
//视频流信息
struct STRU_VIDEOSTREAM_RQ
{
	 PackType m_nType;
	 char m_szName[_DEF_SIZE];	
	 char m_szStream[_DEF_STREAMSIZE];
	 int  m_nlen;
};
//组队请求
struct STRU_GETTEAM_RQ
{
	 PackType m_nType;
	 char m_szName[_DEF_SIZE];	
};
struct STRU_GETTEAM_RS
{
	 PackType m_nType;
	 char szResult;
	 int m_roomid;
	// char m_szName[_DEF_SIZE];	
};
#endif