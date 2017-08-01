#include <stdio.h>
#include<string.h>
#include <stdlib.h>

void opt_strcat(char* base, char* tar){
  const char* dlm = "../";
  int dlen = strlen(dlm);
  int len = strlen(tar);
  // Copy
  char* clone = (char*)malloc(len+1);
  strcpy(clone, tar);
  // Remove "./"
  char* index = clone;
  if( strncmp(index, "./", 2) == 0 )
    index += 2;
  // Counting "../"
  int amount = 0;
  int cmp = strncmp(index, dlm, dlen);
  for(;cmp == 0;){
    index += dlen;
    amount++;
    cmp = strncmp(index, dlm, dlen);
  }
  // remove
  if( amount == 0 ){
    strcat(base, index);
  }else{
    char cur = 0;
    char* zero = base + strlen(base);
    for(int cnt=0; cnt <= amount ;){
      cur = *(--zero);
      if( cur == '/' ){
        *zero = '\0';
        cnt++;
      }
    }
    strcat(base, "/");
    strcat(base, index);
  }
  free(clone);
  return;
}
/*
int main(){
  char base[100] = "./dir1/dir2/";
  opt_strcat(base,"./a.c");
  printf("%s\n", base);
}
*/