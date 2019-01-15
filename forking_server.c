#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include "networking.h"

#define YEL "\x1B[33m"
#define GRN "\x1B[32m"
#define CYN "\x1B[36m"
#define RED "\x1B[31m"
#define RESET "\x1B[0m"

void subserver(int client_socket, int shmid, int shmid2, int dim);


int has_fire(int length, int *** map){
    int i,j;
    for (i=0; i<length; i++){
        for (j=0; j<length; j++){
            if (*map[i][j] == 2){
                return 1;
            }
        }
    }
    return 0;
}

int non_frontier(int length, int density) {
    int map[length][length];
    int i,j;
    for (i = 0; i < length; i++) {
        for (j = 0; j < length; j++) {
            map[i][j] = 0;
            if (rand() % 100 < density) {
                map[i][j] = 1;
            }
        }
    }

    map[length/2][length/2] = 2;

    int turns = 0;
    int has_fire = 1;
    while (has_fire){
        int m,n;

        /*
        for (m=0; m<length; m++){
        for (n=0; n<length; n++){
        printf("%d,",map[m][n]);
    }
    printf("\n");
}
printf("\n");
*/

for (m = 0; m < length; m++){
    for (n = 0; n < length; n++){
        if (map[m][n] == 2){
            if (m != 0 && map[m-1][n]==1){
                map[m-1][n] = 3;
            }
            if (m != length-1 && map[m+1][n]==1){
                map[m+1][n] = 3;
            }
            if (n != 0 && map[m][n-1]==1){
                map[m][n-1] = 3;
            }
            if (n != length-1 && map[m][n + 1]==1){
                map[m][n+1] = 3;
            }
        }
    }
}

/*
for (m=0; m<length; m++){
for (n=0; n<length; n++){
printf("%d,",map[m][n]);
}
printf("\n");
}
printf("\n");
*/

turns++;
has_fire = 0;
for (i=0; i<length; i++){
    for (j=0; j<length; j++){
        if (map[i][j] == 2){
            map[i][j] = 0;
        }
    }
}

for (i=0; i<length; i++){
    for (j=0; j<length; j++){
        if (map[i][j] == 3){
            has_fire = 1;
            map[i][j] = 2;
        }
    }
}
}
return turns;
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

// one method for everything to run
void run(int seed) {
    char inp[100];
    int den, dim,repeat;
    while (1) {
        printf(CYN "\n______________________________________________________\n\n" RESET);
        printf("What do you want to do? (please just type the number)\n1. calculate\n2. fork\n3. server setup\n");
        fgets(inp, 100, stdin);
        *strchr(inp, '\n') = 0;

        // exiting
        if (strcmp(inp, "exit") == 0) {
            exit(0);
        } else if (strcmp(inp, "1") == 0) { // calculating 1 density with specified dimensions
            while (1) {
                // get user input
                printf("\nDesired density: \n");
                fgets(inp, 100, stdin);
                *strchr(inp, '\n') = 0;
                if (isNum(inp)) {
                    den = atoi(inp);
                    if (den > 100 || den < 0) {
                        printf(RED "\nInvalid input. Please enter a density between 0 and 100.\n" RESET);
                    } else {
                        break;
                    }
                } else {
                    printf(RED "\nInvalid input. Please just enter the number of the choice you wish to make.\n" RESET);
                }
            }

            while (1) {
                printf("\nDesired dimensions: \n");
                fgets(inp, 100, stdin);
                *strchr(inp, '\n') = 0;
                if (isNum(inp)) {
                    dim = atoi(inp);
                    break;
                } else {
                    printf(RED "\nInvalid input. Please just enter numbers.\n" RESET);
                }
            }
            printf(YEL "\nYou entered: den -> %d, dim -> %d\n" RESET, den, dim);

            int method_num = 0;
            while (1) {
                printf("\nCalculation Method (Enter number):\n1.Non_frontier\n2.Frontier\n");
                fgets(inp, 100, stdin);
                *strchr(inp, '\n') = 0;
                if (isNum(inp)) {
                    method_num = atoi(inp);
                    if (method_num != 1 && method_num != 2) {
                        printf(RED "\nInvalid input. Please choose either 1 or 2.\n" RESET);
                    } else {
                        break;
                    }
                } else {
                    printf(RED "\nInvalid input. Please just enter the number of the choice you wish to make.\n" RESET);
                }
            }

            if (method_num == 1){
                printf("\n# of turns(non_frontier): " GRN "%d\n" RESET, non_frontier(dim, den));
            }
            else if (method_num == 2){
                printf("\n# of turns(frontier): " GRN "%d\n" RESET, calculate(dim, den));
            }
        } else if (strcmp(inp, "2") == 0) { // using forking to run multiple times
            while (1) {
                // get user input
                printf("\nDesired density: \n");
                fgets(inp, 100, stdin);
                *strchr(inp, '\n') = 0;
                if (isNum(inp)) {
                    den = atoi(inp);
                    if (den > 100 || den < 0) {
                        printf(RED "\nInvalid input. Please enter a density between 0 and 100.\n" RESET);
                    } else {
                        break;
                    }
                } else {
                    printf(RED "\nInvalid input. Please just enter numbers.\n" RESET);
                }
            }

            while (1) {
                printf("\nDesired dimensions: \n");
                fgets(inp, 100, stdin);
                *strchr(inp, '\n') = 0;
                if (isNum(inp)) {
                    dim = atoi(inp);
                    break;
                } else {
                    printf(RED "\nInvalid input. Please just enter numbers.\n" RESET);
                }
            }
            printf(YEL "\nYou entered: den -> %d, dim -> %d\n" RESET, den, dim);

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

                    seed = time(NULL);
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
            } printf("\nAveraged Results: " GRN "%d\n" RESET, averaged / num_cores);
        } else if (strcmp(inp,"3") == 0) { // using clients and server
            int max_clients = 0;
            int current_clients = 0;
            while (1) {
                printf("\nHow many clients do you want connected: \n");
                fgets(inp, 100, stdin);
                *strchr(inp, '\n') = 0;
                if (isNum(inp)) {
                    max_clients = atoi(inp);
                    if (max_clients > 30 || max_clients < 1) {
                        printf(RED "\nInvalid input. Please enter a number between 1 and 30.\n" RESET);
                    } else {
                        break;
                    }
                } else {
                    printf(RED "\nInvalid input. Please just enter numbers.\n" RESET);
                }
            }

            while (1){
                printf("Dimensions: \n");
                fgets(inp, 100, stdin);
                *strchr(inp, '\n') = 0;
                if (isNum(inp)) {
                    dim = atoi(inp);
                    if (dim > 20000 || max_clients < 1) {
                        printf(RED "\nInvalid input. Please enter a number between 1 and 20000.\n" RESET);
                    } else {
                        break;
                    }
                } else {
                    printf(RED "\nInvalid input. Please just enter numbers.\n" RESET);
                }
            }

            int shmid = shmget(0xC00FFEE,200*sizeof(int), 0644| IPC_CREAT);
            int shmid2 = shmget(0xDEADBEEF,2*sizeof(int), 0644| IPC_CREAT);
            int * data = (int*)shmat(shmid, 0, 0);
            int * data2 = (int*)shmat(shmid2, 0, 0);
            *data2= 1;

            int i;
            for (i=0; i<100; i++){
                data[i] = 0;
            }
            int listen_socket;
            int f;
            listen_socket = server_setup();

            while (1) {
                if (current_clients < max_clients) {
                    int client_socket = server_connect(listen_socket);
                    current_clients++;
                    f = fork();
                    if (f == 0)
                    subserver(client_socket, shmid, shmid2, dim);
                    else
                    close(client_socket);
                }
                else if (*data2 == 1){
                    *data2 = 0;
                }
            }
        }
    }
}

