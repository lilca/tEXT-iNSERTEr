#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "tir.h"
#include "base64convert.h"
#include "tirconfig.h"
#include "opt_strcat.h"

// Maximum of includes (Safty)
#define MAX_INCS	1000

// Message level
#define MSG_ERROR	1
#define MSG_WARNING	2
#define MSG_INFO	3

// Response code
#define RESP_YES	1
#define RESP_NO		2

#define VERSION_CODE	"0.5.4";

// Convert mode
#define CONV_BASE64	"base64"

// Default configuration file name
#define DEFAULT_CFG_FILE	"./tir.cfg"

// for automatically added line feed
#define AALF	1

// Prompt of functions
void init();
void parse_parameters(int c, char** v);
void invalid_check_parameters();
char* match_suffix(const char* suffix, const char* key);
void fix_parameters();
void get_current(char* path);
char* open_file(char* path);
void add_string_list(const char* val);
void init_tir_attributes(struct tir_attributes* tar);
void apply_files(char* f, char* path);
void set_reference_value(const char* ref, struct tir_reference* result);
char* copy_value(char* pos,  char* buf, int bias);
void get_reference_path(char* content, struct tir_attributes* result);
void insert_reference_file(const char* path, const char* cnv_mode, FILE* ofp);
void insert_reference_kvp(const struct tir_reference* ref, FILE* ofp);
void insert_reference_shell(const char* shell, FILE* ofp);
void print_version();
void print_usage();
void print_parameters();
void print_read_tircfg(const struct type_cfg* cfg, const char* path);
void print_default_words();
void print_message(int level, char* msg);
int ask_user(char* msg);
char* cutsuffix(char* in, char* tar);

// Parameters
char p_ifp[BUF_SIZE];	// Input file path in parameter
char p_ofp[BUF_SIZE];	// Output file path in parameter
char p_bw[BUF_SIZE];	// The beginning word in parameter
char p_ew[BUF_SIZE];	// The ending word in parameter
char p_cfg[BUF_SIZE];	// The name of default configuration file

// Global variable
int gl_cfg_read		= 0; // When config file was read, not zero
int gl_yes_flag		= 0;	// Default of all-yes-flag is false
int gl_makefile_flag	= 0;	// Default of makefile-flag is false
char gl_current[BUF_SIZE];		// Current path(place of input file)
struct type_cfg* gl_tir_cfg;	// "tir" config
struct str_list{
	int count;
	char list[MAX_INCS][BUF_SIZE];
};
struct str_list gl_str_list;

int main(int argc, char** argv){
	init();
	// Procedures of parameters
	parse_parameters(argc, argv);
	// When configuration file wasn't input
	if( !strcmp(p_cfg, "") ){
		strcpy(p_cfg, DEFAULT_CFG_FILE);
	}
	// Read configuration file, then parse
	gl_tir_cfg	= parse_cfg(p_cfg, &gl_cfg_read);
	fix_parameters();
	invalid_check_parameters();
	get_current(p_ifp);
	print_parameters();
	print_read_tircfg(gl_tir_cfg, p_cfg);
	// Read a input file
	char* all		= open_file(p_ifp);
	// Insert & output
	apply_files(all, p_ofp);
	// Free memory
	free_cfg(gl_tir_cfg);
	free(all);
	// Normal termination
	return 0;
}

