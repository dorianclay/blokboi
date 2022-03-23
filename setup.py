from numpy import True_
from setuptools import Extension, setup

USE_CYTHON = True

ext = ".pyx" if USE_CYTHON else ".c"

extensions = [Extension("cgame", ["lib/cgame"+ext])]

if USE_CYTHON:
    from Cython.Build import cythonize
    extensions = cythonize(extensions, compiler_directives={'language_level':3})

setup(
    name="blokboi",
    ext_modules=extensions
)