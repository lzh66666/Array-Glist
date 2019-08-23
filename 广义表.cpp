#include <iostream>
#include <stdlib.h>
#include <string.h> 

#define OVERFLOW -2
#define OK 1
#define ERROR 0

typedef int Status; 

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<串S的操作开始<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 
typedef struct{
	char *ch;    //若是空串，则按串长分配存储区，否则ch为NULL 
	int length;  //串长度 
}SString;

Status InitSString(SString &T){
	T.ch=(char *)malloc(sizeof(char)); 
	T.length=0;
	return OK;
}

Status StrAssign(SString &T,char *chars){
//	生成一个其值等于串常量chars的串T 
	int i=0,j;
	
	char *c=chars;
	if(T.ch)
		free(T.ch);
	for(i=0;*c;++i,++c); 
	if(!i)
	{
		T.ch=NULL;
		T.length=0;
	} 
	else
	{
		T.ch=(char *)malloc(i*sizeof(char));
		if(T.ch==NULL)
			exit(OVERFLOW);
		for(j=0;j<i;j++)
		{
			T.ch[j]=chars[j];
		}
		T.length=i;
	}
	
	return OK;
}


Status StrPrint(SString T)
{
	for(int i=0;i<T.length;i++)
	{
		printf("%c",T.ch[i]);
	} 
	printf("\n");
	return OK;
}

int StrLength(SString S)
{
	return S.length;
} 

int StrCompare(SString S,SString T){
	
	int i;
	for(i=0;i<S.length && i<T.length;++i)
	{
		if(S.ch[i]!=T.ch[i])
			return S.ch[i]-T.ch[i];
	}
	return S.length-T.length;
 } 

Status ClearString(SString &S){
	
	//将S串清空 
	if(S.ch)
	{
		free(S.ch);
		S.ch=NULL;
	}
	S.length=0;
	return OK;
} 

Status SubString(SString &Sub,SString S,int pos,int len){
//	用Sub返回串S的第pos个字符起长度为len的子串 
	if(pos<1 || pos>S.length ||len<0 || len>S.length-pos+1)
		return ERROR;
	if(Sub.ch)
		free(Sub.ch);		//释放旧空间 
	if(!len)
		InitSString(Sub);
	else					//完整子串 
	{
		Sub.ch=(char *)malloc(len*sizeof(char));
		for(int i=0;i<len;i++)
		{
		 	Sub.ch[i]=S.ch[pos-1+i];
		}
		Sub.length=len; 
	}
	return OK;
}

Status StrEmpty(SString S){
	if(!S.ch || S.length==0)
		return OK;
	else
		return ERROR;
} 

Status StrCopy(SString &hstr,SString str){
	if(!StrEmpty(str)){
		for(int i=0;i<str.length;i++)
			hstr.ch[i]=str.ch[i];
		hstr.length=str.length;
	}
	return OK; 
}
 
Status serve(SString &str,SString &hstr){
	//将非空串str分割成两部分:hstr为第一个','之前的子串,str为那之后的子串 
	int i=0,k=0;	//k为尚未匹配的左括号个数 
	SString ch;
	InitSString(ch);
	
	int n=StrLength(str);
	//搜索最外成的第一个逗号 
	do{
		++i;
		SubString(ch,str,i,1);
		if(ch.ch[0] == '(')
			++k;
		else if(ch.ch[0] == ')')
			--k;
	}while(i<n&&(ch.ch[0] != ',' || k != 0));
	if(i<n){
		SubString(hstr,str,1,i-1);
		SubString(str,str,i+1,n-i);
	}
	else{
		StrCopy(hstr,str);
		ClearString(str);
	}
	return OK;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<广义表操作开始<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

typedef enum{ATOM,LIST}ElemTag;//ATOM == 0:原子,LIST == 1:子表 

typedef SString AtomType;

typedef struct GLNode{
	ElemTag tag;	//公共部分,用于区分原子结点和表结点
	union{			//原子结点和表结点联合的部分 
		AtomType atom;	//原子结点的值域
		struct {
			struct GLNode *hp,*tp; 
		}ptr;		//ptr是表结点的指针域,ptr.hp和ptr.tp分别指向表头和表尾 
	};  
}*GList;			//广义表类型GList是一种头尾链表 

Status CreateGList(GList &L,SString S){
	//采用头尾链表存储结构,有广义表的书写形式串S创建广义表L。设emp="()" 
	SString emp,sub,hsub;
	
	InitSString(emp);
	char chars[]="()"; 
	
	StrAssign(emp,chars);

	InitSString(sub);
	InitSString(hsub);
	
	GList p,q;
	if(!StrCompare(S,emp)){//创建空表 
		L=NULL;
		printf("空"); 
	}
	else{
		if(!(L = (GList)malloc(sizeof(GLNode))))//建表结点 
			exit(OVERFLOW);
		if(StrLength(S) == 1){			//创建单原子广义表 
			L->tag = ATOM;
			L->atom=S;
		} 
		else{
			L->tag = LIST;
			p=L;
			SubString(sub,S,2,StrLength(S)-2);//脱去外层括号 
			do{
				//重复建n个子表
				serve(sub,hsub); //从sub中分离出表头串hsub
				CreateGList(p->ptr.hp,hsub);
				q=p;
				if(!StrEmpty(sub)){//表尾不空 
					if(!(p = (GList)malloc(sizeof(GLNode))))
						exit(OVERFLOW);
					p->tag = LIST;
					q->ptr.tp = p;
				}
			}while(!StrEmpty(sub));
			q->ptr.tp = NULL;
		} 
	} 
	return OK; 
}

int GListDepth(GList L){
	//采用头尾链表存储结构,求广义表L的深度
	if(!L)
		return 1;		//空表深度为1
	if(L->tag == ATOM)
		return 0;		//原子深度为0
	GList pp;
	int max,dep;
	for(max = 0,pp = L;pp;pp=pp->ptr.tp){
		dep = GListDepth(pp->ptr.hp);//求以pp->ptr.hp为头指针的子表深度
		if(dep > max)
			max = dep; 
	}
	return max+1;
}

Status CopyGList(GList &T,GList L){
	//采用头尾链表存储结构,由广义表L复制得到广义表T
	if(!L)
		T = NULL;
	else{
		if(!(T = (GList)malloc(sizeof(GLNode))))
			exit(OVERFLOW);
		T->tag = L->tag;
		if(L->tag == ATOM)
			T->atom = L->atom;		//复制单原子
		else{
			CopyGList(T->ptr.hp,L->ptr.hp);
			CopyGList(T->ptr.tp,L->ptr.tp);
		} 
	} 
	return OK;
}
 
int main(int argc, char** argv) {
	
	SString S;
	InitSString(S);
	char chars[100];
	printf("请输入一个广义表:");
	gets(chars);
	StrAssign(S,chars);
	printf("广义表输出:"); 
	StrPrint(S);
	
	GList L,T;
	CreateGList(L,S);
	
	int deep = GListDepth(L);
	printf("广义表的深度为:%d",deep);
	return 0;
}

 