void subserver(int client_socket, int shmid, int shmid2, int dim) {
    char buffer[BUFFER_SIZE];
    int wait = 1;

    int * data = (int*)shmat(shmid,0,0);
    int * data2 = (int*)shmat(shmid2,0,0);
    while (1) {
        if (*data2){
            sprintf(buffer, "%d", wait);
        }
        else {
            int i;
            int den = 0;
            for (i=0; i<100; i++){
                if (data[i] == 0){
                    den = i+99;
                    break;
                }
            }
            sprintf(buffer, "%d,%d", dim,den);
        }
        write(client_socket, buffer, sizeof(buffer));
        read(client_socket, buffer, sizeof(buffer));

        printf("received: [%s]. data2: [%d]\n", buffer, *data2);
    }

    close(client_socket);
    exit(0);
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

/*
void subserver(int client_socket, int shmid, int dim, int repeat) {
char buffer[BUFFER_SIZE];
char * cool_buffer = malloc(BUFFER_SIZE);

int * final_data = shmat(shmid, 0, 0);

while (read(client_socket, buffer, sizeof(buffer))) {
//return percentage and avg turns
strcpy(cool_buffer,buffer);
char * percentage_string = cool_buffer;
strsep(&cool_buffer," ");
char * output_string = cool_buffer;

int percentage = atoi(percentage_string);
int output = atoi(output_string);

final_data[percentage] = output;

int next_percentage = 0;
int i;
for (i = 0; i<100; i++){
if (final_data[i] == -1){
next_percentage = i+1;
break;
}
}

if (next_percentage == 0){
break;
}

char sending_data[100];
sprintf(sending_data,"%d %d %d", dim, next_percentage, repeat);
write(client_socket, sending_data, sizeof(sending_data));
}//end read loop
close(client_socket);
exit(0);
}
*/


int main() {

    int seed = time(NULL);
    //srand(1446978541);
    srand(seed);
    printf("seed: %d\n", seed);
    // printf("# of turns: %d\n",calculate(30,65));

    // char *temp = "\t";
    // printf("%d\n", isNum(temp));
    run(seed);

    return 0;
}

// #include "networking.h"
//
// void process(char *s);
// void subserver(int from_client);
//
// int main() {
//
//     int listen_socket;
//     int f;
//     listen_socket = server_setup();
//
//     while (1) {
//
//         int client_socket = server_connect(listen_socket);
//         f = fork();
//         if (f == 0)
//         subserver(client_socket);
//         else
//         close(client_socket);
//     }
// }
//
// void subserver(int client_socket) {
//     char buffer[BUFFER_SIZE];
//     int sum = 0;
//     int count = 0;
//
//     while (1) {
//         // printf("enter data: ");
//         // fgets(buffer, sizeof(buffer), stdin);
//         // *strchr(buffer, '\n') = 0;
//         sprintf(buffer, "%d", count);
//         // count++;
//         write(client_socket, buffer, sizeof(buffer));
//         read(client_socket, buffer, sizeof(buffer));
//         sum += atoi(buffer);
//         printf("received: [%s]. current sum: [%d]\n", buffer, sum);
//     }
//
//     close(client_socket);
//     exit(0);
// }
//
// void process(char * s) {
//     while (*s) {
//         if (*s >= 'a' && *s <= 'z')
//         *s = ((*s - 'a') + 13) % 26 + 'a';
//         else  if (*s >= 'A' && *s <= 'Z')
//         *s = ((*s - 'a') + 13) % 26 + 'a';
//         s++;
//     }
// }
