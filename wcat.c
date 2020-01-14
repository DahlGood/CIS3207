#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

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
        printf(content);
    }
    //fgets(content, size+1, file);
    
    fclose(file);

    free(content);

    return 0;
}