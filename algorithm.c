#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
<<<<<<< HEAD
=======
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
>>>>>>> 2b9c55ea46f5e24db1f1b47959b6f53780821a44

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
        for (m = 0; m < length; m++){
            for (n = 0; n < length; n++){
                if (map[m][n] == 2){
                    if (m == 0 && map[m-1][n]==1){
                        map[m-1][n] = 3;
                    }
                    if (m != length && map[m+1][n]==1){
                        map[m+1][n] = 3;
                    }
                    if (n != 0 && map[m][n-1]==1){
                        map[m][n-1] = 3;
                    }
                    if (m != length && map[m][n + 1]==1){
                        map[m][n+1] = 3;
                    }
                }
            }
        }

        for (m=0; m<length; m++){
            for (n=0; n<length; n++){
                printf("%d,",map[m][n]);
            }
            printf("\n");
        }
        printf("\n");

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
                printf("0");
            }
            else{
                printf("%d",map[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");

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
                    printf("0");
                }
                else{
                    printf("%d",map[l][n]);
                }
            }
            printf("\n");
        }
        printf("\n");

    }
    // printf("legend:\n0: dirt/empty block\n1: tree\n2: fire\n\n");
    return time_count;
}

<<<<<<< HEAD
=======
// check if a string has only numbers
// 0: not solely digits(tabs/newlines are not digits)
// 1: solely digits
int isNum(char *inp) {
    int i, ret;
    for (int i = 0; i < strlen(inp); i++) {
        if (! isdigit(inp[i])) {
            return 0;
        }
    } return 1;
}

>>>>>>> 2b9c55ea46f5e24db1f1b47959b6f53780821a44
// one method for everything to run
void run() {
    char inp[100];
    int den, dim;
    while (1) {
        printf("\n<____________________________________________________>\n\n");
        printf("What do you want to do? (please just type the number)\n1. calculate\n2. fork\n");
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
                    break;
                } else {
                    printf("Invalid input. Please just enter the number of the choice you wish to make.\n");
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
                    printf("Invalid input. Please just enter the number of the choice you wish to make.\n");
                }
            }
            printf("You entered: den -> %d, dim -> %d\n", den, dim);

            printf("# of turns(non_frontier): %d\n", non_frontier(dim, den));
            printf("# of turns(frontier): %d\n", calculate(dim, den));
        } else if (strcmp(inp, "2") == 0) { // using forking to run multiple times
<<<<<<< HEAD

=======
            while (1) {
                // get user input
                printf("\nDesired density: \n");
                fgets(inp, 100, stdin);
                *strchr(inp, '\n') = 0;
                if (isNum(inp)) {
                    den = atoi(inp);
                    break;
                } else {
                    printf("Invalid input. Please just enter the number of the choice you wish to make.\n");
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
                    printf("Invalid input. Please just enter the number of the choice you wish to make.\n");
                }
            }
            printf("You entered: den -> %d, dim -> %d\n", den, dim);

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
            printf("result1: %d\n", *res);
            free(res);
>>>>>>> 2b9c55ea46f5e24db1f1b47959b6f53780821a44
        }
        else if (strcmp(inp,"3") == 0) { //using clients and server
            int max_clients = 0;
            printf("How many other computers do you want to do calcuations? (1 to 30): \n");
            
        }
    }
}

int main() {
    int seed = time(NULL);
    // srand(1546978500);
    srand(seed);
    printf("seed: %d\n",seed);
    // printf("# of turns: %d\n",calculate(30,65));

    // char *temp = "\t";
    // printf("%d\n", isNum(temp));
    run();

    return 0;
}
