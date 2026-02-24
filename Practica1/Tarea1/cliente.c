#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>   // biblioteca de sockets para Windows (Winsock2)
#pragma comment(lib, "ws2_32.lib") // enlaza automáticamente la librería ws2_32 al compilar con MSVC o MinGW

#define PUERTO   5000   // debe coincidir con el puerto que usa el servidor Java
#define IP       "127.0.0.1" // dirección del servidor (localhost = misma máquina)
#define TAM_BUF  256    // tamaño máximo del buffer para mensajes de texto

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData); // inicializa la librería Winsock; obligatorio antes de usar cualquier función de socket en Windows

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); // crea un socket TCP (AF_INET = IPv4, SOCK_STREAM = orientado a conexión)
    if (sock == INVALID_SOCKET) {
        printf("Error al crear el socket.\n");
        WSACleanup();
        return 1;
    }

    struct sockaddr_in servidor; // estructura que contiene la dirección y puerto del servidor al que nos conectaremos
    servidor.sin_family      = AF_INET;        // familia de direcciones IPv4
    servidor.sin_port        = htons(PUERTO);  // convierte el puerto a orden de bytes de red (big-endian)
    servidor.sin_addr.s_addr = inet_addr(IP);  // convierte la cadena IP a formato binario de red

    if (connect(sock, (struct sockaddr*)&servidor, sizeof(servidor)) == SOCKET_ERROR) { // establece la conexión TCP con el servidor
        printf("No se pudo conectar al servidor.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    printf("Conectado al servidor.\n");

    // --- ENVÍO DEL MENSAJE AL SERVIDOR ---
    char mensaje[] = "Hola\n"; // el '\n' es importante: Java usa readLine() que espera un salto de línea como delimitador
    send(sock, mensaje, strlen(mensaje), 0); // envía los bytes del mensaje por el socket al servidor
    printf("Mensaje enviado: %s", mensaje);

    // --- RECEPCIÓN DE LA RESPUESTA DEL SERVIDOR ---
    char buffer[TAM_BUF];
    memset(buffer, 0, TAM_BUF); // limpia el buffer antes de recibir para evitar basura residual en memoria
    recv(sock, buffer, TAM_BUF - 1, 0); // recibe los bytes enviados por el servidor Java (bloqueante)
    printf("Respuesta del servidor: %s\n", buffer);

    // cierre ordenado del socket y liberación de Winsock
    closesocket(sock);
    WSACleanup(); // libera todos los recursos internos que Winsock reservó con WSAStartup
    return 0;
}