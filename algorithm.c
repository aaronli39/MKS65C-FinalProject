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

    for (i = 0; i < length; i++) {
        for (j = 0; j < length; j++) {
            printf("%d,",map[i][j]);
        }
        printf("\n");
    }
    return num_trees;
}

int main() {
    srand(time(NULL));
    printf("%d",calculate(10,99));
}
