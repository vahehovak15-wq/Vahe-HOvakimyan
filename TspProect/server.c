#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>
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

        total_sent += sent;
    }

    return 0;
}

void *handle_client(void *arg) {

    int client_fd = *(int *)arg;
    free(arg);

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
        return NULL;
    }

    if (bytes_received == 0) {
        printf("Client disconnected\n");
        close(client_fd);
        return NULL;
    }

    buffer[bytes_received] = '\0';

    printf("Request received:\n%s\n", buffer);

    char method[16] = {0};
    char resource[256] = {0};
    char version[16] = {0};

    int parsed = sscanf(
        buffer,
        "%15s %255s %15s",
        method,
        resource,
        version
    );

    if (parsed != 3) {
        printf("Invalid request format\n");
        close(client_fd);
        return NULL;
    }

    printf("Method: %s\n", method);
    printf("Resource: %s\n", resource);
    printf("Version: %s\n", version);
    if (strcmp(version, "CHLP/1.0") != 0) {

        char *response =
            "CHLP/1.0 400 Bad Request\n"
            "Body-Size: 0\n"
            "\n";

            send_all(
            client_fd,
            response,
            strlen(response)
        );

        printf("400 Bad Request: invalid version\n");

        close(client_fd);
        return NULL;
    }size_t body_size = 0;

        char *body_size_header = strstr(buffer, "Body-Size:");

        if (body_size_header == NULL) {

            char *response =
                "CHLP/1.0 400 Bad Request\n"
                "Body-Size: 0\n"
                "\n";

            send_all(client_fd, response, strlen(response));

            printf("400 Bad Request: Body-Size header missing\n");

            close(client_fd);
            return NULL;
        }

        if (sscanf(body_size_header, "Body-Size: %zu", &body_size) != 1) {

            char *response =
                "CHLP/1.0 400 Bad Request\n"
                "Body-Size: 0\n"
                "\n";

            send_all(client_fd, response, strlen(response));

            printf("400 Bad Request: invalid Body-Size\n");

            close(client_fd);
            return NULL;
        }

        printf("Body-Size: %zu\n", body_size);
            
        char *body = strstr(buffer, "\n\n");

        if (body != NULL) {

            body += 2;

            size_t header_size = body - buffer;
            if (body_size > BUFFER_SIZE - 1 - header_size) {

                char *response =
                    "CHLP/1.0 400 Bad Request\n"
                    "Body-Size: 0\n"
                    "\n";

            send_all(client_fd, response, strlen(response));

                printf("400 Bad Request: body too large\n");

                close(client_fd);
                return NULL;
            }
            size_t body_received = bytes_received - header_size;

            while (body_received < body_size) {

                ssize_t n = recv(
                    client_fd,
                    buffer + bytes_received,
                    BUFFER_SIZE - 1 - bytes_received,
                    0
                );

                if (n <= 0) {
                    printf("Incomplete request body\n");
                    close(client_fd);
                    return NULL;
                }

                bytes_received += n;
                body_received += n;
            }

            buffer[bytes_received] = '\0';
        }else {

            char *response =
                "CHLP/1.0 400 Bad Request\n"
                "Body-Size: 0\n"
                "\n";

            send_all(client_fd, response, strlen(response));

            printf("400 Bad Request: invalid header format\n");

            close(client_fd);
            return NULL;
        }

    if (strcmp(method, "GET") == 0) {

        char filepath[512];
        if (strstr(resource, "..") != NULL) {

            char *response =
                "CHLP/1.0 400 Bad Request\n"
                "Body-Size: 0\n"
                "\n";

            send_all(client_fd, response, strlen(response));

            printf("400 Bad Request: invalid path\n");

            close(client_fd);
            return NULL;
        }
        snprintf(
            filepath,
            sizeof(filepath),
            "server_files%s",
            resource
        );FILE *file = fopen(filepath, "rb");

if (file == NULL) {

    if (errno == ENOENT) {

        char *response =
            "CHLP/1.0 404 Not Found\n"
            "Body-Size: 0\n"
            "\n";

        send_all(
            client_fd,
            response,
            strlen(response)
        );

        printf("404 Not Found sent\n");
    }

    else {

        char *response =
            "CHLP/1.0 500 Internal Server Error\n"
            "Body-Size: 0\n"
            "\n";

        send_all(
            client_fd,
            response,
            strlen(response)
        );

        perror("fopen");
        printf("500 Internal Server Error sent\n");
    }

    close(client_fd);
    return NULL;
}

if (fseek(file, 0, SEEK_END) != 0) {

    fclose(file);

    char *response =
        "CHLP/1.0 500 Internal Server Error\n"
        "Body-Size: 0\n"
        "\n";

    send_all(
        client_fd,
        response,
        strlen(response)
    );

    close(client_fd);
    return NULL;
}

long file_size = ftell(file);

if (file_size < 0) {

    fclose(file);

    char *response =
        "CHLP/1.0 500 Internal Server Error\n"
        "Body-Size: 0\n"
        "\n";

    send_all(
        client_fd,
        response,
        strlen(response)
    );

    close(client_fd);
    return NULL;
}

rewind(file);

char response_header[256];

snprintf(
    response_header,
    sizeof(response_header),
    "CHLP/1.0 200 OK\n"
    "Body-Size: %ld\n"
    "\n",
    file_size
);

if (send_all(
        client_fd,
        response_header,
        strlen(response_header)
    ) < 0) {

    perror("send_all");
    fclose(file);
    close(client_fd);
    return NULL;
}

char file_buffer[BUFFER_SIZE];

size_t bytes_read;

while ((bytes_read = fread(
            file_buffer,
            1,
            sizeof(file_buffer),
            file
        )) > 0) {

    if (send_all(
            client_fd,
            file_buffer,
            bytes_read
        ) < 0) {

        perror("send_all");
        break;
    }
}

fclose(file);

printf("GET response sent\n");
}