void init(){
	strcpy(p_ifp, "");
	strcpy(p_ofp, "");
	strcpy(p_bw, "");
	strcpy(p_ew, "");
	strcpy(p_cfg, "");
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
	int idx;
	for(idx=1; idx<c; idx++){
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
		else
		if( !strcmp(cur, "-cfg") )
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
			if( !strcmp(preword, "-cfg") && strcmp(p_cfg, "") == 0 )
				strcpy(p_cfg, cur);
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

char* match_suffix(const char* suffix, const char* key){
	char tmp[BUF_SIZE];
	sprintf(tmp, "%s%s", key, suffix);
	return get_cfg_value(gl_tir_cfg, RESERVED_SECTION, tmp);
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
	// Begin word
	if( !strcmp(p_bw, "")  ){
		char* bw;
		if( (bw=match_suffix(suffix, "bw")) == NULL ){
			strcpy(p_bw, "/*[tir:begin]");
		}
		else{
			strcpy(p_bw, bw);
		}
	}
	// End word
	if( !strcmp(p_ew, "") ){
		char* ew;
		if( (ew=match_suffix(suffix, "ew")) == NULL ){
			strcpy(p_ew, "[tir:end]*/");
		}
		else{
			strcpy(p_ew, ew);
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
		return NULL;
	}
	// Get file size
	struct stat st;
	stat(path, &st);
	// Read all
	char* filedata = (char*)malloc(st.st_size * sizeof(char)+AALF);
	char buf[BUF_SIZE];			// Buffer size
	while( fgets(buf, BUF_SIZE, fp) != NULL ){
		strcat(filedata, buf);
	}
	fclose(fp);
	// Return pointer
	return filedata;
}

void add_string_list(const char* val){
	if( gl_str_list.count >= MAX_INCS )
		return;
	int idx;
	for(idx=0; idx<gl_str_list.count; idx++){
		if( !strcmp(gl_str_list.list[idx], val) )
			return;
	}
	strcpy(gl_str_list.list[idx], val);
	gl_str_list.count++;
}

void init_tir_attributes(struct tir_attributes* tar){
	tar->reference.type			= REF_TYPE_NULL;
	tar->reference.section[0]	= '\0';
	tar->reference.key[0]		= '\0';
	tar->reference.value[0]		= '\0';
	tar->convert_mode[0]		= '\0';
	tar->dependence[0]		= '\0';
}

void apply_files(char* f, char* path){
	if( f == NULL )
		return;
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
		if( fp == NULL ){
			exit(-1);
		}
	}
	// Initialize string list
	gl_str_list.count	= 0;
	// Searching
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
		init_tir_attributes(&tattr);
		// Parse attribute
		get_reference_path(pram, &tattr);
		// Edit target valuepath
		char tar_val[BUF_SIZE];
		if( tattr.reference.type == REF_TYPE_KEY ){
			strcpy(tar_val, p_cfg);
		}
		else
		if( tattr.reference.type == REF_TYPE_SHELL ){
			strcpy(tar_val, tattr.reference.value);
		}
		else
		if( tattr.reference.type == REF_TYPE_FILE ){
			char* val	= tattr.reference.value;
			if( strstr( val,"./") == val )
				strcpy(tar_val, val+2);
			else
			if( strstr(val, "/") == val )
				strcpy(tar_val, val+1);
			else
				strcpy(tar_val, val);
		}
		// Join current directory
		char target[BUF_SIZE];
		if( tattr.reference.type == REF_TYPE_FILE ){
			strcpy(target, gl_current);
		}
		else
			strcpy(target, "");
		// Optimize "../" to target path
		if( tattr.reference.type == REF_TYPE_FILE ){
			opt_strcat(target, tar_val);
		}
		else
			strcat(target, tar_val);
		//
		if( gl_makefile_flag ){
			// Dependence was not set
			if( !strcmp(tattr.dependence, "") ){
				if( tattr.reference.type != REF_TYPE_SHELL ){
					add_string_list(target);
				}
			}
			// was set
			else{
				add_string_list(tattr.dependence);
			}
		}
		// Output reference file
		if( !gl_makefile_flag ){
			switch(tattr.reference.type){
				case REF_TYPE_FILE:
					insert_reference_file(target, tattr.convert_mode, fp);
					break;
				case REF_TYPE_KEY:
					insert_reference_kvp(&tattr.reference, fp);
					break;
				case REF_TYPE_SHELL:
					insert_reference_shell(target, fp);
					break;
				default:
					break;
			}
		}
		// Move position
		pos = ptr_end + strlen(p_ew);
	}
	// Output command
	if( gl_makefile_flag ){
		// Output relative files
		int idx;
		for(idx=0; idx<gl_str_list.count; idx++){
			printf(" %s", gl_str_list.list[idx]);
		}
		printf("\n\ttir '%s' -o '%s' -bw '%s' -ew '%s' -y -cfg %s\n\n", p_ifp, p_ofp, p_bw, p_ew, p_cfg);
	}
	else{
		fclose(fp);
	}
	return;
}

void set_reference_value(const char* ref, struct tir_reference* result){
	// Reference shell command
	if( ref[0] == '>' ){
		result->type	= REF_TYPE_SHELL;
		strcpy(result->value, ref+1);
	}
	// Reference a file
	else
	if( ref[0] != '#' ){
		result->type	= REF_TYPE_FILE;
		strcpy(result->value, ref);
	}
	// Reference a config with section
	else
	if( ref[1] == '[' ){
		result->type	= REF_TYPE_KEY;
		char* pos	= strstr(ref, "]");
		int slen		= pos - ( ref+2 );
		int klen		= strlen(ref) - slen - 3;
		strncpy(result->section, ref+2, slen);
		result->section[slen]	= '\0';
		strncpy(result->key, pos+1, klen);
		result->key[klen]		= '\0';
	}
	// Reference a config without section
	else{
		result->type	= REF_TYPE_KEY;
		strcpy(result->key, ref+1);
		strcpy(result->section, "");
	}
}

char* copy_value(char* pos,  char* buf, int bias){
	// Move positision
	pos += bias;
	// Enclosed the string with double quotation
	if( *pos == '"'){
		// Move
		pos++;
		// To double quotation
		long idx	= 0;
		while( *pos != '\"' && *pos != '\0' ){
			if( *pos == '\\' )
				pos++;
			buf[idx] 	= *pos;
			idx++;
			pos++;
		}
		if( *pos == '\"' )
			pos++;
		// Terminate
		buf[idx] = '\0';
	}
	else{
		long idx	= 0;
		while( *pos != ' ' && *pos != '\t' && *pos != '\n' && *pos != '\0'){
			if( *pos == '\\' )
				pos++;
			buf[idx]	= *pos;
			idx++;
			pos++;
		}
		// Terminate
		buf[idx] = '\0';
	}
	return pos;
}
void get_reference_path(char* content, struct tir_attributes* result){
	// Get attribute keyword info
	char* kw_ref	= "ref=";
	int kwlen_ref	= strlen(kw_ref);
	char* kw_cnv	= "convert=";
	int kwlen_cnv	= strlen(kw_cnv);
	char* kw_dep	= "dependence=";
	int kwlen_dep	= strlen(kw_dep);
	// Result buffer clear
	strcpy(result->reference.section, "");
	strcpy(result->reference.key, "");
	strcpy(result->reference.value, "");
	strcpy(result->convert_mode, "");
	char* res_cnv	= result->convert_mode;
	char* res_dep	= result->dependence;
	// Searching
	char buf[BUF_SIZE];
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
			pos	= copy_value(pos, buf, kwlen_ref);
			continue;
		}
		// Matching dependence
		else
		if( !strncmp(pos, kw_dep, kwlen_dep ) ){
			pos	= copy_value(pos, res_dep, kwlen_dep);
			continue;
		}
		// Matching convert keyword
		else
		if( !strncmp(pos, kw_cnv, kwlen_cnv ) ){
			pos	= copy_value(pos, res_cnv, kwlen_cnv);
			continue;
		}
		// Skip next \t or \n or space
		else{
			while( *pos != ' ' && *pos != '\t' && *pos != '\n' && *pos != '\0')
				pos++;
		}
		pos++;
	}
	// Set reference value
	set_reference_value(buf, &(result->reference));
	return;
}

