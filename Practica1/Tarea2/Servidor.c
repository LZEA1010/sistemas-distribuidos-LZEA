#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>  // biblioteca de sockets para Windows
#pragma comment(lib, "ws2_32.lib") // enlaza ws2_32 automáticamente al compilar

#define PUERTO  5000
#define TAM_BUF 256

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData); // inicializa Winsock antes de usar cualquier función de socket

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0); // crea socket TCP servidor
    if (serverSocket == INVALID_SOCKET) {
        printf("Error al crear el socket.\n");
        WSACleanup();
        return 1;
    }

    struct sockaddr_in servidor;
    servidor.sin_family      = AF_INET;         // familia IPv4
    servidor.sin_port        = htons(PUERTO);   // puerto en orden de bytes de red (big-endian)
    servidor.sin_addr.s_addr = INADDR_ANY;      // acepta conexiones en cualquier interfaz de red de la máquina

    if (bind(serverSocket, (struct sockaddr*)&servidor, sizeof(servidor)) == SOCKET_ERROR) { // asocia el socket al puerto indicado
        printf("Error en bind.\n");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    listen(serverSocket, 1); // pone el socket en modo escucha; el "1" es el máximo de conexiones en cola
    printf("Servidor C esperando conexion en el puerto %d...\n", PUERTO);

    struct sockaddr_in clienteAddr;
    int clienteLen = sizeof(clienteAddr);
    SOCKET clienteSocket = accept(serverSocket, (struct sockaddr*)&clienteAddr, &clienteLen); // bloquea hasta que un cliente se conecta
    printf("Cliente conectado.\n");

    char buffer[TAM_BUF];

    while (1) {
        memset(buffer, 0, TAM_BUF); // limpia el buffer antes de cada recepción para evitar datos residuales

        int bytesRecibidos = recv(clienteSocket, buffer, TAM_BUF - 1, 0); // recibe el número enviado por el cliente Java como texto
        if (bytesRecibidos <= 0) break; // si el cliente cerró la conexión o hubo error, sale del bucle

        int numero = atoi(buffer); // convierte la cadena recibida a entero (ej: "5" -> 5)
        printf("Numero recibido del cliente: %d\n", numero);

        if (numero == 0) { // condición de salida: el cliente envió 0
            printf("El cliente envio 0. Cerrando conexion.\n");
            send(clienteSocket, "0\n", 2, 0); // confirma al cliente que también termina
            break;
        }

        int respuesta = numero + 1; // incrementa el número recibido en 1
        sprintf(buffer, "%d\n", respuesta); // convierte el entero a cadena con '\n' como delimitador para Java
        send(clienteSocket, buffer, strlen(buffer), 0); // envía el número incrementado de vuelta al cliente
        printf("Respuesta enviada: %d\n", respuesta);
    }

    // cierre ordenado de sockets y liberación de Winsock
    closesocket(clienteSocket);
    closesocket(serverSocket);
    WSACleanup();
    printf("Servidor cerrado.\n");
    return 0;
}