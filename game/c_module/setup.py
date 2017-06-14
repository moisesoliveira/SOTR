from distutils.core import setup, Extension

setup(name='cmodule', version='1.0',
      ext_modules=[Extension("cmodule", ['cmodule.c'])])
