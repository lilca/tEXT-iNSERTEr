#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "tir.h"
#include "base64convert.h"

// Maximum of includes
#define MAX_INCS	1000

// Buffer size
#define BUF_SIZE	1024

// Message level
#define MSG_ERROR	1
#define MSG_WARNING	2
#define MSG_INFO	3

// Response code
#define RESP_YES	1
#define RESP_NO		2

// Convert mode
#define CONV_BASE64	"base64"

// Prompt of functions
void init();
void parse_parameters(int c, char** v);
void invalid_check_parameters();
void fix_parameters();
void get_current(char* path);
char* open_file(char* path);
void apply_files(char* f, char* path);
void get_reference_path(char* content, struct tir_attributes* result);
void include_reference_file(char* path, char* cnv_mode, FILE* ofp);
void print_version();
void print_usage();
void print_parameters();
void print_default_words();
void print_message(int level, char* msg);
int ask_user(char* msg);
char* cutsuffix(char* in, char* tar);

// Parameters
char p_ifp[BUF_SIZE];	// Input file path in parameter
char p_ofp[BUF_SIZE];	// Output file path in parameter
char p_bw[BUF_SIZE];	// The beginning word in parameter
char p_ew[BUF_SIZE];	// The ending word in parameter

// Global variable
int gl_yes_flag			= 0;	// Default of all-yes-flag is false
int gl_makefile_flag	= 0;	// Default of makefile-flag is false
char gl_current[BUF_SIZE];		// Current path(place of input file)

int main(int argc, char** argv){
	init();
	// Procedures of parameters
	parse_parameters(argc, argv);
	fix_parameters();
	invalid_check_parameters();
	get_current(p_ifp);
	print_parameters();
	// Read a input file
	char* all = open_file(p_ifp);
	// Insert & output
	apply_files(all, p_ofp);
	// Free memory
	free(all);
	// Normal termination
	return 0;
}

void init(){
	strcpy(p_ifp, "");
	strcpy(p_ofp, "");
	strcpy(p_bw, "");
	strcpy(p_ew, "");
	return;
}

void parse_parameters(int c, char** v){
	char* preword = "";	// Previous word
	// When arguments are not, display help info.
	if( c == 1){
		print_version();
		print_usage();
		print_default_words();
		exit(0);
	}
	for(int idx=1; idx<c; idx++){
		char* cur = v[idx];	// Current word
		if( !strcmp(cur, "-h") ){
			print_version();
			print_usage();
			print_default_words();
			exit(0);
		}
		else
		if( !strcmp(cur, "-y") )
			gl_yes_flag = !gl_yes_flag;
		else
		if( !strcmp(cur, "-makefile") )
			gl_makefile_flag = !gl_makefile_flag;
		else
		if( !strcmp(cur, "-o") )
			preword = cur;
		else
		if( !strcmp(cur, "-bw") )
			preword = cur;
		else
		if( !strcmp(cur, "-ew") )
			preword = cur;
		else{
			if( !strcmp(preword, "-o") && strcmp(p_ofp, "") == 0 )
				strcpy(p_ofp, cur);
			else
			if( !strcmp(preword, "-bw") && strcmp(p_bw, "") == 0 )
				strcpy(p_bw, cur);
			else
			if( !strcmp(preword, "-ew") && strcmp(p_ew, "") == 0 )
				strcpy(p_ew, cur);
			else
			if( strcmp(p_ifp, "") == 0 )
				strcpy(p_ifp, cur);
			// Clear previous word
			preword = "";
		}
	}	
	return;
}

void invalid_check_parameters(){
	struct stat st1;
	if( stat(p_ifp, &st1) != 0 ){
		char buf[BUF_SIZE];
		strcpy(buf, p_ifp);
		print_message( MSG_ERROR, strcat(buf, " is not exist.") );
	}
	if( !gl_yes_flag ){
		struct stat st2;
		if( stat(p_ofp, &st2) == 0 ){
			char buf[BUF_SIZE];
			strcpy(buf, p_ofp);
			print_message( MSG_INFO, strcat(buf, " is exist already.") );
			if( ask_user("Override? <y/n>") == RESP_NO ){
				print_message( MSG_ERROR, "Program was aborted by the user." );			
			}
		}
	}
	return;
}

