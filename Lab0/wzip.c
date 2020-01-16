#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    for(int i = 1; i < argc; i++) {
        FILE* file = fopen(argv[i], "rb");
        if(file == NULL) {
            printf("wzip: cannot open file\n");
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
        /*
        while(fgets(content, size+1, file) != NULL) {
            printf("%s", content);
        }
        */
        int previous_char = 0;
        for(int i < 0; i == size; i++) {
            int current_char = fgetsc(file);
            
        }
        int current_char = fgetc(file);

    
        fclose(file);

        free(content);

        printf("\n");

    }
    
    return 0;
}