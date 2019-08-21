#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#define OK 1
#define ERROR 0
#define MAXSIZE 12500		//假设非零元个数的最大值为12500

typedef int Status;
typedef int ElemType;

typedef struct {
	int i,j;			//该非零元的行下标和列下标 
	ElemType e;
}Triple;

typedef struct{
	Triple data[MAXSIZE];		//非零元三元组表 
	int mu,nu,tu;				//矩阵的行数、列数、和非零元个数 
}TSMatrix;

Status CreateSMatrix(TSMatrix &M);		//稀疏矩阵的创建 
Status PrintSMatrix(TSMatrix M);		//稀疏矩阵的输出 

int main(int argc, char** argv) {
	TSMatrix M;
	CreateSMatrix(M);
	PrintSMatrix(M);
	return 0;
}

Status CreateSMatrix(TSMatrix &M){
	int k;
	printf("请输入原矩阵的行数列数和非零元个数\n");
	scanf("%d%d%d",&M.mu,&M.nu,&M.tu);
	printf("请输入矩阵对应的行数列数和非零元(如0,0,0):\n");
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
