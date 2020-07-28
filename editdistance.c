#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INSERT_OP      0x01
#define DELETE_OP      0x02
#define SUBSTITUTE_OP  0x04
#define MATCH_OP       0x08
#define TRANSPOSE_OP   0x10

#define INSERT_COST	1
#define DELETE_COST	1
#define SUBSTITUTE_COST	1
#define TRANSPOSE_COST	1

int __GetMin3( int a, int b, int c);
int __GetMin4( int a, int b, int c, int d);
void backtrace( int *op_matrix, int col_size, char *str1, char *str2, int n, int m);
void print_alignment( char align_str[][8], int level);

int static stack = 0;

// 재귀적으로 연산자 행렬을 순회하며, 두 문자열이 최소편집거리를 갖는 모든 가능한 정렬(alignment) 결과를 출력한다.
// op_matrix : 이전 상태에 대한 연산자 정보(backtrace)가 저장된 행렬 (1차원 배열임에 주의!)
// col_size : op_matrix의 열의 크기
// str1 : 문자열 1
// str2 : 문자열 2
// n : 문자열 1의 길이
// m : 문자열 2의 길이
// level : 재귀호출의 레벨 (0, 1, 2, ...)
// align_str : 정렬된 문자쌍들의 정보가 저장된 문자열 배열 예) "a - a", "a - b", "* - b", "ab - ba"
void backtrace_main( int *op_matrix, int col_size, char *str1, char *str2, int n, int m, int level, char align_str[][8]){		//align_str[n+m][8]		col_size == m
	
	int op = col_size*(n-1) + m - 1;
		
	if(n == 0 && m ==0){
		stack++;
		printf("\n[%d]==============================\n", stack);
		print_alignment(align_str, level-1);
		
		level = 0;		
	}
	else if(n == 0 && m != 0){
		align_str[level][0] = '*'; 
		align_str[level][1] = ' ';
		align_str[level][2] = '-';
		align_str[level][3] = ' ';
		align_str[level][4] = str2[m-1];
		align_str[level][5] = 0;
			
		backtrace_main( op_matrix, col_size, str1, str2, n, m-1, level+1, align_str);
	}
	else if(n != 0 && m == 0){
		align_str[level][0] = str1[n-1]; 
		align_str[level][1] = ' ';
		align_str[level][2] = '-';
		align_str[level][3] = ' ';
		align_str[level][4] = '*';
		align_str[level][5] = 0;
			
		backtrace_main( op_matrix, col_size, str1, str2, n-1, m, level+1, align_str);
	}
	else{
		if(((op_matrix[op] & INSERT_OP) != 0) ){
			
			align_str[level][0] = '*'; 
			align_str[level][1] = ' ';
			align_str[level][2] = '-';
			align_str[level][3] = ' ';
			align_str[level][4] = str2[m-1];
			align_str[level][5] = 0;
			
			backtrace_main( op_matrix, col_size, str1, str2, n, m-1, level+1, align_str);
		}
		if(((op_matrix[op] & DELETE_OP) != 0) ){
			
			align_str[level][0] = str1[n-1]; 
			align_str[level][1] = ' ';
			align_str[level][2] = '-';
			align_str[level][3] = ' ';
			align_str[level][4] = '*';
			align_str[level][5] = 0;
			
			backtrace_main( op_matrix, col_size, str1, str2, n-1, m, level+1, align_str);
		}
		if((op_matrix[op] & SUBSTITUTE_OP) != 0){
			
			align_str[level][0] = str1[n-1]; 
			align_str[level][1] = ' ';
			align_str[level][2] = '-';
			align_str[level][3] = ' ';
			align_str[level][4] = str2[m-1];
			align_str[level][5] = 0;
			
			backtrace_main( op_matrix, col_size, str1, str2, n-1, m-1, level+1, align_str);
		}
		if((op_matrix[op] & MATCH_OP) != 0){
			
			align_str[level][0] = str1[n-1]; 
			align_str[level][1] = ' ';
			align_str[level][2] = '-';
			align_str[level][3] = ' ';
			align_str[level][4] = str2[m-1];
			align_str[level][5] = 0;
	
			backtrace_main( op_matrix, col_size, str1, str2, n-1, m-1, level+1, align_str);
		}
		if((op_matrix[op] & TRANSPOSE_OP) != 0){
			
			align_str[level][0] = str1[n-2];
			align_str[level][1] = str1[n-1];
			align_str[level][2] = ' ';
			align_str[level][3] = '-';
			align_str[level][4] = ' ';
			align_str[level][5] = str2[m-2];
			align_str[level][6] = str2[m-1];
			align_str[level][7] = 0;
			
			backtrace_main( op_matrix, col_size, str1, str2, n-2, m-2, level+1, align_str);
		}
	}
};

// 강의 자료의 형식대로 op_matrix를 출력 (좌하단(1,1) -> 우상단(n, m))
// 각 연산자를  괄호 안의 기호로 표시한다. 삽입(I), 삭제(D), 교체(S), 일치(M), 전위(T)
void print_matrix( int *op_matrix, int col_size, int n, int m){

	int size = col_size;
	//(n-i-1)*m + j
	for(int i = 0; i< n; i++){
		for (int j = 0; j<m; j++){
			if((op_matrix[n*m-size+j] & INSERT_OP) != 0)
				printf("I");
			if((op_matrix[n*m-size+j] & DELETE_OP) != 0)
				printf("D");
			if((op_matrix[n*m-size+j] & SUBSTITUTE_OP) != 0)
				printf("S");
			if((op_matrix[n*m-size+j] & MATCH_OP) != 0)
				printf("M");
			if((op_matrix[n*m-size+j] & TRANSPOSE_OP) != 0)
				printf("T");
			printf("\t");
		}
		printf("\n");
		size = size + col_size;
	}
};

