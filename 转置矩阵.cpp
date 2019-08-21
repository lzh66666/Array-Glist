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
Status FastTransposeSMatrix(TSMatrix M,TSMatrix &T);	//稀疏矩阵的快速转置 

int main(int argc, char** argv) {
	TSMatrix M,T;
	CreateSMatrix(M);
	printf("矩阵M的输出：\n");
	PrintSMatrix(M);
	printf("矩阵M的快速转置矩阵T的输出:\n");
	FastTransposeSMatrix(M,T);
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
}

Status FastTransposeSMatrix(TSMatrix M,TSMatrix &T){
	//采用三元组顺序表存储表示，求稀疏矩阵M的转置矩阵T
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
		//求第col列中第一个非零元在b.data中的序号
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
