from distutils.core import setup, Extension

setup(name='cclient', version='1.0',ext_modules=[Extension("cclient", ['cclient.c'])])
from distutils.core import setup, Extension

setup(name='cserver', version='1.0',ext_modules=[Extension("cserver", ['cserver.c'])])
