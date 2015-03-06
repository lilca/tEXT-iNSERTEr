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

struct type_cfg*  parse_cfg(const char* path);
void free_cfg(struct type_cfg* config);
char* get_cfg_value(const struct type_cfg* config, const char* section, const char* key);
void print_cfg(const struct type_cfg* cfg);

#endif
