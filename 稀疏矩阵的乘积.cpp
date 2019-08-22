#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#define OK 1
#define ERROR 0
#define OVERFLOW -2
#define MAXSIZE 12500		//假设非零元个数的最大值为12500
#define MAXRC 12500			//假设各行非零元的位置最大值为12500 

typedef int Status;
typedef int ElemType;

typedef struct{
	int i,j;			//该非零元的行下标和列下标 
	ElemType e;
}Triple;

//行逻辑链接的顺序表 
typedef struct{
	Triple data[MAXSIZE];		//非零元三元组表 
	int rpos[MAXRC];			//各行第一个非零元的位置表 
	int mu,nu,tu;				//矩阵的行数、列数、和非零元个数 
}RLSMatrix;

Status CreateSMatrix(RLSMatrix &M);		//稀疏矩阵的创建 
Status PrintSMatrix(RLSMatrix M);		//稀疏矩阵的输出 
Status MultSMatrix(RLSMatrix M,RLSMatrix N,RLSMatrix &Q);	//稀疏矩阵的乘积 

int main(int argc, char** argv) {
	int i;
	RLSMatrix M,N,Q;
	CreateSMatrix(M);
	printf("矩阵M的输出：\n");
	PrintSMatrix(M);
	CreateSMatrix(N);
	printf("矩阵N的输出：\n");
	PrintSMatrix(N);
	i=MultSMatrix(M,N,Q);
	if(i==OK){
		printf("矩阵Q=M*N的输出：\n");
		PrintSMatrix(Q);
	}
	else if(i==ERROR){
		printf("输入N的行数和M的列数不一致！！！！！"); 
	}
	
	return 0;
}

Status CreateSMatrix(RLSMatrix &M){
	int k;
	printf("请输入原矩阵的行数列数和非零元个数(如2 2 1)\n");
	scanf("%d%d%d",&M.mu,&M.nu,&M.tu);
	printf("请输入矩阵对应的行数列数和非零元(如0,0,1):\n");
	for(k=0;k<M.tu;k++)
	{
		scanf("%d,%d,%d",&M.data[k].i,&M.data[k].j,&M.data[k].e);
	}
	int row,t;
	int num[M.mu];
	for(row=0;row<M.mu;++row)
	{
		num[row]=0;
	}
	for(t=0;t<M.tu;++t)
	{
		++num[M.data[t].i];
	}
	M.rpos[0]=0;
	for(row=1;row<M.mu;++row)
	{
		M.rpos[row]=M.rpos[row-1]+num[row-1];
	}
	return OK;
}

Status PrintSMatrix(RLSMatrix M){
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

Status MultSMatrix(RLSMatrix M,RLSMatrix N,RLSMatrix &Q){
	//求矩阵乘积Q=M*N，采用行逻辑链接存储表示 
	int arow,tp,brow,p,t,q,k,ccol;
	int ctemp[N.nu];
	if(M.nu!=N.mu)
		return ERROR; 
	Q.mu=M.mu;Q.nu=N.nu;Q.tu=0;		//Q的初始化
	if(M.tu*N.tu!=0){		//Q是非零矩阵 
		for(arow=0;arow<M.mu;++arow) 
		{
			for(k=0;k<Q.nu;k++)		//将当前各元素累加器清零
			{
				ctemp[k]=0;
			}
			Q.rpos[arow]=Q.tu;
			if(arow<M.mu-1)
				tp=M.rpos[arow+1];
			else
				tp=M.tu;
			for(p=M.rpos[arow];p<tp;++p)	//对当前行中的每一个非零元 
			{
				brow=M.data[p].j;		//找到对应元在N中的行号 
				if(brow<N.mu-1)
					t=N.rpos[brow+1];
				else
					t=N.tu;
				for(q=N.rpos[brow];q<t;++q)
				{
					ccol=N.data[q].j;		//乘积元素在Q中的列号 
					ctemp[ccol]+=M.data[p].e*N.data[q].e;
				} //for q
			}		//求得Q中第crow(=arow)行的非零元
			for(ccol=0;ccol<Q.nu;++ccol)		//存储压缩该行非零元
			{
				if(ctemp[ccol]){
					if((Q.tu)>MAXSIZE)
						return ERROR;
					Q.data[Q.tu].i=arow;
					Q.data[Q.tu].j=ccol;
					Q.data[Q.tu].e=ctemp[ccol];
					Q.tu++; 
				}//if
			 } 
		}//for arow
	} //if
	return OK;
}
