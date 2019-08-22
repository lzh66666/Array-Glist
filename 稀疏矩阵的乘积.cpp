#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#define OK 1
#define ERROR 0
#define OVERFLOW -2
#define MAXSIZE 12500		//�������Ԫ���������ֵΪ12500
#define MAXRC 12500			//������з���Ԫ��λ�����ֵΪ12500 

typedef int Status;
typedef int ElemType;

typedef struct{
	int i,j;			//�÷���Ԫ�����±�����±� 
	ElemType e;
}Triple;

//���߼����ӵ�˳��� 
typedef struct{
	Triple data[MAXSIZE];		//����Ԫ��Ԫ��� 
	int rpos[MAXRC];			//���е�һ������Ԫ��λ�ñ� 
	int mu,nu,tu;				//������������������ͷ���Ԫ���� 
}RLSMatrix;

Status CreateSMatrix(RLSMatrix &M);		//ϡ�����Ĵ��� 
Status PrintSMatrix(RLSMatrix M);		//ϡ��������� 
Status MultSMatrix(RLSMatrix M,RLSMatrix N,RLSMatrix &Q);	//ϡ�����ĳ˻� 

int main(int argc, char** argv) {
	int i;
	RLSMatrix M,N,Q;
	CreateSMatrix(M);
	printf("����M�������\n");
	PrintSMatrix(M);
	CreateSMatrix(N);
	printf("����N�������\n");
	PrintSMatrix(N);
	i=MultSMatrix(M,N,Q);
	if(i==OK){
		printf("����Q=M*N�������\n");
		PrintSMatrix(Q);
	}
	else if(i==ERROR){
		printf("����N��������M��������һ�£���������"); 
	}
	
	return 0;
}

Status CreateSMatrix(RLSMatrix &M){
	int k;
	printf("������ԭ��������������ͷ���Ԫ����(��2 2 1)\n");
	scanf("%d%d%d",&M.mu,&M.nu,&M.tu);
	printf("����������Ӧ�����������ͷ���Ԫ(��0,0,1):\n");
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
	//�����˻�Q=M*N���������߼����Ӵ洢��ʾ 
	int arow,tp,brow,p,t,q,k,ccol;
	int ctemp[N.nu];
	if(M.nu!=N.mu)
		return ERROR; 
	Q.mu=M.mu;Q.nu=N.nu;Q.tu=0;		//Q�ĳ�ʼ��
	if(M.tu*N.tu!=0){		//Q�Ƿ������ 
		for(arow=0;arow<M.mu;++arow) 
		{
			for(k=0;k<Q.nu;k++)		//����ǰ��Ԫ���ۼ�������
			{
				ctemp[k]=0;
			}
			Q.rpos[arow]=Q.tu;
			if(arow<M.mu-1)
				tp=M.rpos[arow+1];
			else
				tp=M.tu;
			for(p=M.rpos[arow];p<tp;++p)	//�Ե�ǰ���е�ÿһ������Ԫ 
			{
				brow=M.data[p].j;		//�ҵ���ӦԪ��N�е��к� 
				if(brow<N.mu-1)
					t=N.rpos[brow+1];
				else
					t=N.tu;
				for(q=N.rpos[brow];q<t;++q)
				{
					ccol=N.data[q].j;		//�˻�Ԫ����Q�е��к� 
					ctemp[ccol]+=M.data[p].e*N.data[q].e;
				} //for q
			}		//���Q�е�crow(=arow)�еķ���Ԫ
			for(ccol=0;ccol<Q.nu;++ccol)		//�洢ѹ�����з���Ԫ
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
