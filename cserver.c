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
//#define PORTA 9000
int portno;
char buffer[T_BUFF], direction[2];
pthread_t t;
int newsockfd[2];
int id = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void *cliente(void *arg){
    printf("cliente funcionando!\n");
    int cid = (int)arg;
    int i, n;
//    char buffer[T_BUFF];
    while (1) {
        bzero(direction,sizeof(direction));
        n = read(newsockfd[cid],direction,2);
        printf("\e[1;1H\e[2J");
        printf("Recebeu de %d: %s\n",cid, direction);
/*        if (n < 0) {
            printf("Erro lendo do socket!\n");
            exit(1);
        }*/
	// MUTEX LOCK - GERAL
        pthread_mutex_lock(&mutex);
//            printf("\e[1;1H\e[2J");
/*            printf("O buffer é: ");

            printf("%s\n", buffer);
            printf("\n");
*/
            for (i = 0;i < id; i++){
                //if (i != cid) {
//                    printf("escrevendo no socket!\n");
                    n = write(newsockfd[i],buffer,T_BUFF);
                    if (n < 0) {
                        printf("Erro escrevendo no socket!\n");
                        exit(1);
                    }
                //}
		        // COMO LIDAR COM COMANDO SAIR
            }
	    // MUTEX UNLOCK - GERAL
        pthread_mutex_unlock(&mutex);
    }
}


void *conn(void *arg) {
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    int sockfd;
    //int portno;
//     char buffer[256];
//     int n;
    pthread_t t[2];
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
//    portno = PORTA;//atoi(argv[1]);
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
        pthread_mutex_lock(&mutex);
        if (newsockfd[id] < 0) {
            printf("Erro no accept!\n");
            exit(1);
        }
        pthread_create(&t[id], NULL, cliente, (void *)id);
        id++;
	// MUTEX UNLOCK - GERAL
        pthread_mutex_unlock(&mutex);
    }
//    close(newsockfd);
//    close(sockfd);
    return 0; 
}


static PyObject *cserver_start(PyObject *self, PyObject *args){
    pthread_t con;
    if (!PyArg_ParseTuple(args, "i", &portno))
        return NULL;
    printf("cserver, iniciando na porta %d!\n", portno);
    pthread_create(&con, NULL, conn, NULL);
    Py_RETURN_NONE;
}

static PyObject *cserver_add(PyObject *self, PyObject *args){
    int id, p1x, p2x, bx, by, s1,s2,chk;
    id = 0;
    if (!PyArg_ParseTuple(args, "iiiiiiii", &id, &p1x, &p2x, &bx, &by, &s1, &s2,&chk))
        return NULL;
    pthread_mutex_lock(&mutex);
        sprintf (buffer,"%d%3.d%3.d%3.d%3.d%3.d%3.d%d", id, p1x, p2x, bx, by, s1,s2,chk);
    pthread_mutex_unlock(&mutex);
    return Py_BuildValue("i", id);
}

static PyMethodDef cserver_methods[] = {
    {"start", (PyCFunction) cserver_start, METH_VARARGS, NULL },
    {"add", (PyCFunction) cserver_add, METH_VARARGS, NULL},
//    {"teste", (PyCFunction) cserver_teste, METH_VARARGS, NULL}
    { NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC initcserver(){
    Py_InitModule3("cserver", cserver_methods, "whatever");
}