void fix_parameters(){
	// When the outputfile wasn't input
	if( !strcmp(p_ofp, "") ){
		// Cut "tir" suffix when there was it
		char buf[BUF_SIZE];
		strcpy(buf, p_ifp);
		strcpy(p_ofp, cutsuffix(buf, ".tir"));
	}
	// Check a suffix of input file
	char* suffix = strrchr(p_ofp, '.');
	// When the beginning word wasn't input
	if( !strcmp(suffix, ".html") || !strcmp(suffix, ".htm") || !strcmp(suffix, ".xml") ){
		if( !strcmp(p_bw, "") ){
			strcpy(p_bw, "<!--[tir:begin]");
		}
		if( !strcmp(p_ew, "") ){
			strcpy(p_ew, "[tir:end]-->");
		}
	}
	else
	if( !strcmp(suffix, ".sh") || !strcmp(suffix, ".r") || !strcmp(suffix, ".rb") || !strcmp(suffix, ".py") ){
		if( !strcmp(p_bw, "") ){
			strcpy(p_bw, "#[tir:begin]");
		}
		if( !strcmp(p_ew, "") ){
			strcpy(p_ew, "[tir:end]#");
		}		
	}
	else{
		if( !strcmp(p_bw, "") ){
			strcpy(p_bw, "/*[tir:begin]");
		}
		if( !strcmp(p_ew, "") ){
			strcpy(p_ew, "[tir:end]*/");
		}		
	}
	return;
}

void get_current(char* path){
	char* pos = strrchr(path, '/');
	if( pos == NULL ){
		strcpy(gl_current, "./");
		return;
	}
	strncpy(gl_current, path, pos - path +1);
	return;
}

char* open_file(char* path){
	// Open a input file
	FILE *fp = fopen(path, "r");
	if( fp == NULL ){
		char str[BUF_SIZE];
		strcpy(str, path);
		print_message(MSG_ERROR, strcat("Can't open a file:", str) );
	}
	// Get file size
	struct stat st;
	stat(path, &st);
	// Read all
	char* filedata = (char*)malloc(st.st_size * sizeof(char));
	char buf[BUF_SIZE];			// Buffer size
	while( fgets(buf, BUF_SIZE, fp) != NULL ){
		strcat(filedata, buf);
	}
	fclose(fp);
	// Return pointer
	return filedata;
}

void apply_files(char* f, char* path){
	int count		= 0;
	int find_count	= 0;	// キーワードを探した回数
	char* pos		= f;
	FILE *fp;
	// Print makefile info
	if( gl_makefile_flag ){
		printf("# Makefile info:\n");
		printf("%s: %s",p_ofp, p_ifp);
	}
	else{
		// Open output file
		fp = fopen(path, "w");
	}
	while( find_count <= MAX_INCS ){
		// Count up
		find_count++;
		// Get begin position
		char* ptr_begin	= strstr(pos, p_bw);
		if( ptr_begin == NULL ){
			// Output rest
			if( !gl_makefile_flag ){
				fputs(pos, fp);
			}
			break;			
		}
		// Output until a begin word
		if( !gl_makefile_flag ){
			int len = ptr_begin - pos;
			char* temp = (char*)malloc(len * sizeof(char)+1);
			strncpy(temp, pos, len );
			*(temp+len) = '\0';
			fputs(temp, fp);
			free(temp);
		}
		// Shift
		ptr_begin += strlen(p_bw);
		// Get end position
		char* ptr_end	= strstr(ptr_begin, p_ew);
		if( ptr_end == NULL )
			break;
		// Extract parameters in input file
		char pram[BUF_SIZE];
		strncpy(pram, ptr_begin, ptr_end - ptr_begin );
		*(pram + (ptr_end - ptr_begin)) = '\0';
		// Create struct for attributes
		struct tir_attributes tattr;
		char ref_attr[BUF_SIZE];
		char cnv_attr[BUF_SIZE];
		tattr.reference		= ref_attr;
		tattr.convert_mode	= cnv_attr;
		// Parse attribute
		get_reference_path(pram, &tattr);
		// Edit target path
		char target[BUF_SIZE];
		strcpy(target, gl_current);
		strcat(target, tattr.reference);
		if( gl_makefile_flag )
			printf(" %s", target);
		// Output reference file
		if( !gl_makefile_flag ){
			include_reference_file(target, tattr.convert_mode, fp);
		}
		// Move position
		pos = ptr_end + strlen(p_ew);
	}
	if( gl_makefile_flag )
		printf("\n\ttir '%s' -o '%s' -bw '%s' -ew '%s' -y\n\n", p_ifp, p_ofp, p_bw, p_ew);
	else{
		fclose(fp);
	}
	return;
}

