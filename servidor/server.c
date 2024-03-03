#include <unistd.h>  

/* sockets */
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>

/* strings / errors*/
#include <errno.h>
#include <stdio.h> 
#include <string.h> 

#include "funciones.h"

/* server parameters */
#define SERV_PORT       8080              /* port */
#define SERV_HOST_ADDR "192.168.1.12"     /* IP, only IPV4 support  */
#define BUF_SIZE        500              /* Buffer rx, tx max size  */
#define BACKLOG         5                 /* Max. client pending connections  */

int main(int argc, char* argv[])          /* input arguments are not used */
{ 
    int sockfd, connfd ;  /* listening socket and connection socket file descriptors */
    unsigned int len;     /* length of client address */
    struct sockaddr_in servaddr, client; 
    
    int  len_rx, len_tx = 0;                     /* received and sent length, in bytes */
    char buff_tx[BUF_SIZE] = "[SERVER]: Ingrese la facultad/carrera:";
    char buff_rx[BUF_SIZE];   /* buffers for reception  */
    
     
    /* socket creation */
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) 
    { 
        fprintf(stderr, "[SERVER-error]: socket creation failed. %d: %s \n", errno, strerror( errno ));
        return -1;
    } 
    else
    {
        printf("[SERVER]: Socket successfully created..\n"); 
    }
    
    /* clear structure */
    memset(&servaddr, 0, sizeof(servaddr));
  
    /* assign IP, SERV_PORT, IPV4 */
    servaddr.sin_family      = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); 
    servaddr.sin_port        = htons(SERV_PORT); 
    
    
    /* Bind socket */
    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) 
    { 
        fprintf(stderr, "[SERVER-error]: socket bind failed. %d: %s \n", errno, strerror( errno ));
        return -1;
    } 
    else
    {
        printf("[SERVER]: Socket successfully binded \n");
    }
  
    /* Listen */
    if ((listen(sockfd, BACKLOG)) != 0) 
    { 
        fprintf(stderr, "[SERVER-error]: socket listen failed. %d: %s \n", errno, strerror( errno ));
        return -1;
    } 
    else
    {
        printf("[SERVER]: Listening on SERV_PORT %d \n\n", ntohs(servaddr.sin_port) ); 
    }
    
    len = sizeof(client); 
  
      /* Accept the data from incoming sockets in a iterative way */
        
      while(1)
      {
        connfd = accept(sockfd, (struct sockaddr *)&client, &len); 
        if (connfd < 0) 
        { 
            fprintf(stderr, "[SERVER-error]: connection not accepted. %d: %s \n", errno, strerror( errno ));
            return -1;
        } 
        else
        {          
            write(connfd, buff_tx, strlen(buff_tx));    
            while(1) /* read data from a client socket till it is closed */ 
            {  
                /* read client message, copy it into buffer */
                len_rx = read(connfd, buff_rx, sizeof(buff_rx));  
                
                if(len_rx == -1)
                {
                    fprintf(stderr, "[SERVER-error]: connfd cannot be read. %d: %s \n", errno, strerror( errno ));
                }
                else if(len_rx == 0) /* if length is 0 client socket closed, then exit */
                {
                    printf("[SERVER]: client socket closed \n\n");
                    close(connfd);
                    break; 
                }
                else
                {
                    printf("[CLIENTE]: %s \n", buff_rx);
                    
                    int longitud = strlen(buff_rx);
                    for (int i = 0; i < longitud; ++i) {
                        buff_rx[i] = toupper((unsigned char)buff_rx[i]);
                    }

                    char es_carr_fac = 'z';
                    es_carr_fac = buff_rx[0];

                    
                    struct Estudiante est;
                    struct lista l;
                    l = autenticarCliente(connfd, buff_rx);
                    est = l.est;

                    if (l.id == -1){
                        write(connfd, "e9090", sizeof("e9090"));
                        printf("[ERROR]: e9090 - Estudiante no encontrado\n");
                        write(connfd, "[SERVER]: Conexion finalizada", sizeof("[SERVER]: Conexion finalizada"));

                        return 0;

                    } else {
                        l = menu(connfd, es_carr_fac, est);

                        // hubo cambios -> guardar
                        if(l.id == 1){
                            struct Estudiante est_a;
                            est_a = est;
                            est = l.est;

                            // llamamos funcion para guardar

                            int conf = guardar(est_a, est);

                            if (conf == 1){
                                write(connfd, "[SERVER]: Datos guardados exitosamente. Saliendo\n", sizeof("[SERVER]: Datos guardados exitosamente. Saliendo\n"));
                                printf("[SERVER]: Datos guardados exitosamente. Saliendo\n");

                            } else {
                                write(connfd, "[SERVER]: Error al guardar los datos. Saliendo\n", sizeof("[SERVER]: Error al guardar los datos. Saliendo\n"));
                                printf("[SERVER]: Error al guardar los datos. Saliendo\n");

                            }




                        } else {
                            write(connfd, "[SERVER]: Ningun cambio registrado. Saliendo\n", sizeof("[SERVER]: Ningun cambio registrado. Saliendo\n"));
                            printf("[SERVER]: Ningun cambio registrado. Saliendo\n");

                        }


                    }

                }            
            }  
        }                      
    }    
} 