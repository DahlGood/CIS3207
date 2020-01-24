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
        /*
        fseek(file, 0, SEEK_END);
        int size = ftell(file);
        rewind(file);
        */

        //Allocating memory for the content in the file.
        /*
        printf("%d\n", size);
        char* content = (char *)malloc(size+1);
        if(content == NULL) {
            printf("MALLOC failed\n");
            exit(1);
        }
        */

        //+1 accounting for the end of file marker.
        /*
        while(fgets(content, size+1, file)) {
            printf("%s\n", content);
        }
    
        fclose(file);

        free(content);
        */

        char buffer[200];
        
        while(fgets(buffer, 200, file)) {
            printf("%s", buffer);
        }

        fclose(file);

    }
    
    return 0;
}