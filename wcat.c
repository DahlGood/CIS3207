#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    for(int i = 1; i < argc; i++) {
        FILE* file = fopen(argv[i], "r");
        if(file == NULL) {
            printf("wcat: cannot open file\n");
            exit(1);
        }

        //Getting size of file.
        fseek(file, 0, SEEK_END);
        int size = ftell(file);
        rewind(file);

        //Allocating memory for the content in the file.
        char* content = (char *)malloc(size);
        if(content == NULL) {
            printf("MALLOC failed\n");
            exit(1);
        }
    
        printf("%s is %d MB\n", argv[i], size);

        //+1 accounting for the end of file marker.
        while(fgets(content, size+1, file) != NULL) {
            printf("%s", content);
        }
    
        fclose(file);

        free(content);

        printf("\n");

    }
    
    return 0;
}