// 두 문자열 str1과 str2의 최소편집거리를 계산한다.
// return value : 최소편집거리
// 이 함수 내부에서 print_matrix 함수와 backtrace 함수를 호출함
int min_editdistance( char *str1, char *str2){
	int n ;
	int m ;
	int **matrix;
	int op = 0;
	int value;
	int ins, del, sub, tra;
	
	n = strlen(str1);
	m = strlen(str2);
	
	int op_matrix[n*m];
	
	matrix = (int **) malloc(sizeof(int*) * (n+1));		//(n + 1) x (m + 1) 행렬 
	for(int i = 0; i<n+1; i++)
		matrix[i] = (int*)malloc(sizeof(int*) * (m+1));
	
	matrix[0][0] = 0;
	
	for(int i = 0; i<n+1; i++)
		matrix[i][0] = i;
	
	for(int j = 0; j<m+1; j++)
		matrix[0][j] = j;
		
	
	for(int i = 1; i<n+1; i++){
		for(int j = 1; j<m+1; j++)
			matrix[i][j] = -1;
	}	// 초기값 초기화
	
	for(int i = 1; i<n+1; i++){	
		for (int j = 1; j<m+1; j++){
			op_matrix[op] = 0;
						
			ins = matrix[i][j-1] + INSERT_COST;
			del = matrix[i-1][j] + DELETE_COST;
			sub = matrix[i-1][j-1] + SUBSTITUTE_COST;
			
			if (str1[i-1] == str2[j-1]){
					
				matrix[i][j] = matrix[i-1][j-1];	
				op_matrix[op] += MATCH_OP;
															
				if(ins == matrix[i][j])
					op_matrix[op] += INSERT_OP;
				if(del == matrix[i][j])
					op_matrix[op] += DELETE_OP;
					
				if(i>1 && j >1 && str1[i-1] == str2[j-2] && str1[i-2] == str2[j-1]){
					tra = matrix[i-2][j-2] + TRANSPOSE_COST;
					if(tra == matrix[i][j]){
						op_matrix[op] += TRANSPOSE_OP;
					}
				}
				op++;
			}
			else{		//  문자 서로 다를 때			
				if(i>1 && j >1 && str1[i-1] == str2[j-2] && str1[i-2] == str2[j-1]){
					
					tra = matrix[i-2][j-2] + TRANSPOSE_COST;
					
					matrix[i][j] = __GetMin4(ins, del, sub, tra);		// 현재의 matrix의 값은 다음 4개 중의 최솟값.
					
					if (ins == matrix[i][j])
						op_matrix[op] += INSERT_OP;
					if(del == matrix[i][j])
						op_matrix[op] += DELETE_OP;
					if(sub == matrix[i][j])
						op_matrix[op] += SUBSTITUTE_OP;
					if(tra == matrix[i][j])
						op_matrix[op] += TRANSPOSE_OP;
					
					op++;
					
				}
				else {
					
					matrix[i][j] = __GetMin3(ins, del, sub);
					
					if (ins == matrix[i][j])
						op_matrix[op] += INSERT_OP;
					if(del == matrix[i][j])
						op_matrix[op] += DELETE_OP;
					if(sub == matrix[i][j])
						op_matrix[op] += SUBSTITUTE_OP;
					
					op++;
				}
			}
		}
	}		// matrix랑 op_matrix 만들기
	
	print_matrix( op_matrix, m, n, m);
	backtrace( op_matrix, m, str1, str2, n, m);		// n, m은 strlen(str1), strlen(str2)
	stack = 0;
	
	value = matrix[n][m];
	
	for (int i = 0; i<n+1; i++)
		free(matrix[i]);
	free(matrix);
	
	return value;
};

////////////////////////////////////////////////////////////////////////////////
// 세 정수 중에서 가장 작은 값을 리턴한다.
int __GetMin3( int a, int b, int c)
{
	int min = a;
	if (b < min)
		min = b;
	if(c < min)
		min = c;
	return min;
}

////////////////////////////////////////////////////////////////////////////////
// 네 정수 중에서 가장 작은 값을 리턴한다.
int __GetMin4( int a, int b, int c, int d)
{
	int min = __GetMin3( a, b, c);
	return (min > d) ? d : min;
}

////////////////////////////////////////////////////////////////////////////////
// 정렬된 문자쌍들을 출력 
void print_alignment( char align_str[][8], int level)
{
	int i;
	
	for (i = level; i >= 0; i--)
	{
		printf( "%s\n", align_str[i]);
	}
}

////////////////////////////////////////////////////////////////////////////////
// backtrace_main을 호출하는 wrapper 함수
void backtrace( int *op_matrix, int col_size, char *str1, char *str2, int n, int m)
{
	char align_str[n+m][8]; // n+m strings
	
	backtrace_main( op_matrix, col_size, str1, str2, n, m, 0, align_str);
}

////////////////////////////////////////////////////////////////////////////////
int main()
{
	char str1[30];
	char str2[30];
	char a[3];
	
	int distance;
	
	printf( "INSERT_COST = %d\n", INSERT_COST);
	printf( "DELETE_COST = %d\n", DELETE_COST);
	printf( "SUBSTITUTE_COST = %d\n", SUBSTITUTE_COST);
	printf( "TRANSPOSE_COST = %d\n", TRANSPOSE_COST);
	
	while( fscanf( stdin, "%s\t%s", str1, str2) != EOF)
	{
		printf( "\n==============================\n");
		printf( "%s vs. %s\n", str1, str2);
		printf( "==============================\n");
		
		distance = min_editdistance( str1, str2);
		
		printf( "\nMinEdit(%s, %s) = %d\n", str1, str2, distance);
	}
	return 0;
}
