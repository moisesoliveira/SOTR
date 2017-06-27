from distutils.core import setup, Extension

setup(name='cserver', version='1.0',ext_modules=[Extension("cserver", ['cserver.c'], extra_link_args=['-lrt', '-pthread'])])
setup(name='cclient', version='1.0',ext_modules=[Extension("cclient", ['cclient.c'], extra_link_args=['-lrt', '-pthread'])])


