#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "base64convert.h"

unsigned char* get_file_data(char* path, long* size_addr);
void put_file_data(char* path, unsigned char* data, long size);
unsigned char b2n(char in);

const char* table		= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// for test
/*
int main(void){
	// Input
	printf("Convert test strings:\n");
	char* instring0 	= "Now on sale!";
	char* instring1 	= "Wanabeeeee!";
	char* instring2 	= "Serbstverstaentlich";
	long outsize;
	printf("%s\n",instring0);
	long insize		= strlen(instring0);
	char* res		= base64encode((unsigned char*)instring0, insize);
	printf("%s\n",res);
	insize			= strlen(res);
	char* res2 		= (char*)base64decode(res, insize, &outsize);
	printf("%s\n",res2);
	free(res);
	free(res2);
	//
	printf("%s\n",instring1);
	insize		= strlen(instring1);
	res		= base64encode((unsigned char*)instring1, insize);
	printf("%s\n",res);
	insize			= strlen(res);
	res2 		= (char*)base64decode(res, insize, &outsize);
	printf("%s\n",res2);
	free(res);
	free(res2);
	//
	printf("%s\n",instring2);
	insize		= strlen(instring2);
	res		= base64encode((unsigned char*)instring2, insize);
	printf("%s\n",res);
	insize			= strlen(res);
	res2 		= (char*)base64decode(res, insize, &outsize);
	printf("%s\n",res2);
	free(res);
	free(res2);
	
	char* infile			= "./testicon.png";
	unsigned char* data		= get_file_data(infile, &insize);
	res						= base64encode(data, insize);
	insize					= strlen(res);
	unsigned char* res3 	= base64decode(res, insize, &outsize);
	put_file_data("./testicon2.png", res3, outsize);
	free(data);
	free(res);
	free(res3);

	return 0;
}
*/

void put_file_data(char* path, unsigned char* data, long size){
	FILE *fp = fopen(path, "wb+");
	fwrite(data, sizeof(unsigned char), size, fp);
	fclose(fp);
	return;
}
unsigned char* get_file_data(char* path, long* size_addr){
	// Open a input file
	FILE *fp = fopen(path, "rb");
	if( fp == NULL ){
		printf("Can't open:%s", path);
	}
	// Get file size
	struct stat st;
	stat(path, &st);
	*size_addr	= st.st_size;
	// Read all
	unsigned char* filedata = (unsigned char*)malloc(st.st_size * sizeof(unsigned char));
	fread(filedata, sizeof(unsigned char), st.st_size, fp);
	fclose(fp);
	// Return pointer
	return filedata;
}
char* base64encode(unsigned char* data, long size){
	unsigned char c0, c1, c2;
	int bit_div		= ( size * 8 + 6-1 ) / 6;
	int bit_mod		= size * 8 % 6;
	int char_div	= ( bit_div + 4-1 ) / 4;
	int char_mod	= bit_div % 4;
	int res_size	= char_div * 4;
	char* res		= (char*)malloc( res_size +1);
	char* res_p		= res;
	int idx;
	for(idx=0; idx<size; idx+=3){
		switch( size - idx ){
		case 1:
			c0			= data[idx+0];
			res_p[0]	= table[ c0 >> 2 ];
			res_p[1]	= table[ (c0 & 0x03) << 4 ];
			res_p		+= 2;
			break;
		case 2:
			c0			= data[idx+0];
			c1			= data[idx+1];
			res_p[0]	= table[ c0 >> 2 ];
			res_p[1]	= table[ (c0 & 0x03) << 4 | c1 >> 4 ];
			res_p[2]	= table[ (c1 & 0x0f) << 2 ];
			res_p		+= 3;
			break;
		default:
			c0			= data[idx+0];
			c1			= data[idx+1];
			c2			= data[idx+2];
			res_p[0]	= table[ c0 >> 2 ];
			res_p[1]	= table[ (c0 & 0x03) << 4 | c1 >> 4 ];
			res_p[2]	= table[ (c1 & 0x0f) << 2 | c2 >> 6 ];
			res_p[3]	= table[ c2 & 0x3f ];
			res_p		+= 4;
			break;
		}
	}
	for(idx=char_mod; idx>0; idx--)
		*(res_p++)	= '=';
	res[res_size]	= 0;
	return res;
}
unsigned char* base64decode(char* base64, long size, long* outsize){
	long real_size;
	unsigned char c0, c1, c2, c3;
	if( base64[ size - 1 - 2 ] == '=' )
		real_size	= size - 3;
	else
	if( base64[ size - 1 - 1 ] == '=' )
		real_size	= size - 2;
	else
	if( base64[ size - 1 ] == '=' )
		real_size	= size - 1;
	else
		real_size	= size;
	*outsize				= real_size*6/8;
	unsigned char* res		= (unsigned char*)malloc(real_size*6/8);
	unsigned char* res_p	= res;
	int rest_bit			= 8;
	int idx;
	for(idx=0; idx<real_size; idx+=4){
		switch( real_size - idx  ){
		case 2:
			c0	= b2n(base64[idx+0]);
			c1	= b2n(base64[idx+1]);
			res_p[0]	= c0 << 2 | ( c1 & 0x30 ) >> 4;
			res_p		+= 1;
			break;
		case 3:
			c0	= b2n(base64[idx+0]);
			c1	= b2n(base64[idx+1]);
			c2	= b2n(base64[idx+2]);
			res_p[0]	= c0 << 2 | ( c1 & 0x30 ) >> 4;
			res_p[1]	= c1 << 4 | ( c2 & 0x3c ) >> 2;
			res_p		+= 2;
			break;
		default:
			c0	= b2n(base64[idx+0]);
			c1	= b2n(base64[idx+1]);
			c2	= b2n(base64[idx+2]);
			c3	= b2n(base64[idx+3]);
			res_p[0]	= c0 << 2 | ( c1 & 0x30 ) >> 4;
			res_p[1]	= c1 << 4 | ( c2 & 0x3c ) >> 2;
			res_p[2]	= c2 << 6 | c3;
			res_p		+= 3;
			break;
		}
	}
	*res_p	= 0;
	return res;
}
unsigned char b2n(char in){
	if( in >= 'A' && in <= 'Z' )
		return in - 'A';
	if( in >= 'a' && in <= 'z' )
		return in - 'a' + 26;
	if( in >= '0' && in <= '9' )
		return in - '0' + 52;
	if( in == '+' )
		return 62;
	if( in == '/' )
		return 63;
	return 255;
}