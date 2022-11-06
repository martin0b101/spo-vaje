
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
    while ((bytes_read = read(file, buffer, BUFFER_SIZE)) > 0){
        printf("Bytes read %d\n", bytes_read);
        for (int i = 0; i < bytes_read; i++)
        {
            printf("%s", buffer);
            if (buffer[i] == '\n')
            {
                printf("\n\nFound new line on byte %d\n\n", i);
            }
        }
    }
    return 0;
}

int main(int argc, char *argv[]){

    if (argc > 4)
        printf("Error: to much arguments!\n");
    if (argc < 3)
        printf("Error: to much arguments!\n");

    
    //open file for writing
    int file = open(argv[1], O_WRONLY | O_APPEND | O_RDONLY);
    //append
    if (argv[2][0] == 'a')
    {
        append_text(file);
        /* code */
    }else if(argv[2][0] == 'd'){
        //delete 
        printf("delete\n");
        int n = atoi(argv[3]);
        delete_n_line(file, n);

    }else if(argv[2][0] == 'i'){
        //insert
    }else{
        printf("Wrong arguments!\n");
    }

    return 0;
}