#include <stdio.h>
#include <winsock2.h>
#pragma comment ( lib, "ws2_32.lib" )

int main()
{
	printf("|=============================================|\n");
	printf("|                                             |\n");
	printf("|                  客户端                     |\n");
	printf("|                                             |\n");
	printf("|=============================================|\n");
	
	int len=1024;
	char Server_IP[20]={'0'};
	char recvBuf[1024]="\0";
	char sendBuf[1024]="\0";
	char path[100]={'0'};
	
	printf("请输入服务端IP:");
	gets(Server_IP);
	WORD wVersion = MAKEWORD(2, 0);   //0x0200  设置协议版本为2.0 
	WSADATA wsData;                   
	/* 
	typedef struct WSAData
	{
    	WORD wVersion; //库文件建议应用程序使用的版本
    	WORD wHighVersion; //库文件支持的最高版本
    	//库描述字符串
    	char szDescription[WSADESCRIPTION_LEN+1];
    	//系统状态字符串
    	char szSystemStatus[WSASYS_STATUS_LEN+1];
    	//同时支持的最大套接字数量
    	unsigned short iMaxSockets;
    	*****************************************************************
    	//以下两个参数在2.0版中已废弃
    	unsigned short iMaxUdpDg;
    	char FAR* lpVendorInfo;
    }	
	WSDATA,FAR * LPWSADATA;
　　WSADATA结构被用来保存函数WSAStartup返回的Windows Sockets初始化信息。
	*/
	if (WSAStartup(wVersion, &wsData)!=0)//初始化ws2_32.dll动态库
	{
		printf("初始化失败!\n");
	}
	
	SOCKET sockCli=socket(AF_INET,SOCK_STREAM,0);//SOCK_STREAM：流套接字，使用TCP 
	//地址族，一般是AF_INET，表示使用IP地址族              0表示协议类型 

	sockaddr_in addClient;
	/*
	
	struct sockaddr_in 
	{
    	short sa_family; //地址族（指定地址格式） ，设为AF_INET
    	u_short	sin_port; //端口号
    	struct in_addr sin_addr; //IP地址
    	char sin_zero[8]; //空子节，设为空
	}
	
	struct in_addr
	{
    	union
    	{
    		struct 
			{
    			u_char s_b1,s_b2,s_b3,s_b4;
    		} S_un_b;
    		
	    	struct  
			{
	    		u_short s_w1,s_w2;
	    	} S_un_w;
	    	
	    	u_long S_addr;
    	} S_un;
    };
	
	struct sockaddr
	{
    	u_short	sa_family;
    	char  sa_data[14];
	}
	
	*/ 
	addClient.sin_family=AF_INET;                       // 指定地址 格式 
	addClient.sin_addr.S_un.S_addr=inet_addr(Server_IP);// 联合体 IP地址 
	addClient.sin_port=htons(8000);                     // 应用程序端口号 
	/*
	int connect
	(
    	SOCKET s                         //将要连接的socket 
        const struct sockaddr FAR * name //目标socket地址
    	int namelen                      //地址参数(name)的长度
    );
	*/
	
	if(connect(sockCli,(SOCKADDR*)&addClient,sizeof(SOCKADDR))!=0)
		printf("连接服务器失败!\n");
	//addClient：指定地址格式，存放：IP地址和端口号 
	//sockCli ：SOCKET类型，类似于文件描述符 
	else
	{		
		printf("连接成功!\n");		
		while (1)
		{
			printf("请输入需要发送的内容：");
			gets(sendBuf);
			send(sockCli,sendBuf,strlen(sendBuf)+1,0);				
			if (strcmp(sendBuf,"exit")==0)    //发送终止符exit，通信结束 
			{
				break;
			}	
			//fflush(stdin);	
			/*
			int recv
			(
		    	SOCKET s        //套接字 
		    	char FAR * buf, //接收数据缓冲区
		    	int len      //缓冲区长度
		    	int flags // 0表示接收的是正常数据，无特殊行为
		    );
                返回值：接收到的字节数
			*/
			recv(sockCli,recvBuf,len,0);
    		if (strcmp(recvBuf,"exit")==0)  //当收到exit字符时，表示另一方想要通信结束 
	    	{
	    		break;
	    	}
		    printf("接受：%s\n",recvBuf);						
			fflush(stdin);	
		}
		closesocket(sockCli);
	}
	return 0;
}
