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
                //char* found = (char *)malloc(strlen(argv[2]));
                //int previous_char = content[0];
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


        /*
        for(int i = 0; i < size; i++) {
            int current_char = fgetsc(file);
            if(current_char == previous_char) {
                running_char_length++;
            }
            else {
                fwrite(running_char_length, sizeof(int), 1, stdout);
                fwrite(previous_char, sizeof(char), 1, stdout);
            }

            previous_char = current_char;

        }
        */
        

        fclose(file);

        free(content);

        printf("\n");

    }
    
    return 0;
}