#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int calculate(int length, int density) {
    int map[length][length];
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

    int * fire_locations = (int *)malloc(length*length*3);
    int num_fire = 1;
    int time_count = 0;
    fire_locations[0] = length / 2;
    fire_locations[1] = length / 2;
    map[fire_locations[0]][fire_locations[1]] = 2;

    // Test print of the entire map
    for (i = 0; i < length; i++) {
        for (j = 0; j < length; j++) {
            if (map[i][j] == 0){
                // printf("0");
            }
            else{
                // printf("%d",map[i][j]);
            }
        }
        // printf("\n");
    }
    // printf("\n");

    while (num_fire) {
        int * temp_locations = (int *)malloc(length*length*3);
        temp_locations[0] = '\0';
        int num_temp = 0;
        int k;
        // int m = 0;
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
            if (row != length && map[row+1][col]==1){
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
            if (row != length && map[row][col + 1]==1){
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

        int l,n;
        for (l = 0; l < length; l++) {
            for (n = 0; n < length; n++) {
                if (map[l][n] == 0){
                    // printf("0");
                }
                else{
                    // printf("%d",map[l][n]);
                }
            }
            // printf("\n");
        }
        // printf("\n");

    }
    printf("legend:\n0: dirt/empty block\n1: tree\n2: fire\n");
    return time_count;
}

int isNum(char *inp) {
    int i, ret;
    for (int i = 0; i < strlen(inp); i++) {
        if ()
    }
}

// one method for everything to run
void run() {
    char inp[100];
    int den, dim, child_pid, status;
    int *res = calloc(1, sizeof(int));
    int i;
    while (1) {
        printf("\n<____________________________________________________>\n\n");
        printf("What do you want to do? (please just type the number)\n1. calculate\n2. fork calculate (will summarize results)\n");
        fgets(inp, 100, stdin);
        *strchr(inp, '\n') = 0;

        // exiting
        if (strcmp(inp, "exit") == 0) {
            exit(0);
        } else if (strcmp(inp, "1") == 0) { // calculating 1 density with specified dimensions
            printf("Desired density: \n");
            fgets(inp, 100, stdin);
            *strchr(inp, '\n') = 0;
            den = atoi(inp);
            printf("Desired dimensions: \n");
            fgets(inp, 100, stdin);
            *strchr(inp, '\n') = 0;
            dim = atoi(inp);
            printf("You entered: den -> %d, dim -> %d\n", den, dim);

            printf("# of turns: %d\n", calculate(dim, den));
        } else if (strcmp(inp, "2") == 0) { // using forking to run multiple times
            // get user input
            printf("Desired density: \n");
            fgets(inp, 100, stdin);
            *strchr(inp, '\n') = 0;
            den = atoi(inp);
            if (! den) {
                printf("Invalid input. Please just enter the number of the choice you wish to make.\n");

            }
            printf("Desired dimensions: \n");
            fgets(inp, 100, stdin);
            *strchr(inp, '\n') = 0;
            dim = atoi(inp);
            printf("You entered: den -> %d, dim -> %d\n", den, dim);
            int hi[2];
            // forking twice
            for (i = 0; i < 2; i++) {
                child_pid = fork();
                if (child_pid == 0) { // child
                    // calculate
                    // int temp = calculate(den,dim);
                    // *res = calculate(dim, den);
                    // sleep(2);
                    int seed = time(NULL);
                    srand(seed + i);
                    // int e = calculate(dim,den);
                    printf("current result: %d\n", calculate(dim, den));
                    // printf("result1: %d\n", *res);
                    // printf("result2: %d\n", *(res + 1));
                    // res += temp;
                    exit(0);
                } else { // parent
                    waitpid(child_pid, &status, 0);
                    int error = WEXITSTATUS(status);
                    if (error) {
                        printf("Error: %s\n", strerror(errno));
                    } else {
                        printf("successfully executed\n");
                    }
                }
            }
            // for (i = 0; i < 2; i++) {
            //     int temp = calculate(dim, den);
            //     printf("val: %d\n", temp);
            //     sleep(2);
            // }
            printf("result1: %d\n", *res);
            free(res);
        }

    }
}

int main() {
    int seed = time(NULL);
    // srand(1546978500);
    srand(seed);
    printf("seed: %d\n",seed);
    // printf("# of turns: %d\n",calculate(30,65));

    run();

    return 0;
}
