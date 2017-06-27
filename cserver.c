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
#include <errno.h>
#include <signal.h>
#include <time.h>
#define T_BUFF 24
//#define PORTA 9000
int portno;
//buffers de envio e recepção de dados do atraves do socket
char buffer[T_BUFF], direction[2][2];
pthread_t t;
int newsockfd[2];
int id = 0;
int p1x, p2x, bx, by, s1,s2,chk;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t recept = PTHREAD_MUTEX_INITIALIZER;

void *cliente(void *arg){
    int cid = (int)arg;
    int i, n;
    char aux[2];
    while (1) {
        pthread_mutex_lock(&recept);
        bzero(direction[cid],sizeof(direction[cid]));
        read(newsockfd[cid],direction[cid],2);
//        printf("\e[H\e[2J"); //printf() para limpar a tela
//        printf("%s\n",direction[cid]);
        pthread_mutex_unlock(&recept);
        pthread_mutex_lock(&mutex);
            for (i = 0;i < 2; i++){
                n = send(newsockfd[i],buffer,T_BUFF,0);
                //if(n < 0)
                    //printf("Erro enviando socket!\n");
            }
        pthread_mutex_unlock(&mutex);
    usleep(200);
    }
}

void *printstatus(void *arg){
//    int id, p1x, p2x, bx, by, s1,s2,chk;
    while(1){
        printf("\e[H\e[2J"); //printf() para limpar a tela
        printf("Players online:\t\t %d\n", id);
        printf("Posição do player 1:\t %d\n", p1x);
        printf("Posição do player 2:\t %d\n", p2x);
        printf("Posição da bola:\t %dx%d\n", bx,by);
        printf("Placar:\t\t\t %dx%d\n", s1, s2);
        printf("Direção do player:\t%s:%s\n", direction[0], direction[1]);
        usleep(100);
    }
}
void *conn(void *arg) {
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    int sockfd,i;
    pthread_t t[2], prints;

    sigset_t alarm_sig;
    sigemptyset (&alarm_sig);
    sigaddset (&alarm_sig, SIGALRM);
    sigprocmask(SIG_BLOCK, &alarm_sig, NULL);
    //tarefa periodica
    pthread_create(&prints, NULL, printstatus, NULL);
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Erro abrindo o socket!\n");
        exit(1);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        printf("Erro fazendo bind!\n");
        exit(1);
    }
    listen(sockfd,5);
    while (id<3) {
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
    while(1){
        sleep(1);
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
//    int id, p1x, p2x, bx, by, s1,s2,chk; //tornei essas variaveis globais
    int i = 0;
    if (!PyArg_ParseTuple(args, "iiiiiiii", &i, &p1x, &p2x, &bx, &by, &s1, &s2,&chk))
        return NULL;
    pthread_mutex_lock(&mutex);
        sprintf (buffer,"%d%3.d%3.d%3.d%3.d%3.d%3.d%d", i, p1x, p2x, bx, by, s1,s2,chk);
    pthread_mutex_unlock(&mutex);
    return Py_BuildValue("i", i);
}

static PyObject *cserver_control(PyObject *self, PyObject *args){
    int player = 0;
    int dir;
    if (!PyArg_ParseTuple(args, "i", &player))
        return NULL;
    dir = atoi(direction[player-1]);
    return Py_BuildValue("i", dir);
}

static PyMethodDef cserver_methods[] = {
    {"start", (PyCFunction) cserver_start, METH_VARARGS, NULL },
    {"add", (PyCFunction) cserver_add, METH_VARARGS, NULL},
    {"control", (PyCFunction) cserver_control, METH_VARARGS, NULL},
    { NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC initcserver(){
    Py_InitModule3("cserver", cserver_methods, "whatever");
}
