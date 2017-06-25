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
#import <time.h>

#define T_BUFF 24
int sockfd;
int portno;// = 9000;
//char ipaddr[] = "127.0.0.1";
char buffer[T_BUFF], direction[2];
int dir = 3;// direção enviada pelos controles, but int
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *leitura(void *arg) {
    int n;
    while (1) {
        pthread_mutex_lock(&mutex);
            bzero(buffer,sizeof(buffer));
            n = recv(sockfd,buffer,T_BUFF,0);
        pthread_mutex_unlock(&mutex);
        if (n <= 0) {
            printf("Erro lendo do socket!\n");
            exit(1);
        }
    }
}

void *client(void *arg) {
    int n;//portno;
    struct sockaddr_in serv_addr;
    pthread_t t;
//    char buffer[256];
/*    if (argc < 3) {
       fprintf(stderr,"Uso: %s nomehost porta\n", argv[0]);
       exit(0);
    }*/
//    portno = 9000;//atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Erro criando socket!\n");
        return -1;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
//    serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
//    inet_aton(argv[1], &serv_addr.sin_addr);
    inet_aton("127.0.0.1", &serv_addr.sin_addr);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        printf("Erro conectando!\n");
        return -1;
    }
    //cria a thread para fazer a leitura do socket
    pthread_create(&t, NULL, leitura, NULL);
    //e segue fazendo a escrita no socket
    do {
        bzero(direction,sizeof(direction));
//        printf("Digite a mensagem (ou sair):");
//        fgets(buffer,50,stdin);

        sprintf(direction, "%d",dir);
        printf("Enviou: %s\n", direction);
        n = send(sockfd,direction,2,0);
        if (n == -1) {
            printf("Erro escrevendo no socket!\n");
            return n;
        }
        if (strcmp(buffer,"sair\n") == 0) {
            break;
        }
    } while (1);
    close(sockfd);
    return 0;
}


static PyObject *cclient_start(PyObject *self, PyObject *args){
    pthread_t cli;
    if (!PyArg_ParseTuple(args, "i", &portno))
        return NULL;
    printf("cclient, conectando no endereço 127.0.0.1:%d!\n", portno);
    pthread_create(&cli, NULL, client, NULL);
    Py_RETURN_NONE;
}

static PyObject *cclient_pos(PyObject *self, PyObject *args){
    int i, head = 1, tail = 3;
    int inf[7]; //id, p1x, p2x, bx, by, s1,s2; 
    char temp[5];
    pthread_mutex_lock(&mutex);
    strncpy (temp, buffer, 1);
    temp[1] = '\0';
    inf[0] = atoi(temp);
//    printf("%d\n", inf[0]);
    /*converter o buffer para um vetor de numeros 
    inteiros contendo as posições dos objetos do jogo*/
    for (i=1;i<7;i++){
        strncpy (temp, buffer+head, 3);
        temp[4] = '\0';
        inf[i] = atoi(temp);
//        printf("%d\n", inf[i]);
        head += 3;
        tail+=3;
    }
    pthread_mutex_unlock(&mutex);
        return Py_BuildValue("iiiiiii", inf[0], inf[1], inf[2], inf[3], inf[4], inf[5], inf[6]);

}

static PyObject *cclient_move(PyObject *self, PyObject *args){
    if (!PyArg_ParseTuple(args, "i", &dir))
        return NULL;
//    printf("%d", dir);
    Py_RETURN_NONE;
}

static PyMethodDef cclient_methods[] = {
    {"start", (PyCFunction) cclient_start, METH_VARARGS, NULL},
    {"pos", (PyCFunction) cclient_pos, METH_VARARGS, NULL},
    {"move", (PyCFunction) cclient_move, METH_VARARGS, NULL}
};

PyMODINIT_FUNC initcclient(){
    Py_InitModule3("cclient", cclient_methods, "whatever");
}
