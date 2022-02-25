from distutils.core import setup
from Cython.Build import cythonize

setup(
    name="blokboi",
    ext_modules=cythonize('src/*.pyx',
                          compiler_directives={
                              'language_level': 3,
                          }),
)
