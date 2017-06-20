#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>      // inet_aton
#include <pthread.h>
#import <Python.h>

#define T_BUFF 24
int sockfd;
char buffer[T_BUFF];
void printbuff(){
    int i;
//    printf("\e[1;1H\e[2J");
//    printf("O buffer é: ");

    for(i=0;i<T_BUFF;i++){
        printf("%c", buffer[i]);
    }
    printf("\n");
}

void *leitura(void *arg) {
//    char buffer[256];
    int n;
    while (1) {
        //bzero(buffer,sizeof(buffer));
        n = recv(sockfd,buffer,24,0);
        if (n <= 0) {
            printf("Erro lendo do socket!\n");
            exit(1);
        }
        //printf("MSG: %s\n",buffer);
        //printbuff();
    }
}

void *client(void *arg) {
    int portno, n;
    struct sockaddr_in serv_addr;
    pthread_t t;
//    char buffer[256];
/*    if (argc < 3) {
       fprintf(stderr,"Uso: %s nomehost porta\n", argv[0]);
       exit(0);
    }*/
    portno = 9999;//atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Erro criando socket!\n");
        return -1;
    }
    //bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
//    serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
//    inet_aton(argv[1], &serv_addr.sin_addr);
    inet_aton("127.0.0.1", &serv_addr.sin_addr);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        printf("Erro conectando!\n");
        return -1;
    }
    pthread_create(&t, NULL, leitura, NULL);
    do {
        //bzero(buffer,sizeof(buffer));
        //printf("Digite a mensagem (ou sair):");
        //fgets(buffer,50,stdin);
        //n = send(sockfd,buffer,50,0);
        if (n == -1) {
            printf("Erro escrevendo no socket!\n");
            return -1;
        }
        if (strcmp(buffer,"sair\n") == 0) {
            break;
        }
    } while (1);
    close(sockfd);
    return 0;
}


static PyObject *cclient_start(PyObject *self){
    pthread_t cli;
    printf("cclient, estou chamando uma thread!\n");
    pthread_create(&cli, NULL, client, NULL);
    Py_RETURN_NONE;
}

static PyObject *cclient_pos(PyObject *self, PyObject *args){
//    int id, p1x, p2x, bx, by, s1,s2,chk;
//    if (!PyArg_ParseTuple(args, "iiiiiiii", &id, &p1x, &p2x, &bx, &by, &s1, &s2,&chk))
//        return NULL;
//    sprintf (buffer,"%d%3.d%3.d%3.d%3.d%3.d%3.d%d", id, p1x, p2x, bx, by, s1,s2,chk);
    //printf("buffer: %s", buffer);
    char buff2[T_BUFF];
    int i;
    for(i=0;i<T_BUFF;i++){
        if (buffer[i] == ' ')
            buff2[i] = '0';
        else
            buff2[i] = buffer[i];
    }
    return Py_BuildValue("s", buff2);
}



static PyMethodDef cclient_methods[] = {
    {"start", (PyCFunction) cclient_start, METH_NOARGS, NULL},
    {"pos", (PyCFunction) cclient_pos, METH_VARARGS, NULL}
};

PyMODINIT_FUNC initcclient(){
    Py_InitModule3("cclient", cclient_methods, "whatever");
}
