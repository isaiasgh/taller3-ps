#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <unistd.h>

#define SERVER_ADDRESS  "192.168.1.12"     /* server IP */
#define PORT            8080

/* Test sequences */
char buf_tx[] = "Hello server. I am a client";      
char buf_rx[500];                     /* receive buffer */
 
 
/* This clients connects, sends a text and disconnects */
int main() 
{ 
    int sockfd; 
    struct sockaddr_in servaddr; 
    
    /* Socket creation */
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) 
    { 
        printf("CLIENT: socket creation failed...\n"); 
        return -1;  
    } 
    else
    {
        printf("CLIENT: Socket successfully created..\n"); 
    }
    
    
    memset(&servaddr, 0, sizeof(servaddr));

    /* assign IP, PORT */
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr( SERVER_ADDRESS ); 
    servaddr.sin_port = htons(PORT); 
  
    /* try to connect the client socket to server socket */
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) 
    { 
        printf("connection with the server failed...\n");  
        return -1;
    } 
    
    printf("connected to the server..\n"); 
  
    /* send test sequences*/
    read(sockfd, buf_rx, sizeof(buf_rx));
    printf("%s \n", buf_rx);

    printf("-> ");

    // ingreso facultad/carrera
    char facu [6];
    scanf("%s", facu);
    write(sockfd, facu, sizeof(facu));

    // pide matricula
    read(sockfd, buf_rx, sizeof(buf_rx));
    printf("%s", buf_rx);

    int caracter;
    while ((caracter = getchar()) != '\n' && caracter != EOF);

    // ingreso matricula
    char matricula [10];
    printf("->");
    scanf("%s", matricula);
    write(sockfd, matricula, sizeof(matricula));


    // muestra info
    read(sockfd, buf_rx, sizeof(buf_rx));
    printf("%s", buf_rx);

    // verifica que todo este bien
    read(sockfd, buf_rx, sizeof(buf_rx));

    if (strcmp(buf_rx, "e9090") == 0){
        read(sockfd, buf_rx, sizeof(buf_rx));
        printf("%s", buf_rx);

        close(sockfd);
        return 1;

    } else {
        // menu - opciones
        // read(sockfd, buf_rx, sizeof(buf_rx));
        printf("%s", buf_rx);

        // enviamos opcion
        char opcion[1];
        printf("-> ");
        scanf("%s", opcion);
        write(sockfd, opcion, sizeof(opcion));

        int b = 0;

        while (opcion[0] != '0'){

            if (opcion[0] = '1'){
                // instrucciones
                read(sockfd, buf_rx, sizeof(buf_rx));
                printf("%s", buf_rx);

                // mandamos el promedio / faculta
                char p [10];

                printf("-> ");
                scanf("%9s", p);
                write(sockfd, p, sizeof(p));

                // recibimos respuesta exitosa
                read(sockfd, buf_rx, sizeof(buf_rx));
                printf("%s", buf_rx);

                // menu
                read(sockfd, buf_rx, sizeof(buf_rx));
                printf("%s", buf_rx);

                // nueva opcion
                printf("-> ");
                scanf("%s", opcion);
                write(sockfd, opcion, sizeof(opcion));

                b = 1;

            } else if (opcion[0] = '2'){
                // instrucciones
                read(sockfd, buf_rx, sizeof(buf_rx));
                printf("%s", buf_rx);
                
                // mandamos cant materias ap / carrera
                char m [10];

                printf("-> ");
                scanf("%9s", m);
                write(sockfd, m, sizeof(m));

                // recibimos respuesta exitosa
                read(sockfd, buf_rx, sizeof(buf_rx));
                printf("%s", buf_rx);

                // menu
                read(sockfd, buf_rx, sizeof(buf_rx));
                printf("%s", buf_rx);

                // nueva opcion
                printf("-> ");
                scanf("%s", opcion);
                write(sockfd, opcion, sizeof(opcion));

                b = 1;

            } else if (opcion[0] = '3'){
                // instrucciones
                read(sockfd, buf_rx, sizeof(buf_rx));
                printf("%s", buf_rx);

                // mandamos cant materias rp
                char r[10];

                printf("-> ");
                scanf("%9s", r);
                write(sockfd, r, sizeof(r));

                // recibimos respuesta exitosa
                read(sockfd, buf_rx, sizeof(buf_rx));
                printf("%s", buf_rx);

                // menu
                read(sockfd, buf_rx, sizeof(buf_rx));
                printf("%s", buf_rx);

                // nueva opcion
                printf("-> ");
                scanf("%s", opcion);
                write(sockfd, opcion, sizeof(opcion));

                b = 1;
            }

            // termina el programa
            if (opcion[0] == '0'){

                // si no hubo cambios
                if (b == 0){
                    read(sockfd, buf_rx, sizeof(buf_rx));
                    printf("%s", buf_rx);
                    return 0;
                    
                } else {
                    read(sockfd, buf_rx, sizeof(buf_rx));
                    printf("%s", buf_rx);
                    return 0;
                }

            }

        }  

    }

    /* close the socket */
    close(sockfd); 
} 