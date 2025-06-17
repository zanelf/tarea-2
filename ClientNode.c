#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_TAM_MENSAJE 512

char mensaje[MAX_TAM_MENSAJE];
char usuario[50];

void catch(int sig) {
    printf("\n[!] Terminando cliente...\n");
    exit(0);
}

int main() {
    int descriptor_socket;
    struct sockaddr_in servidor;
    int IP;
    int puerto;

    signal(SIGINT, catch);

    //datos usuari

    printf("Ingrese puerto del servidor: ");
    scanf("%d", &puerto);

    printf("Ingrese nombre de usuario: ");
    scanf("%s", usuario);
    getchar();

    IP = htonl(INADDR_ANY);



    //crea socket
    descriptor_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (descriptor_socket == -1) {
        perror("Error al crear socket");
        return 1;
    }

    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(puerto);
    servidor.sin_addr.s_addr = inet_addr(IP);

    //conectar serv
    if (connect(descriptor_socket, (struct sockaddr*)&servidor, sizeof(servidor)) < 0) {
        perror("Error al conectar");
        return 1;
    }

    printf("\nConectado como '%s'. Puede comenzar a escribir mensajes.\n", usuario);

    //bucle de mensajes
    while (1) {
        printf(">> ");
        fgets(mensaje, MAX_TAM_MENSAJE, stdin);
        mensaje[strcspn(mensaje, "\n")] = '\0';

        if (strlen(mensaje) > 0) {
            char mensaje_final[MAX_TAM_MENSAJE + 50];
            snprintf(mensaje_final, sizeof(mensaje_final), "%s: %s", usuario, mensaje);
            send(descriptor_socket, mensaje_final, strlen(mensaje_final), 0);
        }
    }

    close(descriptor_socket);
    return 0;
}