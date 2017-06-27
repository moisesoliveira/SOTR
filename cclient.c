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
#include <Python.h>
#include <time.h>
#include <signal.h>
#include <errno.h>

#define T_BUFF 24

//Variaveis globais
int sockfd;
int portno;
char ipaddr[20] = "127.0.0.1";
char buffer[T_BUFF], direction[2];
int dir = 3;// direção enviada pelos controles, but int
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int inf[7];

//Tarefa responsavel por receber os dados do servidor
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

void *printstatus(void *arg){
//    int id, p1x, p2x, bx, by, s1,s2,chk;
    while(1){
        printf("\e[H\e[2J"); //printf() para limpar a tela
        printf("Client Monitor\n");
        printf("Conectado no ip:\t %s\n", &ipaddr);
        printf("Conectado na porta:\t %d\n", portno);
        printf("Posição do player 1:\t %d\n", inf[1]);
        printf("Posição do player 2:\t %d\n", inf[2]);
        printf("Posição da bola:\t %dx%d\n", inf[3],inf[4]);
        printf("Placar:\t\t\t %dx%d\n", inf[5], inf[6]);
        printf("comando enviado:\t %s\n", direction);
//        printf("buffer:\t\t %s\n", buffer);
        usleep(300);
    }
}

//tarefa responsável por toda a comunicação do cliente com o servidor
//aqui sao configurados os sockets e é feito o envio dos dados
void *client(void *arg) {
    int n;
    struct sockaddr_in serv_addr;
    pthread_t t, p;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Erro criando socket!\n");
        return -1;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
//    inet_aton(argv[1], &serv_addr.sin_addr);
//    inet_aton(&ipaddr, &serv_addr.sin_addr);
    serv_addr.sin_addr.s_addr = inet_addr(&ipaddr);
    printf("%s\n",inet_ntoa(serv_addr.sin_addr)); 
    serv_addr.sin_port = htons(portno);
    n = connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr));
    if (n < 0) {
        printf("Erro conectando!: %d\n", n);
        return -1;
    }
    //cria a thread para fazer a leitura do socket
    pthread_create(&t, NULL, leitura, NULL);
//    pthread_create(&p, NULL, printstatus, NULL);
    //e segue fazendo a escrita no socket
    //aqui pode ser feita uma tarefa periodica
    //mas nao pode deixar de ter um loop, pois é a tarefa principal
    do {
        bzero(direction,sizeof(direction));
        sprintf(direction, "%d",dir);
        if (send(sockfd,direction,2,0) == -1){
            printf("Erro escrevendo no socket!\n");
            exit(1);
        }
//        printf("Enviou %s\n",direction);
        usleep(1000);
    }while (1);
    close(sockfd);
    return 0;
}

//tarefa acessada através do python. é responsavel por inicializar o cliente
//o python nao pode acessar a tarefa "*client()", pois ela contem um loop
//e assim o programa ficaria travado durante sua execução
static PyObject *cclient_start(PyObject *self, PyObject *args){
    int size;
    char ip[20];
    pthread_t cli;
    if (!PyArg_ParseTuple(args,"si",&ip, &portno))
        return NULL;
    printf("cclient, conectando no endereço %s : %d\n",&ipaddr, portno);
    pthread_create(&cli, NULL, client, NULL);
    Py_RETURN_NONE;
}

//tarefa acessada pelo python
//ela somente converte os dados do buffer e retorna dados do tipo inteiro
static PyObject *cclient_pos(PyObject *self, PyObject *args){
    int i, head = 1, tail = 3;
    //int inf[7]; //id, p1x, p2x, bx, by, s1,s2; 
    char temp[5];
    pthread_mutex_lock(&mutex);
    strncpy (temp, buffer, 1);
    temp[1] = '\0';
    inf[0] = atoi(temp);
    /*converter o buffer para um vetor de numeros 
    inteiros contendo as posições dos objetos do jogo*/
    for (i=1;i<7;i++){
        strncpy (temp, buffer+head, 3);
        temp[4] = '\0';
        inf[i] = atoi(temp);
        head += 3;
        tail+=3;
    }
    pthread_mutex_unlock(&mutex);
    return Py_BuildValue("iiiiiii", inf[0], inf[1], inf[2], inf[3], inf[4], inf[5], inf[6]);
}

static PyObject *cclient_move(PyObject *self, PyObject *args){
    if (!PyArg_ParseTuple(args, "i", &dir))
        return NULL;
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
