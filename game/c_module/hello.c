#include <stdio.h>
#include <Python.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#define T_BUFF 24
int varglobal=0;
char buffer[T_BUFF];
pthread_t t;

void *testando(void *arg){
    int i;
    int buff[T_BUFF];
    while(1){
        for(i=0;i<T_BUFF;i++){
            buff[i] = buff[i];
        }
        printf("\e[1;1H\e[2J");
        printf("O buffer é: ");

        for(i=0;i<T_BUFF;i++){
            printf("%c", buffer[i]);
        }
        printf("\n");
    }
}

static PyObject *hello_hello(PyObject *self){

    printf("Hello, estou chamando uma thread!\n");
    pthread_create(&t, NULL, testando, NULL);
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
