#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

        //+1 accounting for the end of file marker.
        while(fgets(content, size+1, file) != NULL) {
                fflush(stdout);
                int running_char_length = 1;
                for(int i = 1; i <= strlen(content); i++) {
                    if(content[i] == content[i-1]) {
                        running_char_length++;
                    }
                    else {
                        fwrite(&running_char_length, sizeof(int), 1, stdout);
                        fwrite(&content[i-1], sizeof(char), 1, stdout);
                        running_char_length = 1;
                    }

                }

            }        

        fclose(file);

        free(content);

        printf("\n");

    }
    
    return 0;
}