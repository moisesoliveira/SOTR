#include <stdio.h>
#include <Python.h>
#include <time.h>
#include <stdlib.h>


static PyObject *hello_hello(PyObject *self){
    printf("Hello!\n");
    Py_RETURN_NONE;
}

static PyObject *hello_add(PyObject *self, PyObject *args){
    int a;
    int b;
    printf("add running!\n"):
    if (!PyArg_ParseTuple(args, "ii", &a, &b))
        return NULL;
    a = a+100;
    return Py_BuildValue("i", a+b);
}


static PyMethodDef hello_methods[] = {
    {"hello", (PyCFunction) hello_hello, METH_NOARGS, NULL },
    {"add", (PyCFunction) hello_add, METH_VARARGS, NULL}
};

PyMODINIT_FUNC inithello(){
    Py_InitModule3("hello", hello_methods, "whatever");
}
