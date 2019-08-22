#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define OK 1
#define ERROR 0
#define OVERFLOW -2

typedef int ElemType;
typedef int Status;

typedef struct QLNode{
	int i,j;
	ElemType e;
	struct QLNode *right,*down;
}OLNode,*OLink;

typedef struct{
	OLink *rhead,*chead;
	int mu,nu,tu;
}CrossList;

Status InitSMatrix(CrossList *M);
Status CreateSMatrix_OL(CrossList *M);
void PrintSMatrix_OL(CrossList M);
Status AddSMatrix(CrossList M,CrossList N,CrossList *Q);

int main(int argc, char** argv) {
	CrossList M,N,Q;
	
	InitSMatrix(&M);
	CreateSMatrix_OL(&M);
	printf("矩阵M的输出：\n");
	PrintSMatrix_OL(M);
	
	InitSMatrix(&N);
	CreateSMatrix_OL(&N);
	printf("矩阵N的输出：\n");
	PrintSMatrix_OL(N);
	
	InitSMatrix(&Q);
	printf("两矩阵相加的结果为：\n"); 
	AddSMatrix(M,N,&Q);
	PrintSMatrix_OL(Q);
	
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
    
	printf("请输入矩阵的行数列数和非零元个数：\n");
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
		
	printf("输入结点的行列和数值(行和列从1开始)：\n");
	for (int c = 1; c <= t; c++) 
	{ // 按任意次序输入非零元
		scanf("%d%d%d",&i,&j,&e);
		if (!(p = (OLNode *)malloc(sizeof(OLNode))))
			exit(OVERFLOW);
			
		p->i = i; 
		p->j = j; 
		p->e = e; 
		
		if ((*M).rhead[i] == NULL || (*M).rhead[i]->j > j) 	//p插在这一行的第一个结点处，M->rhead[i]始终指向改行的第一个结点处 
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
Status AddSMatrix(CrossList M,CrossList N,CrossList *Q)
{
    int i,k;
    OLink p,pq,pm,pn;
    OLink *col;
    
    if(M.mu!=N.mu && M.nu!=N.nu)
    {
        printf("输入错误！两个矩阵不能相加！");
        return ERROR;
    }
    
    (*Q).mu=M.mu;
    (*Q).nu=M.nu;
    (*Q).tu=0; 
    
    (*Q).rhead=(OLink*)malloc(((*Q).mu+1)*sizeof(OLink));
    if(!(*Q).rhead)
        exit(OVERFLOW);
    
    (*Q).chead=(OLink*)malloc(((*Q).nu+1)*sizeof(OLink));
    if(!(*Q).chead)
        exit(OVERFLOW);
    
    for(k=1;k<=(*Q).mu;k++) // 初始化Q的行头指针向量;各行链表为空链表 
        (*Q).rhead[k]=NULL;
    for(k=1;k<=(*Q).nu;k++) // 初始化Q的列头指针向量;各列链表为空链表 
        (*Q).chead[k]=NULL;
    
    // 生成指向列的最后结点的数组 
    col=(OLink*)malloc(((*Q).nu+1)*sizeof(OLink)); 
    if(!col)
        exit(OVERFLOW);
    
    for(k=1;k<=(*Q).nu;k++) // 赋初值 
        col[k]=NULL;
    
    for(i=1;i<=M.mu;i++) // 按行的顺序相加 
    {
        pm=M.rhead[i];    // pm指向矩阵M的第i行的第1个结点 
        pn=N.rhead[i];    // pn指向矩阵N的第i行的第1个结点 
        while(pm&&pn)    // pm和pn均不空 
        {
            if(pm->j<pn->j) // 矩阵M当前结点的列小于矩阵N当前结点的列 
            {
                p=(OLink)malloc(sizeof(OLNode)); // 生成矩阵Q的结点 
                if(!p)
                    exit(OVERFLOW);
                    
                (*Q).tu++;    // 非零元素数加1 
                p->i=i;        // 给结点赋值 
                p->j=pm->j;
                p->e=pm->e;
                p->right=NULL;
                pm=pm->right; // pm指针向右移 
            }
            else if(pm->j>pn->j)// 矩阵M当前结点的列大于矩阵N当前结点的列 
            {
                p=(OLink)malloc(sizeof(OLNode)); // 生成矩阵Q的结点 
                if(!p)
                    exit(OVERFLOW);
                    
                (*Q).tu++;    // 非零元素数加1 
                p->i=i;        // 给结点赋值 
                p->j=pn->j;
                p->e=pn->e;
                p->right=NULL;
                pn=pn->right; // pn指针向右移 
            }
            // 矩阵M、N当前结点的列相等且两元素之和不为0
            else if(pm->e+pn->e) 
            {
                p=(OLink)malloc(sizeof(OLNode)); // 生成矩阵Q的结点 
                if(!p)
                    exit(OVERFLOW);
                    
                (*Q).tu++; // 非零元素数加1 
                p->i=i; // 给结点赋值 
                p->j=pn->j;
                p->e=pm->e+pn->e;
                p->right=NULL;
                pm=pm->right; // pm指针向右移 
                pn=pn->right; // pn指针向右移 
            }
            else // 矩阵M、N当前结点的列相等且两元素之和为0 
            {
                pm=pm->right; // pm指针向右移 
                pn=pn->right; // pn指针向右移 
                continue;
            }
            
            
            if((*Q).rhead[i]==NULL) // p为该行的第1个结点
                // p插在该行的表头且pq指向p(该行的最后一个结点) 
                (*Q).rhead[i]=pq=p; 
            else // 插在pq所指结点之后 
            {
                pq->right=p; // 完成行插入 
                pq=p; // pq指向该行的最后一个结点 
            }
            
            if((*Q).chead[p->j]==NULL) // p为该列的第1个结点
                 // p插在该列的表头且col[p->j]指向p  
                (*Q).chead[p->j]=col[p->j]=p;
            else // 插在col[p->]所指结点之后 
            {
                col[p->j]->down=p; // 完成列插入
                 // col[p->j]指向该列的最后一个结点 
                col[p->j]=p;
            }
            
        }
        
        while(pm) // 将矩阵M该行的剩余元素插入矩阵Q 
        {
            p=(OLink)malloc(sizeof(OLNode)); // 生成矩阵Q的结点 
            if(!p)
                exit(OVERFLOW);
                
            (*Q).tu++; // 非零元素数加1 
            p->i=i; // 给结点赋值 
            p->j=pm->j;
            p->e=pm->e;
            p->right=NULL;
            pm=pm->right; // pm指针向右移 
            
            if((*Q).rhead[i] == NULL) // p为该行的第1个结点 
                // p插在该行的表头且pq指向p(该行的最后一个结点)
                (*Q).rhead[i] = pq = p;  
            else // 插在pq所指结点之后 
            {
                pq->right=p; // 完成行插入 
                pq=p; // pq指向该行的最后一个结点 
            }
            
            if((*Q).chead[p->j] == NULL) // p为该列的第1个结点
                 // p插在该列的表头且col[p->j]指向p 
                (*Q).chead[p->j] = col[p->j] = p; 
            else // 插在col[p->j]所指结点之后 
            {
                col[p->j]->down=p; // 完成列插入
                // col[p->j]指向该列的最后一个结点 
                col[p->j]=p;  
            }
            
        }
        
        while(pn) // 将矩阵N该行的剩余元素插入矩阵Q 
        {
            p=(OLink)malloc(sizeof(OLNode)); // 生成矩阵Q的结点 
            if(!p)
                exit(0);
                
            (*Q).tu++; // 非零元素数加1 
            p->i=i; // 给结点赋值 
            p->j=pn->j;
            p->e=pn->e;
            p->right=NULL;
            pn=pn->right; // pn指针向右移 
            
            if((*Q).rhead[i]==NULL) // p为该行的第1个结点 
                // p插在该行的表头且pq指向p(该行的最后一个结点)
                (*Q).rhead[i]=pq=p;  
            else // 插在pq所指结点之后 
            {
                pq->right=p; // 完成行插入 
                pq=p; // pq指向该行的最后一个结点 
            }
            
            if((*Q).chead[p->j]==NULL) // p为该列的第1个结点
                // p插在该列的表头且col[p->j]指向p  
                (*Q).chead[p->j]=col[p->j]=p; 
            else // 插在col[p->j]所指结点之后 
            {
                col[p->j]->down=p; // 完成列插入
                // col[p->j]指向该列的最后一个结点 
                col[p->j]=p;  
            }
            
        }
    }
    for(k=1;k<=(*Q).nu;k++)
        if(col[k]) // k列有结点 
            col[k]->down=NULL; //  令该列最后一个结点的down指针为空 
    free(col);
    return OK;
}

