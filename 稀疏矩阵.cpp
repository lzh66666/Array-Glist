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

int main(int argc, char** argv) {
	TSMatrix M;
	CreateSMatrix(M);
	PrintSMatrix(M);
	return 0;
}

Status CreateSMatrix(TSMatrix &M){
	int k;
	printf("������ԭ��������������ͷ���Ԫ����\n");
	scanf("%d%d%d",&M.mu,&M.nu,&M.tu);
	printf("����������Ӧ�����������ͷ���Ԫ(��0,0,0):\n");
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
