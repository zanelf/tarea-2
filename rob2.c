#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_MSG 500

int main() {
    int sockfd;
    struct sockaddr_in servidor;
    char buffer[MAX_MSG];
    socklen_t addr_size = sizeof(servidor);

    char usuario[50];
    char ip_servidor[20];
    int puerto;

    printf("Ingrese IP del servidor: ");
    scanf("%s", ip_servidor);
    printf("Ingrese puerto del servidor: ");
    scanf("%d", &puerto);
    printf("Ingrese nombre de usuario: ");
    scanf("%s", usuario);

    // Crear socket UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error creando socket");
        exit(EXIT_FAILURE);
    }

    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(puerto);
    servidor.sin_addr.s_addr = inet_addr(ip_servidor);

    // Enviar nombre de usuario
    sendto(sockfd, usuario, strlen(usuario), 0, (struct sockaddr*)&servidor, addr_size);

    int opcion;
    while (1) {
        printf("\nSeleccione una opci\xC3\xB3n:\n");
        printf("1. Ver usuarios conectados\n");
        printf("2. Enviar mensaje a otro usuario\n");
        printf("3. Revisar mensajes recibidos\n");
        printf("4. Desconectarse\n");
        printf("Opci\xC3\xB3n: ");
        scanf("%d", &opcion);

        sprintf(buffer, "%d", opcion);
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&servidor, addr_size);

        if (opcion == 1 || opcion == 3 || opcion == 4) {
            recvfrom(sockfd, buffer, MAX_MSG, 0, (struct sockaddr*)&servidor, &addr_size);
            printf("\n%s\n", buffer);

            if (opcion == 4) break;
        } else if (opcion == 2) {
            // Paso 1: pedir destinatario
            recvfrom(sockfd, buffer, MAX_MSG, 0, (struct sockaddr*)&servidor, &addr_size);
            printf("%s ", buffer);
            scanf("%s", buffer);
            sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&servidor, addr_size);

            // Paso 2: mensaje de texto
            recvfrom(sockfd, buffer, MAX_MSG, 0, (struct sockaddr*)&servidor, &addr_size);
            printf("%s ", buffer);
            getchar();
            fgets(buffer, MAX_MSG, stdin);
            buffer[strcspn(buffer, "\n")] = '\0';
            sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&servidor, addr_size);

            // Confirmación
            recvfrom(sockfd, buffer, MAX_MSG, 0, (struct sockaddr*)&servidor, &addr_size);
            printf("%s\n", buffer);
        } else {
            recvfrom(sockfd, buffer, MAX_MSG, 0, (struct sockaddr*)&servidor, &addr_size);
            printf("%s\n", buffer);
        }
    }

    close(sockfd);
    return 0;
}