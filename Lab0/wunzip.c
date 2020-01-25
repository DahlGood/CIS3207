#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    //Some conditions to prevent bad input.
    if(argc < 2) {
        printf("wunzip: file1 [file2 ...]\n");
        exit(1);
    }

    //Looping through files
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
        
        int running_char_length = 1;
        char current_char;

        //+1 accounting for the end of file marker.
        //Reads specified zipped file format. int, char, int, char...
        while(fread(&running_char_length, sizeof(int), 1, file) == 1) {
            fread(&current_char, sizeof(char), 1, file);
            for(int i = 0; i < running_char_length; i++) {  
                //Writes original unzipped file.
                fwrite(&current_char, sizeof(char), 1, stdout);
                
            }
            
        }

        fflush(stdout);
        fclose(file);
        free(content);

    }
    
    return 0;
}