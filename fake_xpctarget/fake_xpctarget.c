#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char** argv) {
    struct header {
        unsigned short magic_num;
        unsigned short cmd;
        int size;
    } pheader;
    unsigned char* buf = NULL;

    printf("Started\n");

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    printf("Made socket\n");

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(22222);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int ret = bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("Bound to 22222\n");

    ret = listen(sock, 5);

    printf("Listening for connections...\n");

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t len;
        int connection = accept(sock, (struct sockaddr*)&client_addr, &len);

        printf("Got a connection\n");

        if (connection < 0) {
            continue;
        }

        while (1) {
            
            int status = recv(connection, &pheader, 8, 0);
            if (status == 0) {
                printf("Connection Closed\n");
                break;
            }
            if (status != 8) {
                perror("recv failed\n");
                close(connection);
                break;
            }
            unsigned short cmd = ntohs(pheader.cmd);
            printf("Recieved cmd: 0x%X size: %i\n", cmd, pheader.size);

            if (cmd == 0xC88C) {
                int fsize;
                status = recv(connection, &fsize, sizeof(fsize), 0);
                if (status == 0) {
                    printf("Connection Closed\n");
                    break;
                }
                else if (status != 4) {
                    fprintf(stderr, "fisze recv failed %i", status);
                    close(connection);
                    break;
                }

                buf = malloc(fsize);
                unsigned char* buf_write = buf;
                while (fsize > 0) {
                    status = recv(connection, buf_write, fsize, 0);
                    if (status == 0) {
                        printf("Connection Closed\n");
                        break;
                    }
                    if (status < 0) {
                        perror("File recv failed\n");
                    }

                    buf_write += status;
                    fsize -= status;
                    printf("Read %i Remaining %i\n", status, fsize);
                }
                
            }
            else if (pheader.size > 0) {

                buf = malloc(pheader.size);
                status = recv(connection, buf, pheader.size, 0);

                if (status == 0) {
                    printf("Connection Closed\n");
                    break;
                }
                else if (status != pheader.size) {
                    fprintf(stderr, "recv2 failed, got %i\n", status);
                    close(connection);
                    break;
                }
            }



            switch (cmd) {
                case 0xC83C:
                char payload[19];
                memcpy(payload, &pheader, 8);
                int size = 7 + 4;
                memcpy(payload + 4, &size, sizeof(size));
                size = 7;
                memcpy(payload + 8, &size, sizeof(size));
                
                strcpy(&payload[12], "loader");
                
                send(connection, payload, 19, 0);
                break;

                default:
                pheader.size = 0;
                send(connection, &pheader, sizeof(pheader), 0);
                break;
            }

            

            printf("Sent Response\n");

            if (buf) {
                free(buf);
                buf = NULL;
            }
        }


    }



}