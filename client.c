// #include "networking.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <ctype.h>

#define YEL "\x1B[33m"
#define GRN "\x1B[32m"
#define CYN "\x1B[36m"
#define RED "\x1B[31m"
#define RESET "\x1B[0m"

// check if a string has only numbers
// 0: not solely digits(tabs/newlines are not digits)
// 1: solely digits
int isNum(char *inp) {
    int i;
    for (i = 0; i < (int)strlen(inp); i++) {
        if (! isdigit(inp[i])) {
            return 0;
        }
    } return 1;
}



void process(char * s) {
    while (*s) {
        if (*s >= 'a' && *s <= 'z')
        *s = ((*s - 'a') + 13) % 26 + 'a';
        else  if (*s >= 'A' && *s <= 'Z')
        *s = ((*s - 'a') + 13) % 26 + 'a';
        s++;
    }
}

int calculate(int length, int density) {
    int (*map)[length] = calloc(sizeof(int[length][length]),1);
    int num_trees = 0;
    int i,j;
    for (i = 0; i < length; i++) {
        for (j = 0; j < length; j++) {
            map[i][j] = 0;
            if (rand() % 100 < density) {
                map[i][j] = 1;
                num_trees++;
            }
        }
    }

    int * fire_locations = (int *)malloc((int)(length*length*2.1));
    int num_fire = 1;
    int time_count = 0;
    fire_locations[0] = length / 2;
    fire_locations[1] = length / 2;
    map[fire_locations[0]][fire_locations[1]] = 2;

    // Test print of the entire map
    /*
    for (i = 0; i < length; i++) {
    for (j = 0; j < length; j++) {
    if (map[i][j] == 0){
    printf("0");
}
else{
printf("%d",map[i][j]);
}
}
printf("\n");
}
printf("\n");
*/

while (num_fire) {
    int * temp_locations = (int *)malloc(length*length*3);
    temp_locations[0] = '\0';
    int num_temp = 0;
    int k;
    for (k = 0; k < num_fire; k++){
        // m++;
        int row = fire_locations[2*k];
        int col = fire_locations[2*k + 1];
        map[row][col] = 0;
        if (row != 0 && map[row-1][col]==1){
            temp_locations[2*num_temp] = row - 1;
            temp_locations[2*num_temp + 1] = col;
            map[row-1][col] = 2;
            num_temp++;
        }
        if (row != length-1 && map[row+1][col]==1){
            temp_locations[2*num_temp] = row + 1;
            temp_locations[2*num_temp + 1] = col;
            map[row+1][col] = 2;
            num_temp++;
        }
        if (col != 0 && map[row][col-1]==1){
            temp_locations[2*num_temp] = row;
            temp_locations[2*num_temp + 1] = col - 1;
            map[row][col-1] = 2;
            num_temp++;
        }
        if (col != length-1 && map[row][col + 1]==1){
            temp_locations[2*num_temp] = row;
            temp_locations[2*num_temp + 1] = col + 1;
            map[row][col+1] = 2;
            num_temp++;
        }
        // printf("%d, m:%d\n",k,m);
        // printf("%d\n", k);
    }
    fire_locations = temp_locations;
    num_fire = num_temp;
    time_count++;

    /*
    int l,n;
    for (l = 0; l < length; l++) {
    for (n = 0; n < length; n++) {
    if (map[l][n] == 0){
    printf("0");
}
else{
printf("%d",map[l][n]);
}
}
printf("\n");
}
printf("\n");
*/
}
// printf("legend:\n0: dirt/empty block\n1: tree\n2: fire\n\n");
return time_count;
}

// using 2 cores, makes each client run calculate 10 times
// and return the average for a total of 2 * 10 trials(cores * 10 times)
int meanCalc(int dim, int den) {
    // forking twice
    int pipe_fd[2];
    // 0: Read, 1: Write
    pipe(pipe_fd);

    int num_cores = 2;
    int i;
    for (i = 0; i < num_cores; i++) {
        int child_pid = fork();
        if (child_pid == 0) { // child
            close(pipe_fd[0]); // closes the reading end

            int seed = time(NULL);
            srand(seed + i);
            int result = calculate(dim, den);
            printf("current resultn: %d\n", result);
            char str_result[20];
            sprintf(str_result, "%d", result);
            write(pipe_fd[1], str_result, strlen(str_result) + 1);
            exit(0);
        }
    }

    int all_results[num_cores];
    int num_results = 0;
    close(pipe_fd[1]); //Closes the write end of the pipe
    while (num_results < num_cores){
        char returned_result[20];
        read(pipe_fd[0], returned_result, 20);
        all_results[num_results] = atoi(returned_result);
        num_results++;
    }

    int averaged = 0;
    for (i = 0; i < num_cores; i++){
        averaged += all_results[i];
        printf("\navg[%d]\n", all_results[i]);
    }
    printf("\nres[0]: %d\n", all_results[0]);
    printf("\nres[1]: %d\n", all_results[1]);
    return averaged / num_cores;
}

