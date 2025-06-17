#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX_MSG 512

char mensaje[MAX_MSG];
char usuario[50];
int descriptor_socket;
struct sockaddr_in servidor;
struct sockaddr_in cliente;   // <-- variable para dirección local
socklen_t largo_servidor;

void cerrar_cliente(int sig) {
    printf("\n[!] Terminando cliente...\n");
    snprintf(mensaje, MAX_MSG, "SALIR:%s", usuario);
    sendto(descriptor_socket, mensaje, strlen(mensaje), 0,
           (struct sockaddr *)&servidor, largo_servidor);
    close(descriptor_socket);
    exit(0);
}

void enviar_y_mostrar_respuesta(const char *mensaje_envio) {
    sendto(descriptor_socket, mensaje_envio, strlen(mensaje_envio), 0,
           (struct sockaddr *)&servidor, largo_servidor);

    char respuesta[MAX_MSG];
    int len = recvfrom(descriptor_socket, respuesta, MAX_MSG - 1, 0, NULL, NULL);
    if (len >= 0) {
        respuesta[len] = '\0';
        printf("[Servidor]: %s\n", respuesta);
    } else {
        perror("Error al recibir respuesta del servidor");
    }
}

int main() {
    signal(SIGINT, cerrar_cliente);

    char ip[20];
    int puerto;

    printf("Ingrese IP del servidor: ");
    scanf("%19s", ip);
    printf("Ingrese puerto del servidor: ");
    scanf("%d", &puerto);
    printf("Ingrese nombre de usuario: ");
    scanf("%49s", usuario);
    getchar();  // limpiar \n

    descriptor_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (descriptor_socket < 0) {
        perror("Error al crear socket UDP");
        return 1;
    }

    // Configurar dirección local para el cliente y bindear
    memset(&cliente, 0, sizeof(cliente));
    cliente.sin_family = AF_INET;
    cliente.sin_port = htons(0); // Puerto 0: asigna uno libre automáticamente
    cliente.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(descriptor_socket, (struct sockaddr*)&cliente, sizeof(cliente)) == -1) {
        perror("ERROR al configurar el socket del cliente (bind)");
        close(descriptor_socket);
        exit(EXIT_FAILURE);
    }

    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(puerto);
    servidor.sin_addr.s_addr = inet_addr(ip);
    largo_servidor = sizeof(servidor);

    // Enviar nombre de usuario al servidor
    snprintf(mensaje, MAX_MSG, "NOMBRE:%s", usuario);
    enviar_y_mostrar_respuesta(mensaje);

    // Menú principal
    while (1) {
        printf("\n=== Menú ===\n");
        printf("1. Ver lista de usuarios conectados\n");
        printf("2. Enviar mensaje a un usuario\n");
        printf("3. Ver mensajes recibidos (buzón)\n");
        printf("4. Leer mensaje por ID\n");
        printf("5. Desconectarse\n");
        printf("Seleccione una opción: ");

        char opcion[10];
        fgets(opcion, sizeof(opcion), stdin);

        if (strncmp(opcion, "1", 1) == 0) {
            enviar_y_mostrar_respuesta("LISTA");

        } else if (strncmp(opcion, "2", 1) == 0) {
            char destino[50], texto[MAX_MSG];
            printf("Usuario destino: ");
            fgets(destino, sizeof(destino), stdin);
            destino[strcspn(destino, "\n")] = '\0';

            printf("Mensaje: ");
            fgets(texto, sizeof(texto), stdin);
            texto[strcspn(texto, "\n")] = '\0';

            snprintf(mensaje, MAX_MSG, "MSG:%s:%s:%s", usuario, destino, texto);
            enviar_y_mostrar_respuesta(mensaje);

        } else if (strncmp(opcion, "3", 1) == 0) {
            snprintf(mensaje, MAX_MSG, "BUZON:%s", usuario);
            enviar_y_mostrar_respuesta(mensaje);

        } else if (strncmp(opcion, "4", 1) == 0) {
            char id[10];
            printf("ID del mensaje: ");
            fgets(id, sizeof(id), stdin);
            id[strcspn(id, "\n")] = '\0';

            snprintf(mensaje, MAX_MSG, "LEER:%s:%s", usuario, id);
            enviar_y_mostrar_respuesta(mensaje);

        } else if (strncmp(opcion, "5", 1) == 0) {
            cerrar_cliente(SIGINT);

        } else {
            printf("Opción no válida. Intente nuevamente.\n");
        }
    }

    return 0;
}
