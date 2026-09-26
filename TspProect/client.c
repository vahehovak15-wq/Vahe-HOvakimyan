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

int send_all(int socket_fd, const char *data, size_t length) {
    size_t total_sent = 0;

    while (total_sent < length) {
        ssize_t sent = send(
            socket_fd,
            data + total_sent,
            length - total_sent,
            0
        );

        if (sent <= 0) {
            return -1;
        }

        total_sent += (size_t)sent;
    }

    return 0;
}

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

        total_received += (size_t)received;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage:\n");
        printf("  %s GET  /file.txt\n", argv[0]);
        printf("  %s POST /resource \"body\"\n", argv[0]);
        printf("  %s ECHO /echo \"body\"\n", argv[0]);
        return 1;
    }

    const char *method = argv[1];
    const char *resource = argv[2];
    const char *request_body = "";
    size_t request_body_size = 0;

    if (strcmp(method, "GET") == 0) {
        if (argc != 3) {
            printf("GET does not use a body\n");
            return 1;
        }
    } else if (strcmp(method, "POST") == 0 ||
               strcmp(method, "ECHO") == 0) {
        if (argc != 4) {
            printf("%s requires a body\n", method);
            return 1;
        }

        request_body = argv[3];
        request_body_size = strlen(request_body);
    } else {
        printf("Supported methods: GET, POST, ECHO\n");
        return 1;
    }

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (client_fd < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(client_fd);
        return 1;
    }

    if (connect(
            client_fd,
            (struct sockaddr *)&server_addr,
            sizeof(server_addr)
        ) < 0) {
        perror("connect");
        close(client_fd);
        return 1;
    }

    printf("Connected to server\n");

    char request_header[BUFFER_SIZE];

    int header_length = snprintf(
        request_header,
        sizeof(request_header),
        "%s %s CHLP/1.0\n"
        "Body-Size: %zu\n"
        "\n",
        method,
        resource,
        request_body_size
    );

    if (header_length < 0 ||
        (size_t)header_length >= sizeof(request_header)) {
        printf("Request header is too large\n");
        close(client_fd);
        return 1;
    }

    if (send_all(
            client_fd,
            request_header,
            (size_t)header_length
        ) < 0) {
        perror("send_all");
        close(client_fd);
        return 1;
    }

    if (request_body_size > 0) {
        if (send_all(
                client_fd,
                request_body,
                request_body_size
            ) < 0) {
            perror("send_all");
            close(client_fd);
            return 1;
        }
    }

    char buffer[BUFFER_SIZE];
    size_t bytes_received = 0;
    char *body = NULL;

    while (body == NULL) {
        if (bytes_received >= BUFFER_SIZE - 1) {
            printf("Response header is too large\n");
            close(client_fd);
            return 1;
        }

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
            printf("Server closed connection before complete response header\n");
            close(client_fd);
            return 1;
        }

        bytes_received += (size_t)n;
        buffer[bytes_received] = '\0';

        body = strstr(buffer, "\n\n");
    }

    body += 2;

    size_t header_size = (size_t)(body - buffer);
    size_t body_size = 0;

    char *body_size_header = strstr(buffer, "Body-Size:");

    if (body_size_header == NULL ||
        sscanf(body_size_header, "Body-Size: %zu", &body_size) != 1) {
        printf("Invalid Body-Size header\n");
        close(client_fd);
        return 1;
    }

    size_t body_received = bytes_received - header_size;

    if (body_received > body_size) {
        body_received = body_size;
    }

    printf("Response from server:\n");
    fwrite(buffer, 1, header_size, stdout);

    if (body_received > 0) {
        fwrite(body, 1, body_received, stdout);
    }

    char chunk[BUFFER_SIZE];

    while (body_received < body_size) {
        size_t remaining = body_size - body_received;
        size_t chunk_size =
            remaining < sizeof(chunk) ? remaining : sizeof(chunk);

        if (recv_all(client_fd, chunk, chunk_size) < 0) {
            printf("\nFailed to receive complete response body\n");
            close(client_fd);
            return 1;
        }

        fwrite(chunk, 1, chunk_size, stdout);
        body_received += chunk_size;
    }

    printf("\n");

    close(client_fd);
    return 0;
}
