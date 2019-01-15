#include "networking.h"

void process(char * s) {
    while (*s) {
        if (*s >= 'a' && *s <= 'z')
        *s = ((*s - 'a') + 13) % 26 + 'a';
        else  if (*s >= 'A' && *s <= 'Z')
        *s = ((*s - 'a') + 13) % 26 + 'a';
        s++;
    }
}

int main(int argc, char **argv) {

    int server_socket;
    char buffer[BUFFER_SIZE];

    if (argc == 2)
    server_socket = client_setup( argv[1]);
    else
    server_socket = client_setup( TEST_IP );

    while (read(server_socket, buffer, sizeof(buffer))) {

        printf("[client] received: [%s]\n", buffer);
        // process(buffer);
        int temp = atoi(buffer);
        temp += 1;
        sleep(1);
        sprintf(buffer, "%d", temp);
        write(server_socket, buffer, sizeof(buffer));
        printf("[client] sent: [%s]. temp was: [%d]\n", buffer, temp);
    }//end read loop

}