void get_reference_path(char* content, struct tir_attributes* result){
	// Get attribute keyword info
	char* kw_ref	= "ref=";
	int kwlen_ref	= strlen(kw_ref);
	char* kw_cnv	= "convert=";
	int kwlen_cnv	= strlen(kw_cnv);
	// Result buffer clear
	char* res_ref	= result->reference;
	char* res_cnv	= result->convert_mode;
	*res_ref		= 0;
	*res_cnv		= 0;
	// Searching
	char* pos = content;
	while( *pos != '\0' ){
		// Skip
		if( *pos == ' ' || *pos == '\t' || *pos == '\n' ){
			pos++;
			continue;
		}
		// Matching reference keyword
		else
		if( !strncmp(pos, kw_ref, kwlen_ref ) ){
			// Move positision
			pos += kwlen_ref;
			// Enclosed the string with double quotation
			if( *pos == '"'){
				// Move
				pos++;
				// To double quotation
				long idx = 0;
				while( *pos != '\"' && *pos != '\0' ){
					*(res_ref+idx) = *pos;
					idx++;
					pos++;
				}
				if( *pos == '\"' )
					pos++;
				// Terminate
				*(res_ref+idx) = '\0';
				continue;
			}
			else{
				long idx = 0;
				while( *pos != ' ' && *pos != '\t' && *pos != '\n' && *pos != '\0'){
					*(res_ref+idx) = *pos;
					idx++;
					pos++;
				}
				// Terminate
				*(res_ref+idx) = '\0';
				continue;
			}
		}
		// Matching convert keyword
		else
		if( !strncmp(pos, kw_cnv, kwlen_cnv ) ){
			// Move positision
			pos += kwlen_cnv;
			// Enclosed the string with double quotation
			if( *pos == '"'){
				// Move
				pos++;
				// To double quotation
				long idx = 0;
				while( *pos != '\"' && *pos != '\0' ){
					*(res_cnv+idx) = *pos;
					idx++;
					pos++;
				}
				if( *pos == '\"' )
					pos++;
				// Terminate
				*(res_cnv+idx) = '\0';
				continue;
			}
			else{
				long idx = 0;
				while( *pos != ' ' && *pos != '\t' && *pos != '\n' && *pos != '\0'){
					*(res_cnv+idx) = *pos;
					idx++;
					pos++;
				}
				// Terminate
				*(res_cnv+idx) = '\0';
				continue;
			}
		}
		// Skip next \t or \n or space
		else{
			while( *pos != ' ' && *pos != '\t' && *pos != '\n' && *pos != '\0')
				pos++;
		}
		pos++;
	}
	return;
}