void insert_reference_file(const char* path, const char* cnv_mode, FILE* ofp){
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

void insert_reference_kvp(const struct tir_reference* ref, FILE* ofp){
	char* val	= get_cfg_value(gl_tir_cfg, ref->section, ref->key);
	if( val == NULL ){
		char str[BUF_SIZE];
		sprintf(str, "%s msg=\"Not found([%s]%s) \" %s",p_bw, ref->section, ref->key, p_ew);
		fwrite(str, sizeof(char), strlen(str), ofp);
		return;
	}
	fwrite(val, sizeof(char), strlen(val), ofp);
}

void insert_reference_shell(const char* shell, FILE* ofp){
	FILE* ostd;
	if( (ostd=popen(shell, "r")) == NULL ){
		char str[BUF_SIZE];
		sprintf(str, "%s msg=\"Shell script fault(>%s) \" %s",p_bw, shell, p_ew);
		fputs(str, ofp);
		return;
	}
	char buf[BUF_SIZE];
	while( fgets(buf, BUF_SIZE, ostd) != NULL ){
		fputs(buf, ofp);
	}
	pclose(ostd);
}

void print_version(){
	char* ver	= VERSION_CODE;
	printf("tir (tEXT-iNSERTEr)\n");
	printf("  version : %s\n", ver);
	printf("  github  : https://github.com/lilca/tEXT-iNSERTEr\n");
	return;
}

void print_usage(){
	printf("Usage:\n");
	printf("  -o  : Output file\n");
	printf("  -bw : Beginning word for inserting\n");
	printf("  -ew : Ending word for inserting\n");
	printf("        e.g. tir infile.c -o outfile.c -bw '//[tir:begin]' -ew '[tir:end]'\n");
	printf("  -cfg: Configuration file\n");
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
	printf("  Config file    = %s\n",p_cfg);
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
	printf("  Suffix         | Begin           | End\n");
	printf("  ---------------------------------------------\n");
	printf("  cfg            | ;[tir:begin]    | [tir:end];\n");
	printf("  htm(l) svg xml | <!--[tir:begin] | [tir:end]-->\n");
	printf("  sh rb py r     | #[tir:begin]    | [tir:end]#\n");
	printf("  other(e.g. c)  | /*[tir:begin]   | [tir:end]*/\n");
	return;
}

void print_read_tircfg(const struct type_cfg* cfg, const char* path){
	// When makefile-flag was on, return without print
	if( gl_makefile_flag )
		return;

	if( gl_cfg_read == 0 ){
		printf("Config file:\n");
		printf("  No read config file (%s)\n", path);
	}else{
		printf("Config file:\n");
		printf("  Read config file (%s)\n", path);
	}
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
	int idx;
	for(idx=0; idx<tar_len; idx++){
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
