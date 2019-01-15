#include "networking.h"

int main(int argc, char **argv) {

    int server_socket;
    char buffer[BUFFER_SIZE];
    char * cool_buffer = malloc(BUFFER_SIZE);

    if (argc == 2)
    server_socket = client_setup( argv[1]);
    else
    server_socket = client_setup( TEST_IP );
    
    while (read(server_socket, buffer, sizeof(buffer))) {
        printf("here i am");
        strcpy(cool_buffer,buffer);
        printf("%s",cool_buffer);
    }
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
