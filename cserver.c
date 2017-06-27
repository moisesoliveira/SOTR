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
#include <sys/time.h>
#define T_BUFF 24
//#define PORTA 9000
int portno;
//buffers de envio e recepção de dados do atraves do socket
char buffer[T_BUFF], direction[2][2];
pthread_t t;
int newsockfd[2];
int id = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t recept = PTHREAD_MUTEX_INITIALIZER;

struct periodic_info {
	sigset_t alarm_sig;
};

static int make_periodic(unsigned int period, struct periodic_info *info){
    int ret;
    struct itimerval value;

    /* Block SIGALRM in this thread */
    sigemptyset(&(info->alarm_sig));
    sigaddset(&(info->alarm_sig), SIGALRM);
    pthread_sigmask(SIG_BLOCK, &(info->alarm_sig), NULL);

    /* Set the timer to go off after the first period and then
       repetitively */
    value.it_value.tv_sec = period / 1000000;
    value.it_value.tv_usec = period % 1000000;
    value.it_interval.tv_sec = period / 1000000;
    value.it_interval.tv_usec = period % 1000000;
    ret = setitimer(ITIMER_REAL, &value, NULL);
    if (ret != 0)
        perror("Failed to set timer");
    return ret;
}

static void wait_period(struct periodic_info *info){
    int sig;

    /* Wait for the next SIGALRM */
    sigwait(&(info->alarm_sig), &sig);
}

void *cliente(void *arg){
    int cid = (int)arg;
    int i, n;
    struct periodic_info info;
    make_periodic(15000, &info)
    while (1) {
        pthread_mutex_lock(&recept);
        bzero(direction[cid],sizeof(direction[cid]));
        n = read(newsockfd[cid],direction[cid],2);
        pthread_mutex_unlock(&recept);
        pthread_mutex_lock(&mutex);
            for (i = 0;i < 2; i++){
                if(write(newsockfd[i],buffer,T_BUFF)<0)
                    printf("Erro lendo do socket!\n");
            }
        pthread_mutex_unlock(&mutex);
    wait_period(&info);
    }
}

void *conn(void *arg) {
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    int sockfd,i;
    pthread_t t[2];

    sigset_t alarm_sig;
    sigemptyset (&alarm_sig);
    sigaddset (&alarm_sig, SIGALRM);
    sigprocmask(SIG_BLOCK, &alarm_sig, NULL);

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
