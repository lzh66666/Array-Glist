#include <stdlib.h>	
#include <stdio.h>		
#include <string.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
 
typedef int Status;
 //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<串S的操作开始<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 
//串的定长顺序表示
#define MAXSTRLEN 100
 
typedef unsigned char SString[MAXSTRLEN+1];			//0号单元存放串的长度
 
//生成一个其值等于chars的串T
Status StrAssign(SString &T , char *chars)
{
	int i;
	if(strlen(chars) > MAXSTRLEN) 
		return ERROR;
	else
	{
		T[0] = strlen(chars);
		for(i = 1 ; i <= T[0] ; i++)
			T[i] = *(chars+i-1);
		return OK;
	}
}

Status StrPrint(SString T){
   int i;
   for(i=1;i<=T[0];i++)
    	printf("%c",T[i]);
   printf("\n");
   return OK;
   
}

int StrLength(SString S){
	return S[0];
}

int StrCompare(SString S , SString T){
	int i;
	for(i = 1 ; i <= S[0] && i <= T[0] ; i++)
		if(S[i] != T[i]) 
			return S[i] - T[i];
	return S[0]-T[0];
}

Status ClearString(SString &S){
	//将S串清空 
	S[0] = 0;
	return OK;
}
 
Status SubString(SString &Sub , SString S , int pos , int len){
	//	用Sub返回串S的第pos个字符起长度为len的子串 
	int i;
	if(pos < 1 || pos > S[0] || len < 0 || len > S[0]-pos+1)
		return ERROR;
	for(i = 1 ; i <= len ; i++)
		Sub[i] = S[i+pos-1];	//注意此处是S[i+pos-1]，从第pos个字符
	Sub[0] = len;
	return OK;
}

Status StrEmpty(SString S){
	if(S[0] == 0)
		return TRUE;
	else
		return FALSE;
}

Status StrCopy(SString &T , SString S){
	int i;
	for(i = 0 ; i <= S[0] ; i++)
		T[i] = S[i];
	return OK;
}
 
Status sever(SString &str, SString &hstr){
	//将非空串str分割成两部分:hstr为第一个','之前的子串,str为那之后的子串 
	int i = 0, k = 0;//k为尚未匹配的左括号个数 
	SString ch;
	int n = StrLength(str);
	//搜索最外成的第一个逗号 
	do
	{
		++i;
		SubString(ch, str, i, 1);
		if (ch[1] == '(') ++k;
		else if (ch[1] == ')') --k;
	} while (i < n && (ch[1] != ',' || k != 0));
	if (i < n) 
	{
		SubString(hstr, str, 1, i - 1);
		SubString(str, str, i + 1, n - i);
	}
	else
	{
		StrCopy(hstr, str);
		ClearString(str);
	}
	return OK;
} 
 
 
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<广义表操作开始<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
typedef char AtomType;
typedef int ElemType;

typedef enum {ATOM, LIST} ElemTag;		// ATOM == 0：原子 LIST == 1：子表
 
typedef struct GLNode{
	ElemTag tag;	//公共部分,用于区分原子结点和表结点
	union{			//原子结点和表结点联合的部分 
		AtomType atom;	//原子结点的值域
		struct {
			struct GLNode *hp,*tp; 
		}ptr;		//ptr是表结点的指针域,ptr.hp和ptr.tp分别指向表头和表尾 
	};  
}*GList;			//广义表类型GList是一种头尾链表 
 
void InitList(GList &L)
{
	L = NULL;
}
 
void CreateGList(GList &L, SString S){
	//采用头尾链表存储结构,有广义表的书写形式串S创建广义表L。设emp="()" 
	SString sub, hsub, emp;
	GList p, q;
	char chars[]="()";
	StrAssign(emp, chars);
	
	if (!StrCompare(S, emp)) //创建空表 
		L = NULL;
	else
	{
		L = (GList)malloc(sizeof(GLNode));
		if (!L)				//建表结点 
			exit(OVERFLOW);
		if (StrLength(S) == 1)	//创建单原子广义表 
		{
			L->tag = ATOM;
			L->atom = S[1];
		}
		else
		{
			L->tag = LIST;
			p = L;
			SubString(sub, S, 2, StrLength(S) - 2);		//脱去外层括号 
			do
			{
				//重复建n个子表
				sever(sub, hsub);//从sub中分离出表头串hsub
				CreateGList(p->ptr.hp, hsub);
				q = p;
				if (!StrEmpty(sub))		//表尾不空 
				{
					p = (GList)malloc(sizeof(GLNode));
					if (!p) 
						exit(OVERFLOW);
					p->tag = LIST;
					q->ptr.tp = p;
				}
			} while (!StrEmpty(sub));
			q->ptr.tp = NULL;
		}
	}
}
 
int GListLength(GList L)
{
	int len = 0;
	while (L)
	{
		L = L->ptr.tp;
		++len;
	}
	return len;
}
 
int GListDepth(GList L){
	//采用头尾链表存储结构,求广义表L的深度
	int max, dep;
	GList pp;
	if (!L) 		//空表深度为1
		return 1;
	if (L->tag == ATOM) 	//原子深度为0
		return 0;
	for (max = 0, pp = L; pp; pp = pp->ptr.tp)
	{
		dep = GListDepth(pp->ptr.hp);//求以pp->ptr.hp为头指针的子表深度
		if (dep > max) 
			max = dep;
	}
	return max + 1;
}

 
void CopyGList(GList &T, GList L){
	//采用头尾链表存储结构,由广义表L复制得到广义表T
	if (!L) T = NULL;		// 复制空表
	else
	{
		T = (GList)malloc(sizeof(GLNode));
		if (!T) exit(OVERFLOW);
		T->tag = L->tag;
		if (L->tag == ATOM)
			T->atom =L->atom;	//复制单原子
		else
		{
			CopyGList(T->ptr.hp, L->ptr.hp);
			CopyGList(T->ptr.tp, L->ptr.tp);
		}
	}
}
 
 
  
void DestroyGList(GList &L)
{
	GList q1, q2;
	if (L)
	{
		q1 = L->ptr.hp;
		q2 = L->ptr.tp;
		DestroyGList(q1);
		DestroyGList(q2);
	}
	free(L);
	L =  NULL;
}

 
GList GetHead(GList L)
{
	GList h, p;
	if (!L)
		return NULL;
	p = L->ptr.hp;
	CopyGList(h, p);
	return h;
}
 
GList GetTail(GList L)
{
	GList h, p;
	if (!L)
		return NULL;
	p = L->ptr.tp;
	CopyGList(h, p);
	return h;
}
 
 
 
int main(int argc,char**argv){
 	SString S;
 	char chars[100]; 
 	
	printf("请输入广义表T:");
	gets(chars);
	StrAssign(S,chars);
	printf("广义表T输出:"); 
	StrPrint(S);
	
	GList L,T;
	InitList(L);
	CreateGList(L,S); 
	int deepth=GListDepth(L);
	printf("广义表T的深度为:%d\n",deepth);
	
	int length = GListLength(L);
	printf("广义表的T长度为:%d\n",length);
	
	printf("开始复制广义表T到L\n");
	InitList(T);
	CopyGList(T,L);
	
	int deepth1=GListDepth(L);
	printf("广义表L的深度为:%d\n",deepth1);
	
	int length1 = GListLength(L);
	printf("广义表的L长度为:%d\n",length1);
 	return 0;
 } 
 
 