void include_reference_file(char* path, char* cnv_mode, FILE* ofp){
	int	base64flag	= 0;
	if( strcmp(CONV_BASE64, cnv_mode) == 0 )
		base64flag	= 1;
	FILE* incfp;
	// Open input file
	if( base64flag == 1 )
		incfp = fopen(path, "rb");
	else
		incfp = fopen(path, "r");
	// Output error message
	if( incfp == NULL ){
		char str[BUF_SIZE];
		sprintf(str, "%s msg=\"Can't open a file:%s\" %s",p_bw, path, p_ew);
		fputs(str, ofp);
		strcpy(str, "Can't open a file:");
		strcat(str, path);
		print_message(MSG_WARNING, str);
		return;
	}
	// Insert
	if( base64flag == 1 ){
		// Get file size
		struct stat st;
		stat(path, &st);
		// Read all
		unsigned char* filedata = (unsigned char*)malloc(st.st_size * sizeof(unsigned char));
		fread(filedata, sizeof(unsigned char), st.st_size, incfp);
		// Convert data
		char* base64code	= base64encode(filedata, st.st_size);
		// Output base64code
		long outsize		= strlen(base64code);
		fwrite(base64code, sizeof(unsigned char), outsize, ofp);
		free(filedata);
		free(base64code);
	}
	else{
		char buf[BUF_SIZE];
		while( fgets(buf, BUF_SIZE, incfp) != NULL ){
			fputs(buf, ofp);
		}
	}
	fclose(incfp);
}

void print_version(){
	printf("Text InserteR\n");
	printf("  version : 0.2.0\n");
	printf("  github  : https://github.com/lilca/tir\n");
	return;
}

void print_usage(){
	printf("Usage:\n");
	printf("  -o  : Output file\n");
	printf("  -bw : Beginning word for inserting\n");
	printf("  -ew : Ending word for inserting\n");
	printf("        e.g. tir infile.c -o outfile.c -bw '//[tir:begin]' -ew '[tir:end]'\n");
	printf("  -h  : Display help\n");
	printf("  -y  : To override without asking\n");
	printf("  -makefile:\n");
	printf("        Output makefile info.\n");
	return;
}

void print_parameters(){
	// When makefile-flag was on, return without print
	if( gl_makefile_flag )
		return;

	printf("Parameters:\n");
	printf("  Input file     = %s\n",p_ifp);
	printf("  Output file    = %s\n",p_ofp);
	printf("  Beginning word = %s\n",p_bw);
	printf("  Ending word    = %s\n",p_ew);
	printf("  Override       = ");
	if( gl_yes_flag )
		printf("TRUE\n");
	else
		printf("FALSE\n");
	printf("  Makefile       = ");
	if( gl_makefile_flag )
		printf("TRUE\n");
	else
		printf("FALSE\n");
	return;
}

void print_default_words(){
	printf("Default words:\n");
	printf("  Suffix       | Begin           | End\n");
	printf("  ---------------------------------------------\n");
	printf("  html htm xml | <!--[tir:begin] | [tir:end]-->\n");
	printf("  sh rb py r   | #[tir:begin]    | [tir:end]#\n");
	printf("  other(e.g. c)| /*[tir:begin]   | [tir:end]*/\n");
	return;
}

void print_message(int level, char* msg){
	switch(level){
		case MSG_ERROR:
			printf("ERROR: %s\n",msg);
			exit(-1);
			break;
		case MSG_WARNING:
			printf("WARNING: %s\n",msg);
			break;
		case MSG_INFO:
			printf("INFO: %s\n",msg);
			break;
	}
	return;
}

int ask_user(char* msg){
	char rs;
	printf("%s\n", msg);
	while( rs != 'y' && rs != 'n' ){
		rs = getchar();
	}
	if( rs == 'y' )
		return RESP_YES;
	return RESP_NO;
}

char* cutsuffix(char* in, char* tar){
	// Get lengths
	int in_len	= strlen(in);
	int tar_len	= strlen(tar);
	// When var"in" was shorter than var"tar", return var"in"
	if( in_len < tar_len )
		return in;
	// Compare
	for(int idx=0; idx<tar_len; idx++){
//	for(char* idx=tar+tar_len-1; idx>=tar; idx--){
		// When onece was differnt, return var"in"
		if( *(in+in_len-idx-1) != *(tar+tar_len-idx-1) )
			return in;
	}
	// Cut
	*(in+in_len-1-(tar_len-1))	= '\0';
	// When comparison was correct, return var"in"
	return in;
}
