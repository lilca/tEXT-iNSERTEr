#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "tirconfig.h"

// Struct for parse result
struct st_parse{
	int status;
	char key[BUF_SIZE_CFG];
	char value[BUF_SIZE_CFG];
};

// Current section
char cur_sec[BUF_SIZE_CFG];

// Prompt of functions
struct type_cfg* create_default_cfg();
void init_status(struct st_parse* status);
void parse_line(const char* line, struct st_parse* status);
void set_current_section(const char* value);
struct type_cfg* put_cfg(struct type_cfg* cfg, const char* section, const struct st_parse* status);
struct type_kvp* put_kvp(struct type_kvp* kvp, const struct st_parse* status);
void print_kvp(const struct type_kvp* kvp, const char* section);
void free_kvp(struct type_kvp* kvp);

#define DEFAULT_CONFIG_VALUES	16
char* dcv[DEFAULT_CONFIG_VALUES]	= {
	"bw.html=<!--[tir:begin]",
	"ew.html=[tir:end]-->",
	"bw.htm=<!--[tir:begin]",
	"ew.htm=[tir:end]-->",
	"bw.xml=<!--[tir:begin]",
	"ew.xml=[tir:end]-->",
	"bw.sh=#[tir:begin]",
	"ew.sh=[tir:end]#",
	"bw.r=#[tir:begin]",
	"ew.r=[tir:end]#",
	"bw.rb=#[tir:begin]",
	"ew.rb=[tir:end]#",
	"bw.py=#[tir:begin]",
	"ew.py=[tir:end]#",
	"bw.cfg=;[tir:begin]",
	"ew.cfg=[tir:end];",
};

/*/ for test
int main(int argc, char** argv){
	if( argc < 2 ){
		printf("Please set a input file path\n");
		exit(-1);
	}
	char* inpath	= argv[1];

	// Test block
	struct type_cfg* list = parse_cfg(inpath);
	print_cfg(list);
	free_cfg(list);
}
*/

struct type_cfg* create_default_cfg(){
	struct type_cfg* res	= NULL;
	set_current_section(RESERVED_SECTION);
	struct st_parse status;
	char* line;
	int idx;
	for(idx=0; idx<DEFAULT_CONFIG_VALUES; idx++){
		line	= dcv[idx];
		parse_line(line, &status);
		res	= put_cfg(res, cur_sec, &status);
	}
	return res;
}

void init_status(struct st_parse* status){
	status->status	= TYPE_UNKNOWN;
	strcpy(status->key, "");
	strcpy(status->value, "");	
}

void parse_line(const char* line, struct st_parse* status){
	// Copy and add return code
	char tmp	[BUF_SIZE_CFG];
	strcpy(tmp, line);
	strcat(tmp, "\n");
	// Extract keys for parse
	char first			= tmp[0];
	char last;
	const char* lastpos	= strstr(tmp, "\n");
	last				= *(lastpos-1);
	char* eqpos	= strstr(tmp, "=");
	// Initialize status
	init_status(status);
	// Line comment
	if( first == ';' ){
		status->status	= TYPE_COMMENT;
		return;
	}
	// Section
	else
	if( first == '[' && last == ']' ){
		status->status	= TYPE_SECTION;
		int len		=  lastpos - tmp -2;
		strncpy(status->value, (tmp+1), len);
		status->value[len] = '\0';
		return;
	}
	// Parameter
	else
	if( eqpos != NULL ){
		int len;
		status->status	= TYPE_PARAM;
		len			= eqpos-tmp;
		strncpy(status->key, tmp, len);
		status->key[len]	= '\0';
		len			= lastpos-(eqpos+1);
		strncpy(status->value,  (eqpos+1), len);
		status->value[len]	= '\0';
		return;
	}
	// Unknown
	return;
}

void set_current_section(const char* value){
	strcpy(cur_sec, value);
}

