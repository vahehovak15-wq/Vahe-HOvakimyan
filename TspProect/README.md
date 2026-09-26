# CHLP Client/Server

Simple TCP client/server project written in C using the custom `CHLP/1.0` protocol.

## Features

- TCP sockets
- Multi-client server using POSIX threads
- `GET`, `POST`, and `ECHO` methods
- `Body-Size` header
- `200 OK`, `400 Bad Request`, `404 Not Found`, and `500 Internal Server Error`
- Large GET responses are sent in chunks
- Basic `../` path traversal protection

## Build

From the project directory:

```bash
make
```

This creates:

```text
server
client
```

To remove compiled binaries:

```bash
make clean
```

## Run

Start the server in one terminal:

```bash
./server
```

Use the client from another terminal.

### GET

Files requested with GET must be inside `server_files/`.

```bash
./client GET /big.txt
```

### POST

```bash
./client POST /test "Hello from POST"
```

### ECHO

```bash
./client ECHO /echo "Hello World"
```

The ECHO response contains the same body sent by the client.

## Protocol examples

GET:

```text
GET /big.txt CHLP/1.0
Body-Size: 0

```

POST:

```text
POST /test CHLP/1.0
Body-Size: 5

Hello
```

ECHO:

```text
ECHO /echo CHLP/1.0
Body-Size: 5

Hello
```

## Project structure

```text
TspProect/
├── client.c
├── server.c
├── Makefile
├── README.md
└── server_files/
    └── big.txt
```
