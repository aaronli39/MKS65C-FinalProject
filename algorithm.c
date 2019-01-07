#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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
            printf("%d,",map[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    while (num_fire) {
        int * temp_locations = (int *)malloc(length*length*3);
        temp_locations[0] = '\0';
        int num_temp = 0;
        printf("start fire\n");
        for (i = 0; i < num_fire; i++){
            int row = fire_locations[2*i];
            int col = fire_locations[2*i + 1];
            map[row][col] = 0;
            printf("start fire 1\n");
            if (row != 0 && map[row-1][col]==1){
                temp_locations[2*num_temp] = row - 1;
                temp_locations[2*num_temp + 1] = col;
                map[row-1][col] = 2;
                num_temp++;
            }
            printf("start fire 2\n");
            if (row != length && map[row+1][col]==1){
                temp_locations[2*num_temp] = row + 1;
                temp_locations[2*num_temp + 1] = col;
                map[row+1][col] = 2;
                num_temp++;
            }
            printf("start fire 3\n");
            if (col != 0 && map[row][col-1]==1){
                temp_locations[2*num_temp] = row;
                temp_locations[2*num_temp + 1] = col - 1;
                map[row][col-1] = 2;
                num_temp++;
            }
            printf("start fire 4\n");
            if (row != length && map[row][col + 1]==1){
                temp_locations[2*num_temp] = row;
                temp_locations[2*num_temp + 1] = col + 1;
                map[row][col+1] = 2;
                num_temp++;
            }
        }
        printf("start fire 5\n");
        fire_locations = temp_locations;
        num_fire = num_temp;
        printf("%d\n",num_fire);
        time_count++;

        for (i = 0; i < length; i++) {
            for (j = 0; j < length; j++) {
                if (map[i][j] == 0){
                    printf(" ,");
                }
                else{
                    printf("%d,",map[i][j]);
                }

            }
            printf("\n");
        }

        printf("\n");
    }

    return time_count;
}

int main() {
    //int seed = time(NULL);
    srand(1546892972);
    //printf("%d\n",seed);
    printf("%d\n",calculate(30,65));
}
