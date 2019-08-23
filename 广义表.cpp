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
 //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<��S�Ĳ�����ʼ<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 
//���Ķ���˳���ʾ
#define MAXSTRLEN 100
 
typedef unsigned char SString[MAXSTRLEN+1];			//0�ŵ�Ԫ��Ŵ��ĳ���
 
//����һ����ֵ����chars�Ĵ�T
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
	//��S����� 
	S[0] = 0;
	return OK;
}
 
Status SubString(SString &Sub , SString S , int pos , int len){
	//	��Sub���ش�S�ĵ�pos���ַ��𳤶�Ϊlen���Ӵ� 
	int i;
	if(pos < 1 || pos > S[0] || len < 0 || len > S[0]-pos+1)
		return ERROR;
	for(i = 1 ; i <= len ; i++)
		Sub[i] = S[i+pos-1];	//ע��˴���S[i+pos-1]���ӵ�pos���ַ�
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
	//���ǿմ�str�ָ��������:hstrΪ��һ��','֮ǰ���Ӵ�,strΪ��֮����Ӵ� 
	int i = 0, k = 0;//kΪ��δƥ��������Ÿ��� 
	SString ch;
	int n = StrLength(str);
	//��������ɵĵ�һ������ 
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
 
 
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<����������ʼ<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
typedef char AtomType;
typedef int ElemType;

typedef enum {ATOM, LIST} ElemTag;		// ATOM == 0��ԭ�� LIST == 1���ӱ�
 
typedef struct GLNode{
	ElemTag tag;	//��������,��������ԭ�ӽ��ͱ���
	union{			//ԭ�ӽ��ͱ������ϵĲ��� 
		AtomType atom;	//ԭ�ӽ���ֵ��
		struct {
			struct GLNode *hp,*tp; 
		}ptr;		//ptr�Ǳ����ָ����,ptr.hp��ptr.tp�ֱ�ָ���ͷ�ͱ�β 
	};  
}*GList;			//���������GList��һ��ͷβ���� 
 
void InitList(GList &L)
{
	L = NULL;
}
 
void CreateGList(GList &L, SString S){
	//����ͷβ����洢�ṹ,�й�������д��ʽ��S���������L����emp="()" 
	SString sub, hsub, emp;
	GList p, q;
	char chars[]="()";
	StrAssign(emp, chars);
	
	if (!StrCompare(S, emp)) //�����ձ� 
		L = NULL;
	else
	{
		L = (GList)malloc(sizeof(GLNode));
		if (!L)				//������ 
			exit(OVERFLOW);
		if (StrLength(S) == 1)	//������ԭ�ӹ���� 
		{
			L->tag = ATOM;
			L->atom = S[1];
		}
		else
		{
			L->tag = LIST;
			p = L;
			SubString(sub, S, 2, StrLength(S) - 2);		//��ȥ������� 
			do
			{
				//�ظ���n���ӱ�
				sever(sub, hsub);//��sub�з������ͷ��hsub
				CreateGList(p->ptr.hp, hsub);
				q = p;
				if (!StrEmpty(sub))		//��β���� 
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
	//����ͷβ����洢�ṹ,������L�����
	int max, dep;
	GList pp;
	if (!L) 		//�ձ����Ϊ1
		return 1;
	if (L->tag == ATOM) 	//ԭ�����Ϊ0
		return 0;
	for (max = 0, pp = L; pp; pp = pp->ptr.tp)
	{
		dep = GListDepth(pp->ptr.hp);//����pp->ptr.hpΪͷָ����ӱ����
		if (dep > max) 
			max = dep;
	}
	return max + 1;
}

 
void CopyGList(GList &T, GList L){
	//����ͷβ����洢�ṹ,�ɹ����L���Ƶõ������T
	if (!L) T = NULL;		// ���ƿձ�
	else
	{
		T = (GList)malloc(sizeof(GLNode));
		if (!T) exit(OVERFLOW);
		T->tag = L->tag;
		if (L->tag == ATOM)
			T->atom =L->atom;	//���Ƶ�ԭ��
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
 	
	printf("����������T:");
	gets(chars);
	StrAssign(S,chars);
	printf("�����T���:"); 
	StrPrint(S);
	
	GList L,T;
	InitList(L);
	CreateGList(L,S); 
	int deepth=GListDepth(L);
	printf("�����T�����Ϊ:%d\n",deepth);
	
	int length = GListLength(L);
	printf("������T����Ϊ:%d\n",length);
	
	printf("��ʼ���ƹ����T��L\n");
	InitList(T);
	CopyGList(T,L);
	
	int deepth1=GListDepth(L);
	printf("�����L�����Ϊ:%d\n",deepth1);
	
	int length1 = GListLength(L);
	printf("������L����Ϊ:%d\n",length1);
 	return 0;
 } 
 
 
