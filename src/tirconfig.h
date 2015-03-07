/* tirconfig.h */
#ifndef	TIRCONFIG_H
#define	TIRCONFIG_H

// Buffer size
#define BUF_SIZE_CFG	1024
// Type of line data
#define TYPE_COMMENT	1
#define TYPE_SECTION	2
#define TYPE_PARAM		3
#define TYPE_UNKNOWN	-1
#define RESERVED_SECTION "reserved.tir"


struct type_kvp{
	char key[BUF_SIZE_CFG];
	char value[BUF_SIZE_CFG];
	struct type_kvp* next;
	struct type_kvp* last;
};

struct type_cfg{
	char section[BUF_SIZE_CFG];
	struct type_kvp* kvp;
	struct type_cfg* next;
	struct type_cfg* last;
};

struct type_cfg*  parse_cfg(const char* path, int* result);
void free_cfg(struct type_cfg* config);
char* get_cfg_value(const struct type_cfg* config, const char* section, const char* key);
void print_cfg(const struct type_cfg* cfg);

#endif
