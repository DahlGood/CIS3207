#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    if(argc < 2) {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }
    else {
        int size = 0;
        char* content = (char *)malloc(size + 1);

        for(int i = 1; i < argc; i++) {
            FILE* file = fopen(argv[i], "rb");
            if(file == NULL) {
                printf("wzip: cannot open file\n");
                exit(1);
            }

            int size_alt = size;
            //Getting size of file.
            fseek(file, 0, SEEK_END);
            size += ftell(file);
            rewind(file);

            char* buffer = (char *)malloc(size - size_alt);
            if(buffer == NULL) {
                printf("MALLOC failed at malloc buffer\n");
                exit(1);
            }

            //Allocating memory for the content in the file.
            content = (char *)realloc(content, size);
            if(content == NULL) {
                printf("MALLOC failed\n");
                exit(1);
            }

            //+1 accounting for the end of file marker.
            while(fgets(buffer, size+1, file) != NULL) {
                strcat(content, buffer);
            }        

            fclose(file);

            free(buffer);

        }

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

        free(content);

    }

    
    
    return 0;
}