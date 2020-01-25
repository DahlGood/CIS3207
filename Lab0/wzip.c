#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {


    if(argc < 2) {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }
    //This else is definitely not needed but I dont want to go and remove all the extra tabs.
    else {
        //Intializing a global size and allocating memory.
        int size = 0;
        char* content = (char *)malloc(size + 1);

        //Looping through files.
        for(int i = 1; i < argc; i++) {

            FILE* file = fopen(argv[i], "rb");
            if(file == NULL) {
                printf("wzip: cannot open file\n");
                exit(1);
            }

            //Saving current size
            int size_alt = size;

            //Getting size of file.
            fseek(file, 0, SEEK_END);
            size += ftell(file);
            rewind(file);

            //Allocating temporary space for file with only the current file size.
            char* buffer = (char *)malloc(size - size_alt);
            if(buffer == NULL) {
                printf("MALLOC failed at malloc buffer\n");
                exit(1);
            }

            //Allocating memory for the content in the file.
            //This space contains all of the files combined.
            content = (char *)realloc(content, size);
            if(content == NULL) {
                printf("MALLOC failed\n");
                exit(1);
            }

            //+1 accounting for the end of file marker.
            //Getting individual file content and concatinating it with all other file content.
            while(fgets(buffer, size+1, file) != NULL) {
                strcat(content, buffer);
            }        

            fclose(file);
            free(buffer);

        }

        //Running through ALL file content and compressing it.
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