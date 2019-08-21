#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#define OK 1
#define ERROR 0
#define MAXSIZE 12500		//�������Ԫ���������ֵΪ12500

typedef int Status;
typedef int ElemType;

typedef struct {
	int i,j;			//�÷���Ԫ�����±�����±� 
	ElemType e;
}Triple;

typedef struct{
	Triple data[MAXSIZE];		//����Ԫ��Ԫ��� 
	int mu,nu,tu;				//������������������ͷ���Ԫ���� 
}TSMatrix;

Status CreateSMatrix(TSMatrix &M);		//ϡ�����Ĵ��� 
Status PrintSMatrix(TSMatrix M);		//ϡ��������� 
Status FastTransposeSMatrix(TSMatrix M,TSMatrix &T);	//ϡ�����Ŀ���ת�� 

int main(int argc, char** argv) {
	TSMatrix M,T;
	CreateSMatrix(M);
	printf("����M�������\n");
	PrintSMatrix(M);
	printf("����M�Ŀ���ת�þ���T�����:\n");
	FastTransposeSMatrix(M,T);
	PrintSMatrix(T);
	return 0;
}

Status CreateSMatrix(TSMatrix &M){
	int k;
	printf("������ԭ��������������ͷ���Ԫ����\n");
	scanf("%d%d%d",&M.mu,&M.nu,&M.tu);
	printf("����������Ӧ�����������ͷ���Ԫ:\n");
	for(k=0;k<M.tu;k++)
	{
		scanf("%d,%d,%d",&M.data[k].i,&M.data[k].j,&M.data[k].e);
	}
	return OK;
}

Status PrintSMatrix(TSMatrix M){
	int m,n;
	int k=0;
	for(m=0;m<M.mu;m++)
	{
		for(n=0;n<M.nu;n++)
		{
			if(M.data[k].i==m && M.data[k].j==n){
				printf("%d ",M.data[k++].e);
			}
			else{
				printf("%d ",0);
			}
		}
		printf("\n");
	}
}

Status FastTransposeSMatrix(TSMatrix M,TSMatrix &T){
	//������Ԫ��˳���洢��ʾ����ϡ�����M��ת�þ���T
	int col,t,p,q;
	int num[M.nu],cpot[M.nu];
	T.mu=M.nu;T.nu=M.mu;T.tu=M.tu;
	if(T.tu){
		for(col=0;col<M.nu;++col)
		{
			num[col]=0;
		}
		for(t=0;t<M.tu;++t)
		{
			++num[M.data[t].j];
		}
		cpot[0]=0;
		//���col���е�һ������Ԫ��b.data�е����
		for(col=1;col<M.nu;++col)
		{
			cpot[col]=cpot[col-1]+num[col-1];
		} 
		for(p=0;p<M.tu;++p)
		{
			col=M.data[p].j;
			q=cpot[col];
			T.data[q].i=M.data[p].j;
			T.data[q].j=M.data[p].i;
			T.data[q].e=M.data[p].e;
			++cpot[col];
		}
	} 
	return OK;
} 
