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

        char last_char;
        int current_char_length = 1;

        //Looping through files.
        for(int i = 1; i < argc; i++) {

            FILE* file = fopen(argv[i], "rb");
            if(file == NULL) {
                printf("wzip: cannot open file\n");
                exit(1);
            }

            if(i == 1) {
                //Guarentees last_char isnt empty when the first file is being read.
                last_char = fgetc(file);
            }

            char current_char;
            while(!feof(file)) {
                current_char = fgetc(file);

                //Prevents EOF marker from being counted when multiple files are being zipped.
                if(current_char == EOF) {
                    break;
                }
                if(current_char != last_char) {
                    fwrite(&current_char_length, sizeof(int), 1, stdout);
                    fwrite(&last_char, sizeof(char), 1, stdout);
                    current_char_length = 1;
                    last_char = current_char;
                }
                else {
                    last_char = current_char;
                    current_char_length++;
                }
                
            }
            if(i == argc-1) {
                fwrite(&current_char_length, sizeof(int), 1, stdout);
                fwrite(&last_char, sizeof(char), 1, stdout);
                break;
            }

        }

    }

    
    
    return 0;
}