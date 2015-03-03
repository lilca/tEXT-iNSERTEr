/* tir.h */
#ifndef	TIR_H
#define	TIR_H

// Buffer size
#define BUF_SIZE	1024

// Reference type
#define REF_TYPE_NULL	0
#define REF_TYPE_FILE	1
#define REF_TYPE_KEY	2
#define REF_TYPE_SHELL	3

struct tir_reference {
	int type;
	char section[BUF_SIZE];
	char key[BUF_SIZE];
	char value[BUF_SIZE];
};

struct tir_attributes {
	struct tir_reference reference;
	char convert_mode[BUF_SIZE];
	char dependence[BUF_SIZE];
};

#endif
