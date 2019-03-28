#include "pthread_shortpath.h"

int main(int argc, char const *argv[]) {
  // Variables for reading file
  char line[256], filename[10];
  FILE *fp;

  fp = fopen(argv[1], "r");

  // Error if file cannot be opened
  if (fp == NULL) {
    perror("Error while opening the file.\n");
    exit(EXIT_FAILURE);
  }

  // Variables to split lines read
  int pos = 0;
  char separators[3] = ":, ";
  char com[3];
  char *p;

  //
  while(fgets(line, sizeof(line), fp)) {
    if ((p = strtok(line, separators))) {
      printf("%s\n-------\n", p);
      while((p = strtok(NULL, separators))) {
        printf("%s\n", p);
      }
      printf("\n-------\n");
    }
    /**
    p = strtok(line, separators);
    strncpy(com, p, sizeof(com));

    pos++;
    printf("Line %i: %s\n", pos, line);
    p = strtok(NULL, separators);
    printf("%s\n", p);
    */
  }

  fclose(fp);

  return 0;
}
