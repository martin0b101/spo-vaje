
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

#define BUFFER_SIZE 1000

//Writes data from stanadrd input to file.
int append_text(int* file){
    char buffer[BUFFER_SIZE];
    int bytes_read;
    while ((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0)
    {
        if(write(file, buffer, bytes_read) != bytes_read){
            printf("error write");
            return 1;
        }
    }
    return 0;
}

//Deletes line on number n.
int delete_n_line(int* file, int* line_number){
    char buffer[BUFFER_SIZE];
    int number_of_lines = 0;
    int start_place;
    int end_place;
    int bytes_read;
    printf("File desc in d: %d\n", file);
    while ((bytes_read = read(file, buffer, BUFFER_SIZE)) > 0){
        for (int i = 0; i < bytes_read; i++)
        {
            if (buffer[i] == '\n')
            {
                printf("\n\nFound new line on byte %d\n\n", i);
            }
        }
    }
    
    if (bytes_read == -1){
        perror("Error reading: ");
    }
    printf("Bytes read %d", bytes_read);
    return 0;
}

int main(int argc, char *argv[]){

    if (argc > 4)
        printf("Error: to much arguments!\n");
    if (argc < 3)
        printf("Error: to few arguments!\n");

    
    //open file for writing
    //printf("File desc o: %d\n", file);
    //append
    if (argv[2][0] == 'a')
    {
        int file = open(argv[1], O_WRONLY | O_APPEND | O_RDONLY);
        append_text(file);
        if (close(file) > 0)
            perror("Can't close the file: ");
        /* code */
    }else if(argv[2][0] == 'd'){
        //delete 
        int file = open(argv[1], O_RDONLY);
        printf("delete\n");
        int n = atoi(argv[3]);
        int bytes_read = 0;
        char buffer[BUFFER_SIZE];        
        delete_n_line(file, n);

    }else if(argv[2][0] == 'i'){
        //insert
    }else{
        printf("Wrong arguments!\n");
    }

    return 0;
}