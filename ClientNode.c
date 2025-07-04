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

    //agregar envio de nombre al servidor a modo de ingreso de datos

    //conectar serv
	if( bind(descriptor_socket, (struct sockaddr*)&servidor, sizeof(servidor)) < 0) {
        perror("Error al conectar");
        return 1;
  }


    printf("\nConectado como '%s'. Puede comenzar a escribir mensajes.\n", usuario);


    //en el bucle de mensajes integrar una seccion que de al usuario, varias opciones, con todas las posibilidades expresadas en el diagrama de casos de uso.
    //eso es pedir la lista de usuarios que estan en el servidor, abrir un modo de envio de mensajes hacia otros usuarios, poder ver lo mensajes que a recibido (algo asi como un buzon) y la opcion de leer dichos mensajes.
    //tambien la opcion de desconectarse 

    //bucle de mensajes
    while (1) {
        printf("\n=== Menú ===\n");
        printf("1. Ver lista de usuarios conectados\n");
        printf("2. Enviar mensaje a un usuario\n");
        printf("3. Ver mensajes recibidos (buzón)\n");
        printf("4. Leer mensaje por ID\n");
        printf("5. Desconectarse\n");
        printf("escriba el numero de una opcion: ");

        int opcion;
        scanf("%d",&opcion);

        if (opcion == 1) {
            enviar_y_mostrar_respuesta("LISTA");

        } else if (opcion == 2) {
            char destino[50], texto[MAX_MSG];
            printf("Usuario destino: ");
            fgets(destino, sizeof(destino), stdin);
            destino[strcspn(destino, "\n")] = '\0';

            printf("Mensaje: ");
            fgets(texto, sizeof(texto), stdin);
            texto[strcspn(texto, "\n")] = '\0';

            snprintf(mensaje, MAX_MSG, "MSG:%s:%s:%s", usuario, destino, texto);
            enviar_y_mostrar_respuesta(mensaje);

        } else if (opcion == 3) {
            snprintf(mensaje, MAX_MSG, "BUZON:%s", usuario);
            enviar_y_mostrar_respuesta(mensaje);

        } else if (opcion == 4) {
            char id[10];
            printf("ID del mensaje: ");
            fgets(id, sizeof(id), stdin);
            id[strcspn(id, "\n")] = '\0';

            snprintf(mensaje, MAX_MSG, "LEER:%s:%s", usuario, id);
            enviar_y_mostrar_respuesta(mensaje);

        } else if (opcion == 5) {
            cerrar_cliente(SIGINT);

        } else {
            printf("Opción no válida. Intente nuevamente.\n");
        }
    }

    close(descriptor_socket);
    return 0;
}