// int main(int argc, char **argv) {
//     int den, dim;
//     int inp[100];
//     while (1) {
//         // get user input
//         printf("\nDesired density: \n");
//         fgets(inp, 100, stdin);
//         *strchr(inp, '\n') = 0;
//         if (isNum(inp)) {
//             den = atoi(inp);
//             if (den > 100 || den < 0) {
//                 printf(RED "\nInvalid input. Please enter a density between 0 and 100.\n" RESET);
//             } else {
//                 break;
//             }
//         } else {
//             printf(RED "\nInvalid input. Please just enter numbers.\n" RESET);
//         }
//     }
//
//     while (1) {
//         printf("\nDesired dimensions: \n");
//         fgets(inp, 100, stdin);
//         *strchr(inp, '\n') = 0;
//         if (isNum(inp)) {
//             dim = atoi(inp);
//             break;
//         } else {
//             printf(RED "\nInvalid input. Please just enter numbers.\n" RESET);
//         }
//     }
//     printf(YEL "\nYou entered: den -> %d, dim -> %d\n" RESET, den, dim);
//     int avg = meanCalc(dim, den);
//     printf("\n\nAVERAGE: [%d]\n\n", avg);
// }

int main(int argc, char **argv) {

    int server_socket;
    char buffer[BUFFER_SIZE];

    if (argc == 2)
    server_socket = client_setup( argv[1]);
    else
    server_socket = client_setup( TEST_IP );

    while (read(server_socket, buffer, sizeof(buffer))) {
        int trials = 10;
        int i, sum;
        printf("[client] received: [%s]\n", buffer);
<<<<<<< HEAD

        // change buffer into dim/den
        char str[1024];
        strcpy(str, buffer);
        const char delim[2] = ",";
        char *token;
        int *arr = calloc(2, sizeof(int));
        int i = 0;
        /* get the first token */
        token = strtok(str, delim);

        /* walk through other tokens */
        while(token != NULL ) {
            printf("%s\n", token );
            arr[i] = atoi(token);
            i++;
            token = strtok(NULL, delim);
        }
        int dim = arr[0];
        int den = arr[1];
        free(arr);
        // // process(buffer);
        // int temp = atoi(buffer);
        // temp += 1;
        // sprintf(buffer, "%d", temp);

        for (i = 0; i < trials; i++) {
            sum += meanCalc(dim, den);
        } sum = sum / trials;
        sprintf(buffer, "%d,%d", den, sum);

        write(server_socket, buffer, sizeof(buffer));
        printf("[client] sent: [%s]. avg was: [%d] for density [%d]\n", buffer, sum, den);
=======
        // process(buffer);
        int temp = atoi(buffer);
        if (temp == 1){
            printf("%s","not ready");
            sleep(1);
            sprintf(buffer, "%d", temp);
        }
        else {
            //calculate here
            printf("%s","ready");
            sleep(1);
            sprintf(buffer, "%d", temp);
        }

        write(server_socket, buffer, sizeof(buffer));
        //printf("[client] sent: [%s]. temp was: [%d]\n", buffer, temp);
>>>>>>> 1428e6d9de63976a01a25107e3227a38509f4953
    }//end read loop
}

// parser
//
// char str[20] = buffer;
// const char delim[2] = ",";
// char *token;
// int *arr = calloc(2, sizeof(int));
// int i = 0;
// /* get the first token */
// token = strtok(str, delim);
//
// /* walk through other tokens */
// while( token != NULL ) {
//     printf( "%s\n", token );
//     arr[i] = atoi(token);
//     i++;
//     token = strtok(NULL, delim);
// }
// int den = arr[0];
// int result = arr[1];
// free(arr);
