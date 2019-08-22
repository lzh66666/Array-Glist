#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define OK 1
#define ERROR 0
#define OVERFLOW -2

typedef int ElemType;
typedef int Status;

typedef struct QLNode{
	int i,j;		//该非零元的行和列下标 
	ElemType e;
	struct QLNode *right,*down;		//该非零元所在行表和列表的后继链域 
}OLNode,*OLink;

typedef struct{
	OLink *rhead,*chead;			//行和列链表头指针向量基址有CreateSMatrix分配 
	int mu,nu,tu;					//稀疏矩阵的行数列数和非零元个数 
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
{		//创建稀疏矩阵M。采用十字链表存储表示。 
	int m,n,t,i,j,e;
	OLink p,q;
	
	if((*M).rhead)
    	free(M->rhead);
    	
	printf("请输入M的行数列数和非零元个数：\n");
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
	
	for (int a = 1; a <= m; a++)  // 初始化行头指针向量；各行链表为空链表
		(*M).rhead[a] = NULL;
		
	for (int b = 1; b <= n; b++) 	//初始化列头指针向量；各列链表为空链表
		(*M).chead[b] = NULL;
	
	printf("输入结点的行列和数值：\n");
	for (int c = 1; c <= t; c++) 
	{ // 按任意次序输入非零元
		scanf("%d%d%d",&i,&j,&e);
		if (!(p = (OLNode *)malloc(sizeof(OLNode))))
			exit(OVERFLOW);
			
		p->i = i; 
		p->j = j; 
		p->e = e; 
		
		if ((*M).rhead[i] == NULL || (*M).rhead[i]->j > j) 	//p插在这一行的第一个结点处，M->rhead[i]始终指向该行的第一个结点处 
		{
			p->right = (*M).rhead[i];  
			(*M).rhead[i] = p;
		}
		else 
		{  // 寻查在行表中的插入位置
			for (q = (*M).rhead[i]; (q->right) && (q->right->j<j); q = q->right);
			p->right = q->right;  
			q->right = p;
		}  // 完成行插入
		if ((*M).chead[j] == NULL || (*M).chead[j]->i > i)
		{
			p->down = (*M).chead[j];   
			(*M).chead[j] = p;
		}
		else 
		{  // 寻查在列表中的插入位置
			for (q = (*M).chead[j]; (q->down) && q->down->i < i; q = q->down);
			p->down = q->down; 
			q->down = p;
		}  // 完成列插入
	} 
	return OK;
} //CreateSMatrix_OL

void PrintSMatrix_OL(CrossList M)
{
	int i,j;
    OLink p;
    //按行序输出 
    for(j=1;j<=M.mu;j++)
    {
        p=M.rhead[j];
        while(p)
        {
            printf("(%d,%d,%d)\n",p->i,p->j,p->e);
            p=p->right;
        }
    } 
    //按列序输出 
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

