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
Status TransposeSMatrix(TSMatrix M,TSMatrix &T);	//稀疏矩阵的转置矩阵 

int main(int argc, char** argv) {
	TSMatrix M,T;
	CreateSMatrix(M);
	printf("矩阵M的输出为：\n");
	PrintSMatrix(M);
	TransposeSMatrix(M,T);
	printf("M的转置矩阵T为：\n");
	PrintSMatrix(T);
	return 0;
}

Status CreateSMatrix(TSMatrix &M){
	int k;
	printf("请输入原矩阵的行数列数和非零元个数\n");
	scanf("%d%d%d",&M.mu,&M.nu,&M.tu);
	printf("请输入矩阵对应的行数列数和非零元:\n");
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
	return OK;
}

Status TransposeSMatrix(TSMatrix M,TSMatrix &T){
	//采用三元组表存储表示，求稀疏矩阵M的转置矩阵T
	int p,col,q=0;
	T.mu=M.mu;T.nu=M.nu;T.tu=M.tu;
	if(T.tu){
		for(col=0;col<M.nu;++col)
		{
			for(p=0;p<M.tu;++p)
			{
				if(M.data[p].j==col)
				{
					T.data[q].i=M.data[p].j;
					T.data[q].j=M.data[p].i;
					T.data[q].e=M.data[p].e;
					++q;
				}
			}
		}
	} 
	return OK;
}
