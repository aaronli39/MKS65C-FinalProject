#include "networking.h"


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
    }

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
    }
    return averaged / num_cores;
}

int main(int argc, char **argv) {

    int server_socket;
    char buffer[BUFFER_SIZE];

    if (argc == 2)
    server_socket = client_setup( argv[1]);
    else
    server_socket = client_setup( TEST_IP );

    while (read(server_socket, buffer, sizeof(buffer))) {
        int temp = atoi(buffer);
        if (temp == 1){
            sleep(1);
            sprintf(buffer, "%d", temp);
        }
        else {
            int trials = 10;
            int i = 0;
            int sum = 0;
            //printf("[client] received: [%s]\n", buffer);

            // change buffer into dim/den
            char str[1024];
            strcpy(str, buffer);
            const char delim[2] = ",";
            char *token;
            int *arr = calloc(2, sizeof(int));
            /* get the first token */
            token = strtok(str, delim);

            /* walk through other tokens */
            while(token != NULL ) {
                //printf("%s\n", token );
                arr[i] = atoi(token);
                i++;
                token = strtok(NULL, delim);
            }
            int dim = arr[0];
            int den = arr[1];
            printf("Density: %d%%\n",den);
            //printf("Calculating burn time for a %d by %d forest with %d%% density.",dim,dim,den);
            free(arr);
            //calculate here
            for (i = 0; i < trials; i++) {
                sum += meanCalc(dim, den);
            } sum = sum / trials;
            sprintf(buffer, "%d,%d", den, sum);
            //printf("[client] sent: [%s]. avg was: [%d] for density [%d]\n", buffer, sum, den);
        }
        write(server_socket, buffer, sizeof(buffer));

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