else if (strcmp(method, "POST") == 0) {

    printf("POST request received\n");

    if (body != NULL) {
        printf(
            "POST body: %.*s\n",
            (int)body_size,
            body
        );
    }

    char *response =
        "CHLP/1.0 200 OK\n"
        "Body-Size: 0\n"
        "\n";

    send_all(
        client_fd,
        response,
        strlen(response)
    );

    printf("POST response sent\n");
}

else if (strcmp(method, "ECHO") == 0) {

    printf("ECHO request received\n");

    if (body == NULL) {

        printf("ECHO body not found\n");

        close(client_fd);
        return NULL;
    }

    char response_header[256];

    snprintf(
        response_header,
        sizeof(response_header),
        "CHLP/1.0 200 OK\n"
        "Body-Size: %zu\n"
        "\n",
        body_size
    );

    if (send_all(
            client_fd,
            response_header,
            strlen(response_header)
        ) < 0) {

        perror("send_all");
        close(client_fd);
        return NULL;
    }

    if (send_all(
            client_fd,
            body,
            body_size
        ) < 0) {

        perror("send_all");
        close(client_fd);
        return NULL;
    }

    printf("ECHO response sent\n");
}

else {

    char *response =
        "CHLP/1.0 400 Bad Request\n"
        "Body-Size: 0\n"
        "\n";

    send_all(
        client_fd,
        response,
        strlen(response)
    );

    printf("400 Bad Request sent\n");
}

close(client_fd);

return NULL;
}
int main() {

    int server_fd;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

   while (1) {

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int *client_fd = malloc(sizeof(int));

    if (client_fd == NULL) {
        perror("malloc");
        continue;
    }

    *client_fd = accept(
        server_fd,
        (struct sockaddr *)&client_addr,
        &client_len
    );

    if (*client_fd < 0) {
        perror("accept");
        free(client_fd);
        continue;
    }

    printf("Client connected\n");

    pthread_t thread;

    if (pthread_create(
            &thread,
            NULL,
            handle_client,
            client_fd
        ) != 0) {

        perror("pthread_create");
        close(*client_fd);
        free(client_fd);
        continue;
    }

    pthread_detach(thread);

   }
    close(server_fd);
    return 0;
}