#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 4096

int recv_all(int socket_fd, char *buffer, size_t length) {

    size_t total_received = 0;

    while (total_received < length) {

        ssize_t received = recv(
            socket_fd,
            buffer + total_received,
            length - total_received,
            0
        );

        if (received <= 0) {
            return -1;
        }

        total_received += received;
    }

    return total_received;
}
int main() {

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (client_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    if (connect(client_fd,
                (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0) {
        perror("connect");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    char *request =
    "GET /big.txt CHLP/1.0\n"
    "Body-Size: 0\n"
    "\n";

    ssize_t bytes_sent = send(
        client_fd,
        request,
        strlen(request),
        0
    );

    if (bytes_sent < 0) {
        perror("send");
        close(client_fd);
        exit(EXIT_FAILURE);
    }
    char buffer[BUFFER_SIZE];

        ssize_t bytes_received = recv(
            client_fd,
            buffer,
            BUFFER_SIZE - 1,
            0
        );

        if (bytes_received < 0) {
            perror("recv");
            close(client_fd);
            exit(EXIT_FAILURE);
        }

        if (bytes_received == 0) {
            printf("Server closed connection\n");
            close(client_fd);
            return 0;
        }

        buffer[bytes_received] = '\0';

        char *body = strstr(buffer, "\n\n");

        if (body == NULL) {
            printf("Invalid response format\n");
            close(client_fd);
            return 1;
        }

        size_t body_size = 0;

        char *body_size_header = strstr(buffer, "Body-Size:");

        if (body_size_header == NULL ||
            sscanf(body_size_header, "Body-Size: %zu", &body_size) != 1) {

            printf("Invalid Body-Size header\n");
            close(client_fd);
            return 1;
        }

        body += 2;

        size_t header_size = body - buffer;
        size_t body_received = bytes_received - header_size;

        if (body_received > body_size) {
            body_received = body_size;
        }

        printf("Response from server:\n");

        printf("%.*s", (int)header_size, buffer);

        fwrite(
            body,
            1,
            body_received,
            stdout
        );

        char chunk[BUFFER_SIZE];

        while (body_received < body_size) {

            size_t remaining = body_size - body_received;

            size_t chunk_size =
                remaining < BUFFER_SIZE
                ? remaining
                : BUFFER_SIZE;

            ssize_t received = recv_all(
                client_fd,
                chunk,
                chunk_size
            );

            if (received < 0) {
                printf("\nFailed to receive complete body\n");
                break;
            }

            fwrite(
                chunk,
                1,
                received,
                stdout
            );

            body_received += received;
        }

        printf("\n");

        while (body_received < body_size) {

            ssize_t n = recv(
                client_fd,
                buffer + bytes_received,
                BUFFER_SIZE - 1 - bytes_received,
                0
            );

            if (n < 0) {
                perror("recv");
                close(client_fd);
                return 1;
            }

            if (n == 0) {
                break;
            }

            bytes_received += n;
            body_received += n;
        }

        buffer[bytes_received] = '\0';

        printf("Response from server:\n");

        printf("%.*s",
            (int)header_size,
            buffer);

        printf("%.*s\n",
            (int)body_size,
            body);

    close (client_fd);

    return 0;
}