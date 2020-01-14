#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    if(argv[2] == NULL) {
        printf("Please specify something to search for.\n");
        exit(1);
    }

    FILE* file = fopen(argv[1], "r");
    if(file == NULL) {
        printf("Problem openeing file with the name: %s\n", argv[1]);
        exit(1);
    }

    //Getting size of file.
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    rewind(file);

    //Allocating memory for the content in the file.
    char* content = (char *)malloc(size);
    if(content == NULL) {
        printf("MALLOC failed");
        exit(1);
    }
    
    printf("%s is %d MB\n", argv[1], size);

    //+1 accounting for the end of file marker.
    //PROBLEM. Program crashes if the file being read contains an identifer. Ex: if the file contains %s.
    while(fgets(content, size+1, file) != NULL) {
        //char* found = (char *)malloc(strlen(argv[2]));
        if(strstr(content, argv[2])) {
            printf(content);
        }

    }
    //fgets(content, size+1, file);
    
    fclose(file);

    free(content);

    return 0;
}