struct type_cfg* put_cfg(struct type_cfg* cfg, const char* section, const struct st_parse* status){
	// Duplicate check
	struct type_cfg* idx	= cfg;
	while( idx != NULL ){
		if( !strcmp(idx->section, section) ){
			idx->kvp	= put_kvp(idx->kvp, status);
			return cfg;
		}
		idx	= idx->next;
	}
	// Craete data
	struct type_cfg* tmp	= (struct type_cfg*)malloc(sizeof(struct type_cfg));
	strcpy(tmp->section, section);
	tmp->kvp	= put_kvp(tmp->kvp, status);
	// Link
	if( cfg == NULL ){
		tmp->next	= NULL;
		tmp->last	= tmp;
	}else{
		tmp->next	= cfg;
		tmp->last	= cfg->last;
	}
	return tmp;
}

struct type_kvp* put_kvp(struct type_kvp* kvp, const struct st_parse* status){
	// Duplicate check
	struct type_kvp* idx	= kvp;
	while( idx != NULL ){
		if( !strcmp(idx->key, status->key) ){
			strcpy(idx->value, status->value);
			return kvp;
		}
		idx	= idx->next;
	}
	// Craete data
	struct type_kvp* tmp	= (struct type_kvp*)malloc(sizeof(struct type_kvp));
	strcpy(tmp->key, status->key);
	strcpy(tmp->value, status->value);
	// Link
	if( kvp == NULL ){
		tmp->next	= NULL;
		tmp->last	= tmp;
	}else{
		tmp->next	= kvp;
		tmp->last	= kvp->last;
	}
	return tmp;
}

void print_cfg(const struct type_cfg* cfg){
	const struct type_cfg* idx	= cfg;
	printf("Print tir config:\n");
	while( idx != NULL ){
		print_kvp(idx->kvp, idx->section);
		idx	= idx->next;
	}	
}

void print_kvp(const struct type_kvp* kvp, const char* section){
	const struct type_kvp* idx	= kvp;
	while( idx != NULL ){
		printf("[%s][%s-%s(%ld)]\n", section, idx->key, idx->value, strlen(idx->value));
		idx	= idx->next;
	}	
}

struct type_cfg*  parse_cfg(const char* path, int* result){
	struct type_cfg* cfg	= create_default_cfg();
	set_current_section("");
	// Open a input file
	FILE *fp = fopen(path, "r");
	if( fp == NULL ){
		return cfg;
	}
	// Get file size
	struct stat st;
	stat(path, &st);
	// Read all
	char buf[BUF_SIZE_CFG];			// Buffer size
	// Allocate
	struct st_parse status;
	//
	while( fgets(buf, BUF_SIZE_CFG, fp) != NULL ){
		parse_line(buf, &status);
		switch(status.status){
			case TYPE_COMMENT:
				// Go through
				break;
			case TYPE_SECTION:
				set_current_section(status.value);
				break;
			case TYPE_PARAM:
				cfg	= put_cfg(cfg, cur_sec, &status);
				break;
			default:
				// Go through
				break;
		}
	}
	fclose(fp);
	// Config file was read
	*result	= 1;
	return cfg;
}

void free_cfg(struct type_cfg* config){
	struct type_cfg* idx	= config;
	while( idx != NULL ){
		struct type_cfg* trash	= idx;
		{
			struct type_kvp* kvp	= idx->kvp;
			while( kvp != NULL ){
				struct type_kvp* kvp_trash	= kvp;
				kvp	= kvp->next;
				free(kvp_trash);
			}			
		}
		idx	= idx->next;
		free(trash);
	}
}

char* get_cfg_value(const struct type_cfg* config, const char* section, const char* key){
	const struct type_cfg* idx	= config;
	while( idx != NULL ){
		if( !strcmp(idx->section, section) ){
			struct type_kvp* kvp	= idx->kvp;
			while( kvp != NULL ){
				if( !strcmp(kvp->key, key) )
					return kvp->value;
				kvp	= kvp->next;
			}
		}
		idx	= idx->next;
	}
	return NULL;
}
