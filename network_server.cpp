#include <stdio.h>
#include <winsock2.h>
#include <string.h> 
#pragma comment ( lib, "ws2_32.lib" )

void s_to_b(char a[],char b[]); 

int main()
{
	printf("|=============================================|\n");
	printf("|                                             |\n");
	printf("|                                             |\n");
	printf("|                     服务器端                |\n");
	printf("|                                             |\n");
	printf("|                                             |\n");
	printf("|=============================================|\n");
	
	int len=1024;
	int flag=0;
	char Local_IP[20]={'0'};
	char recvBuf[1024]="\0";
	char sendBuf[1024]="\0";
	char TempBuf[1024]="\0";
	char path[100]={'0'};
	
	printf("请输入本地IP:");
	gets(Local_IP);
	WORD wVersion = MAKEWORD(2, 0);
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
	if (WSAStartup(wVersion, &wsData)!=0)
	{
		printf("初始化失败!\n");
	}
	
	SOCKET sockSrv=socket(AF_INET,SOCK_STREAM,0);//创建套接字，返回套接字描述符 
	
	sockaddr_in addServer,addrClient;
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
	addServer.sin_family = AF_INET;                         //指定地址格式 
	addServer.sin_addr.S_un.S_addr = inet_addr(Local_IP);
	addServer.sin_port = htons(8000);                       //指定端口号 
	
	if (bind(sockSrv,(SOCKADDR*)&addServer,sizeof(SOCKADDR))!=0)
	{
		printf("端口连接失败!\n");
	}
	else
	{
loop:
	listen(sockSrv,5);
	addrClient.sin_family=AF_INET;
	addrClient.sin_addr.S_un.S_addr=INADDR_ANY;
	addrClient.sin_port=htons(8000);
	printf("等待客户端连接……\n");
	SOCKET sockConn=accept(sockSrv,(SOCKADDR*)&addrClient,&len);
	/*
	在服务器端通过listen()进入监听状态时， 
	accept() 表示可以接受自客户端的connect()发出的请求，双方进入连接状态
		SOCKET accept
		(
			SOCKET s                    //处于监听状态的socket
			struct sockaddr FAR * addr, //客户机IP地址的sockaddr指针  
			//   addrClient  SOCKADDR类型  
			int FAR * addrlen           //地址的长度
		);
	*/
	printf("连接成功!\n");
	while (1)
	{
/*		
		printf("请输入需要发送的内容：");
		fflush(stdin);
//		gets(sendBuf);		
		if (strcmp(sendBuf,"exit")==0)
		{
			send(sockConn,sendBuf,len,0);
			break;
		}
		else*/
		
		fflush(stdin);	
		recv(sockConn,recvBuf,1024,0);			   //  接受字符 
		printf("接受：%s\n",recvBuf);
		s_to_b(recvBuf,sendBuf);                    // 将小写字母转为大写 
		sendBuf[strlen(recvBuf)] = '\0';
		send(sockConn,sendBuf,strlen(sendBuf)+1,0);//  发送字符 
		printf("发送：%s\n",sendBuf);	
		if (strcmp(recvBuf,"exit")==0)       //收到exit，通信结束 
		{
			printf("客户端已下线!\n");
			goto loop;
		}
		
				           //清空缓存区 
					
	}
	closesocket(sockConn);	
	}
	return 0;
}

void s_to_b(char a[],char b[])
{
	int lena=strlen(a);
	int i,j;
	for(i=0;i<lena;i++)
	{
		if('a'<=a[i] && a[i]<='z')
		{
			b[i]=a[i]-32;
		}
		else
		{
			b[i]=a[i];
		}
	}
}
