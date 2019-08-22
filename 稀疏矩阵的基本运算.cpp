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
	printf("����M�������\n");
	PrintSMatrix_OL(M);
	
	InitSMatrix(&N);
	CreateSMatrix_OL(&N);
	printf("����N�������\n");
	PrintSMatrix_OL(N);
	
	InitSMatrix(&Q);
	printf("��������ӵĽ��Ϊ��\n"); 
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
{		//����ϡ�����M������ʮ������洢��ʾ�� 
	int m,n,t,i,j,e;
	OLink p,q;
	
	if((*M).rhead)
    	free(M->rhead);
    
	printf("�������������������ͷ���Ԫ������\n");
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
		
	printf("����������к���ֵ(�к��д�1��ʼ)��\n");
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
Status AddSMatrix(CrossList M,CrossList N,CrossList *Q)
{
    int i,k;
    OLink p,pq,pm,pn;
    OLink *col;
    
    if(M.mu!=N.mu && M.nu!=N.nu)
    {
        printf("�������������������ӣ�");
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
    
    for(k=1;k<=(*Q).mu;k++) // ��ʼ��Q����ͷָ������;��������Ϊ������ 
        (*Q).rhead[k]=NULL;
    for(k=1;k<=(*Q).nu;k++) // ��ʼ��Q����ͷָ������;��������Ϊ������ 
        (*Q).chead[k]=NULL;
    
    // ����ָ���е����������� 
    col=(OLink*)malloc(((*Q).nu+1)*sizeof(OLink)); 
    if(!col)
        exit(OVERFLOW);
    
    for(k=1;k<=(*Q).nu;k++) // ����ֵ 
        col[k]=NULL;
    
    for(i=1;i<=M.mu;i++) // ���е�˳����� 
    {
        pm=M.rhead[i];    // pmָ�����M�ĵ�i�еĵ�1����� 
        pn=N.rhead[i];    // pnָ�����N�ĵ�i�еĵ�1����� 
        while(pm&&pn)    // pm��pn������ 
        {
            if(pm->j<pn->j) // ����M��ǰ������С�ھ���N��ǰ������ 
            {
                p=(OLink)malloc(sizeof(OLNode)); // ���ɾ���Q�Ľ�� 
                if(!p)
                    exit(OVERFLOW);
                    
                (*Q).tu++;    // ����Ԫ������1 
                p->i=i;        // ����㸳ֵ 
                p->j=pm->j;
                p->e=pm->e;
                p->right=NULL;
                pm=pm->right; // pmָ�������� 
            }
            else if(pm->j>pn->j)// ����M��ǰ�����д��ھ���N��ǰ������ 
            {
                p=(OLink)malloc(sizeof(OLNode)); // ���ɾ���Q�Ľ�� 
                if(!p)
                    exit(OVERFLOW);
                    
                (*Q).tu++;    // ����Ԫ������1 
                p->i=i;        // ����㸳ֵ 
                p->j=pn->j;
                p->e=pn->e;
                p->right=NULL;
                pn=pn->right; // pnָ�������� 
            }
            // ����M��N��ǰ�������������Ԫ��֮�Ͳ�Ϊ0
            else if(pm->e+pn->e) 
            {
                p=(OLink)malloc(sizeof(OLNode)); // ���ɾ���Q�Ľ�� 
                if(!p)
                    exit(OVERFLOW);
                    
                (*Q).tu++; // ����Ԫ������1 
                p->i=i; // ����㸳ֵ 
                p->j=pn->j;
                p->e=pm->e+pn->e;
                p->right=NULL;
                pm=pm->right; // pmָ�������� 
                pn=pn->right; // pnָ�������� 
            }
            else // ����M��N��ǰ�������������Ԫ��֮��Ϊ0 
            {
                pm=pm->right; // pmָ�������� 
                pn=pn->right; // pnָ�������� 
                continue;
            }
            
            
            if((*Q).rhead[i]==NULL) // pΪ���еĵ�1�����
                // p���ڸ��еı�ͷ��pqָ��p(���е����һ�����) 
                (*Q).rhead[i]=pq=p; 
            else // ����pq��ָ���֮�� 
            {
                pq->right=p; // ����в��� 
                pq=p; // pqָ����е����һ����� 
            }
            
            if((*Q).chead[p->j]==NULL) // pΪ���еĵ�1�����
                 // p���ڸ��еı�ͷ��col[p->j]ָ��p  
                (*Q).chead[p->j]=col[p->j]=p;
            else // ����col[p->]��ָ���֮�� 
            {
                col[p->j]->down=p; // ����в���
                 // col[p->j]ָ����е����һ����� 
                col[p->j]=p;
            }
            
        }
        
        while(pm) // ������M���е�ʣ��Ԫ�ز������Q 
        {
            p=(OLink)malloc(sizeof(OLNode)); // ���ɾ���Q�Ľ�� 
            if(!p)
                exit(OVERFLOW);
                
            (*Q).tu++; // ����Ԫ������1 
            p->i=i; // ����㸳ֵ 
            p->j=pm->j;
            p->e=pm->e;
            p->right=NULL;
            pm=pm->right; // pmָ�������� 
            
            if((*Q).rhead[i] == NULL) // pΪ���еĵ�1����� 
                // p���ڸ��еı�ͷ��pqָ��p(���е����һ�����)
                (*Q).rhead[i] = pq = p;  
            else // ����pq��ָ���֮�� 
            {
                pq->right=p; // ����в��� 
                pq=p; // pqָ����е����һ����� 
            }
            
            if((*Q).chead[p->j] == NULL) // pΪ���еĵ�1�����
                 // p���ڸ��еı�ͷ��col[p->j]ָ��p 
                (*Q).chead[p->j] = col[p->j] = p; 
            else // ����col[p->j]��ָ���֮�� 
            {
                col[p->j]->down=p; // ����в���
                // col[p->j]ָ����е����һ����� 
                col[p->j]=p;  
            }
            
        }
        
        while(pn) // ������N���е�ʣ��Ԫ�ز������Q 
        {
            p=(OLink)malloc(sizeof(OLNode)); // ���ɾ���Q�Ľ�� 
            if(!p)
                exit(0);
                
            (*Q).tu++; // ����Ԫ������1 
            p->i=i; // ����㸳ֵ 
            p->j=pn->j;
            p->e=pn->e;
            p->right=NULL;
            pn=pn->right; // pnָ�������� 
            
            if((*Q).rhead[i]==NULL) // pΪ���еĵ�1����� 
                // p���ڸ��еı�ͷ��pqָ��p(���е����һ�����)
                (*Q).rhead[i]=pq=p;  
            else // ����pq��ָ���֮�� 
            {
                pq->right=p; // ����в��� 
                pq=p; // pqָ����е����һ����� 
            }
            
            if((*Q).chead[p->j]==NULL) // pΪ���еĵ�1�����
                // p���ڸ��еı�ͷ��col[p->j]ָ��p  
                (*Q).chead[p->j]=col[p->j]=p; 
            else // ����col[p->j]��ָ���֮�� 
            {
                col[p->j]->down=p; // ����в���
                // col[p->j]ָ����е����һ����� 
                col[p->j]=p;  
            }
            
        }
    }
    for(k=1;k<=(*Q).nu;k++)
        if(col[k]) // k���н�� 
            col[k]->down=NULL; //  ��������һ������downָ��Ϊ�� 
    free(col);
    return OK;
}

