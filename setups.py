from distutils.core import setup, Extension

setup(name='cserver', version='1.0',ext_modules=[Extension("cserver", ['cserver.c'])])
