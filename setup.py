from distutils.core import setup
from Cython.Build import cythonize

setup(
    name="blokboi",
    ext_modules=cythonize('lib/*.pyx',
                          compiler_directives={
                              'language_level': 3,
                          },
                        #   include_path=['include/loguru',
                        #                 'include/random'
                        #   ]
    ),
)

# 'src/*.pyx', 