#include <iostream>
#include <stdlib.h>
#include <string.h> 

#define OVERFLOW -2
#define OK 1
#define ERROR 0

typedef int Status; 

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<��S�Ĳ�����ʼ<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 
typedef struct{
	char *ch;    //���ǿմ����򰴴�������洢��������chΪNULL 
	int length;  //������ 
}SString;

Status InitSString(SString &T){
	T.ch=(char *)malloc(sizeof(char)); 
	T.length=0;
	return OK;
}

Status StrAssign(SString &T,char *chars){
//	����һ����ֵ���ڴ�����chars�Ĵ�T 
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
	
	//��S����� 
	if(S.ch)
	{
		free(S.ch);
		S.ch=NULL;
	}
	S.length=0;
	return OK;
} 

Status SubString(SString &Sub,SString S,int pos,int len){
//	��Sub���ش�S�ĵ�pos���ַ��𳤶�Ϊlen���Ӵ� 
	if(pos<1 || pos>S.length ||len<0 || len>S.length-pos+1)
		return ERROR;
	if(Sub.ch)
		free(Sub.ch);		//�ͷžɿռ� 
	if(!len)
		InitSString(Sub);
	else					//�����Ӵ� 
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
	//���ǿմ�str�ָ��������:hstrΪ��һ��','֮ǰ���Ӵ�,strΪ��֮����Ӵ� 
	int i=0,k=0;	//kΪ��δƥ��������Ÿ��� 
	SString ch;
	InitSString(ch);
	
	int n=StrLength(str);
	//��������ɵĵ�һ������ 
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

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<����������ʼ<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

typedef enum{ATOM,LIST}ElemTag;//ATOM == 0:ԭ��,LIST == 1:�ӱ� 

typedef SString AtomType;

typedef struct GLNode{
	ElemTag tag;	//��������,��������ԭ�ӽ��ͱ���
	union{			//ԭ�ӽ��ͱ������ϵĲ��� 
		AtomType atom;	//ԭ�ӽ���ֵ��
		struct {
			struct GLNode *hp,*tp; 
		}ptr;		//ptr�Ǳ����ָ����,ptr.hp��ptr.tp�ֱ�ָ���ͷ�ͱ�β 
	};  
}*GList;			//���������GList��һ��ͷβ���� 

Status CreateGList(GList &L,SString S){
	//����ͷβ����洢�ṹ,�й�������д��ʽ��S���������L����emp="()" 
	SString emp,sub,hsub;
	
	InitSString(emp);
	char chars[]="()"; 
	
	StrAssign(emp,chars);

	InitSString(sub);
	InitSString(hsub);
	
	GList p,q;
	if(!StrCompare(S,emp)){//�����ձ� 
		L=NULL;
		printf("��"); 
	}
	else{
		if(!(L = (GList)malloc(sizeof(GLNode))))//������ 
			exit(OVERFLOW);
		if(StrLength(S) == 1){			//������ԭ�ӹ���� 
			L->tag = ATOM;
			L->atom=S;
		} 
		else{
			L->tag = LIST;
			p=L;
			SubString(sub,S,2,StrLength(S)-2);//��ȥ������� 
			do{
				//�ظ���n���ӱ�
				serve(sub,hsub); //��sub�з������ͷ��hsub
				CreateGList(p->ptr.hp,hsub);
				q=p;
				if(!StrEmpty(sub)){//��β���� 
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
	//����ͷβ����洢�ṹ,������L�����
	if(!L)
		return 1;		//�ձ����Ϊ1
	if(L->tag == ATOM)
		return 0;		//ԭ�����Ϊ0
	GList pp;
	int max,dep;
	for(max = 0,pp = L;pp;pp=pp->ptr.tp){
		dep = GListDepth(pp->ptr.hp);//����pp->ptr.hpΪͷָ����ӱ����
		if(dep > max)
			max = dep; 
	}
	return max+1;
}

Status CopyGList(GList &T,GList L){
	//����ͷβ����洢�ṹ,�ɹ����L���Ƶõ������T
	if(!L)
		T = NULL;
	else{
		if(!(T = (GList)malloc(sizeof(GLNode))))
			exit(OVERFLOW);
		T->tag = L->tag;
		if(L->tag == ATOM)
			T->atom = L->atom;		//���Ƶ�ԭ��
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
	printf("������һ�������:");
	gets(chars);
	StrAssign(S,chars);
	printf("��������:"); 
	StrPrint(S);
	
	GList L,T;
	CreateGList(L,S);
	
	int deep = GListDepth(L);
	printf("���������Ϊ:%d",deep);
	return 0;
}

 
