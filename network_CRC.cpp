#include<iostream>
#include<stdio.h>
#include<string.h>

int findone(char c[]);
void printcrc(char c[],int len);

int main()
{
	char input[600],gx[300],crc[300];//  input:输入的编码    gx:多项式

	while(1)
	{
		printf("发送数据为：");
		gets(input);
		printf("生成多项式为：");
		gets(gx);
		
		int len1=strlen(input),len2=strlen(gx);
		if(len2>len1)
		{
			printf("输入的多项式不合法！！\n");
			continue;
		}
		
		int i,j=len2; 
		strncpy(crc,input,len2);
		// 对数据码进行加长 	
		for(i=0;i<len2-1;i++)
		{
			input[len1+i]='0';
		}
		input[len1+i]='\0';
		len1=strlen(input);
		
		// CRC计算 
		while(1)
    	{
	    	int k=0;
			//求出余数  
    		for(i=0;i<len2;i++)
	    	{	 
	    		if(crc[i]==gx[i])
	    		{
	     			crc[i]='0';
	    		}
	    		else
	    		{
					crc[i]='1';
				}
			}
			//对余数进行判断 
			k=findone(crc);//求出第一个为1的字符所在的位置 
			if(k==len2) // 余数为0 
			{
				if(j<len1)//还未比完 
				{
					k=findone(input+j);
			    	j+=k;
					strncpy(crc,input+j,len2);
     				j+=len2;
				}
				else//已经比完 
				{
					crc[len2-2]='\0';
				}
			}
			else//余数不为0 
			{
    			int i1=0;
    			for(i=k;i<len2;i++)//移位操作 
    			{
    				crc[i1++]=crc[i];
    			}
				//补上k位
     			strncpy(crc+i1,input+j,k);
    			j+=k;
    		}
		
			//判断补完后的余数位数，若小于除数，直接输出 
	    	k=strlen(crc);
	    	if(k<len2)
     		{
	    		printcrc(crc,len2-1);
    			break;
    		}
    	} 
	}
	return 0;
} 

int findone(char c[])
{
	int i;
	int len1=strlen(c);
	for(i=0;i<len1;i++)
	{
		if(c[i]=='1')
		{
			return i;
		}
	}
	return i;
}

void printcrc(char c[],int len)
{
	int i,len1=strlen(c);

	printf("生成CRC为：");
	
	if(len1==len)
	{
		printf("%s\n\n",c);
	}
	else
	{
		for(i=0;i<len-len1;i++)
		{
			printf("0");
		}
		printf("%s\n\n",c);

	}
	printf("\n");
	return;
}
