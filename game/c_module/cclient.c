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
char buffer[T_BUFF];
char direction[2];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t c = PTHREAD_MUTEX_INITIALIZER;
//mutex para os controles

void *leitura(void *arg) {
//    char buffer[256];
    int n, i;
    while (1) {
        pthread_mutex_lock(&mutex);
            bzero(buffer,sizeof(buffer));
            n = recv(sockfd,buffer,24,0);
            for(i=0;i<T_BUFF;i++){
                if (buffer[i] == ' ')
                    buffer[i] = '0';
            }
//            printf("%s\n", buffer);
        pthread_mutex_unlock(&mutex);
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

    portno = 9000;//atoi(argv[2]);
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
    if (connect(sockfd,(struct sockaddr_in *) &serv_addr,sizeof(serv_addr)) < 0) {
        printf("Erro conectando!\n");
        return -1;
    }
    pthread_create(&t, NULL, leitura, NULL);
    do {
        bzero(buffer,sizeof(buffer));
        //printf("Digite a mensagem (ou sair):");
        //fgets(buffer,50,stdin);
        pthread_mutex_lock(&c);
        n = send(sockfd,direction,2,0);
        pthread_mutex_lock(&c);
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
    int inf[7];
    char temp[5];
    int head=1, tail=3,i;
    pthread_mutex_lock(&mutex);
        //printf("%s\n", buffer);
    strncpy (temp, buffer, 1);
    temp[1] = '\0';
    inf[0] = atoi(temp);
//    printf("%d\n", inf[0]);
    if(inf[0]>0){
        for (i=1;i<7;i++){
            strncpy (temp, buffer+head, 3);
            temp[4] = '\0';
            inf[i] = atoi(temp);
            //printf("%d\n", inf[i]);
            head += 3;
            tail+=3;
        }
    }
    else{
        printf("pacote perdido!\n");
    }
    pthread_mutex_unlock(&mutex);
        return Py_BuildValue("iiiiiii", inf[0], inf[1], inf[2], inf[3], inf[4], inf[5], inf[6]);

}

static PyObject *cclient_move(PyObject *self, PyObject *args){
    char dir[2];
    if (!PyArg_ParseTuple(args, "s", &dir))
        return NULL;
    pthread_mutex_lock(&c);
        strcpy(direction, dir);
    pthread_mutex_unlock(&c);
    printf("%s", direction);
    return Py_BuildValue("s", direction);
}

static PyMethodDef cclient_methods[] = {
    {"start", (PyCFunction) cclient_start, METH_NOARGS, NULL},
    {"pos", (PyCFunction) cclient_pos, METH_VARARGS, NULL},
    {"move", (PyCFunction) cclient_move, METH_VARARGS, NULL}
};

PyMODINIT_FUNC initcclient(){
    Py_InitModule3("cclient", cclient_methods, "whatever");
}
