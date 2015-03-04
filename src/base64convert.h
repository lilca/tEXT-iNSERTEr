/* base64convert.h */
#ifndef	BASE64CONVERT_H
#define	BASE64CONVERT_H

char* base64encode(unsigned char* data, long size);
unsigned char* base64decode(char* base64, long size, long* outsize);

#endif
