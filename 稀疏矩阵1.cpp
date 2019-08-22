#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define OK 1
#define ERROR 0
#define OVERFLOW -2

typedef int ElemType;
typedef int Status;

typedef struct QLNode{
	int i,j;		//�÷���Ԫ���к����±� 
	ElemType e;
	struct QLNode *right,*down;		//�÷���Ԫ�����б���б�ĺ������ 
}OLNode,*OLink;

typedef struct{
	OLink *rhead,*chead;			//�к�������ͷָ��������ַ��CreateSMatrix���� 
	int mu,nu,tu;					//ϡ���������������ͷ���Ԫ���� 
}CrossList;

Status InitSMatrix(CrossList *M);
Status CreateSMatrix_OL(CrossList *M);
void PrintSMatrix_OL(CrossList M);

int main(int argc, char** argv) {
	CrossList M;
	InitSMatrix(&M);
	CreateSMatrix_OL(&M);
	PrintSMatrix_OL(M);
	return 0;
}

Status InitSMatrix(CrossList *M)
{
    (*M).rhead=(*M).chead=NULL;
    (*M).mu=(*M).nu=(*M).tu=0;
    return OK;
}

Status CreateSMatrix_OL(CrossList *M)
{		//����ϡ�����M������ʮ������洢��ʾ�� 
	int m,n,t,i,j,e;
	OLink p,q;
	
	if((*M).rhead)
    	free(M->rhead);
    	
	printf("������M�����������ͷ���Ԫ������\n");
	scanf("%d%d%d",&m,&n,&t);
	(*M).mu=m;
    (*M).nu=n;
    (*M).tu=t;
    
	(*M).rhead=(OLink*)malloc((m+1)*sizeof(OLink));
	if(!(*M).rhead)
		exit(OVERFLOW);
	
	(*M).chead=(OLink*)malloc((n+1)*sizeof(OLink));
	if(!(*M).chead)
		exit(OVERFLOW);
	
	for (int a = 1; a <= m; a++)  // ��ʼ����ͷָ����������������Ϊ������
		(*M).rhead[a] = NULL;
		
	for (int b = 1; b <= n; b++) 	//��ʼ����ͷָ����������������Ϊ������
		(*M).chead[b] = NULL;
	
	printf("����������к���ֵ��\n");
	for (int c = 1; c <= t; c++) 
	{ // ����������������Ԫ
		scanf("%d%d%d",&i,&j,&e);
		if (!(p = (OLNode *)malloc(sizeof(OLNode))))
			exit(OVERFLOW);
			
		p->i = i; 
		p->j = j; 
		p->e = e; 
		
		if ((*M).rhead[i] == NULL || (*M).rhead[i]->j > j) 	//p������һ�еĵ�һ����㴦��M->rhead[i]ʼ��ָ����еĵ�һ����㴦 
		{
			p->right = (*M).rhead[i];  
			(*M).rhead[i] = p;
		}
		else 
		{  // Ѱ�����б��еĲ���λ��
			for (q = (*M).rhead[i]; (q->right) && (q->right->j<j); q = q->right);
			p->right = q->right;  
			q->right = p;
		}  // ����в���
		if ((*M).chead[j] == NULL || (*M).chead[j]->i > i)
		{
			p->down = (*M).chead[j];   
			(*M).chead[j] = p;
		}
		else 
		{  // Ѱ�����б��еĲ���λ��
			for (q = (*M).chead[j]; (q->down) && q->down->i < i; q = q->down);
			p->down = q->down; 
			q->down = p;
		}  // ����в���
	} 
	return OK;
} //CreateSMatrix_OL

void PrintSMatrix_OL(CrossList M)
{
	int i,j;
    OLink p;
    //��������� 
    for(j=1;j<=M.mu;j++)
    {
        p=M.rhead[j];
        while(p)
        {
            printf("(%d,%d,%d)\n",p->i,p->j,p->e);
            p=p->right;
        }
    } 
    //��������� 
    /*for(j=1;j<=M.nu;j++)
    {
        p=M.chead[j];
        while(p)
        {
            printf("(%d,%d,%d)\n",p->i,p->j,p->e);
            p=p->down;
        }
    }*/
}

