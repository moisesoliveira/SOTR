#include <stdio.h>
#include <Python.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define T_BUFF 24

int varglobal=0;
char buffer[T_BUFF];
pthread_t t;
int newsockfd[2];
int id = 0;

void *cliente(void *arg){
    printf("cliente funcionando!\n");
    int cid = (int)arg;
    int i, n;
//    char buffer[256];
    while (1) {
        //bzero(buffer,sizeof(buffer));
        //n = read(newsockfd[cid],buffer,50);
        //printf("Recebeu: %s - %lu\n", buffer,strlen(buffer));
        //if (n < 0) {
            //printf("Erro lendo do socket!\n");
            //exit(1);
        //}
	// MUTEX LOCK - GERAL
        printbuff();
        for (i = 0;i < id; i++){
            //if (i != cid) {
                printf("escrevendo no socket!\n");
                n = write(newsockfd[i],buffer,T_BUFF);
                if (n < 0) {
                    printf("Erro escrevendo no socket!\n");
                    exit(1);
                }
            //}
		    // COMO LIDAR COM COMANDO SAIR
        }
	// MUTEX UNLOCK - GERAL
    }
}


void *conn(void *arg) {
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    int sockfd, portno;
//     char buffer[256];
//     int n;
    pthread_t t;
/*    if (argc < 2) {*/
/*        printf("Erro, porta nao definida!\n");*/
/*        exit(1);*/
/*    }*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Erro abrindo o socket!\n");
        exit(1);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 9003;//atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        printf("Erro fazendo bind!\n");
        exit(1);
    }
    listen(sockfd,5);
    while (1) {
        newsockfd[id] = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
	// MUTEX LOCK - GERAL
        if (newsockfd[id] < 0) {
            printf("Erro no accept!\n");
            exit(1);
        }
        pthread_create(&t, NULL, cliente, (void *)id);
        id++;
	// MUTEX UNLOCK - GERAL
    }
//    close(newsockfd);
//    close(sockfd);
    return 0; 
}


void printbuff(){
    int i;

    printf("\e[1;1H\e[2J");
    printf("O buffer é: ");

    printf("%s\n", buffer);
    printf("\n");
}

static PyObject *hello_hello(PyObject *self){
    pthread_t t;
    printf("Hello, estou chamando uma thread!\n");
    pthread_create(&t, NULL, conn, NULL);
    Py_RETURN_NONE;
}

static PyObject *hello_add(PyObject *self, PyObject *args){
    int id, p1x, p2x, bx, by, s1,s2,chk;
    if (!PyArg_ParseTuple(args, "iiiiiiii", &id, &p1x, &p2x, &bx, &by, &s1, &s2,&chk))
        return NULL;
    sprintf (buffer,"%d%3.d%3.d%3.d%3.d%3.d%3.d%d", id, p1x, p2x, bx, by, s1,s2,chk);

    return Py_BuildValue("i", id);
}



static PyObject *hello_teste(PyObject *self, PyObject *args){
    int a;
    int b;
    if (!PyArg_ParseTuple(args, "ii", &a, &b))
        return NULL;
    a = a+1;
    printf("running\n");
    sleep(5);
    return Py_BuildValue("i", a+b);

}


static PyMethodDef hello_methods[] = {
    {"hello", (PyCFunction) hello_hello, METH_NOARGS, NULL },
    {"add", (PyCFunction) hello_add, METH_VARARGS, NULL},
    {"teste", (PyCFunction) hello_teste, METH_VARARGS, NULL}
};

PyMODINIT_FUNC inithello(){
    Py_InitModule3("hello", hello_methods, "whatever");
}
