#ifndef __BF_T_H__
#define __BF_T_H__
#include <stdio.h>
#include <stdlib.h>
//#define __DEBUG__
//the class for the implementation
typedef struct 
{
	int a[8];
	int b;
} h_param;
typedef unsigned char byte;
class bf_t
{
	public:
		bf_t();
		~bf_t();
		int find(char *q);
		void insert(char *s);
	private:
		static const long int MAXP = 3037000943;
		static const int SIZE = 2000000;
	
		h_param *param;
		byte ** table;

		long h_fun(char *q, int* a, int b);

};
//the interface for the homework
bf_t * create_bf();
void insert_bf(bf_t * b, char *s);
int is_element(bf_t *b, char * q); 
#endif
