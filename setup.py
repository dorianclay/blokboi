from setuptools import Extension, setup

USE_CYTHON = True

ext = ".pyx" if USE_CYTHON else ".cpp"

source_files = [
    "lib/cgame" + ext,
    "lib/block.cpp",
    "lib/game_manager.cpp",
    "lib/game.cpp",
    "lib/player_controller.cpp",
    "lib/player.cpp",
    "lib/scene.cpp",
]


extensions = [
    Extension(
        "cgame",
        source_files,
        #   include_dirs=["include/random",
        #                 "include/loguru"],
        libraries=["effolkronium/random", "loguru"],
        library_dirs=["include/random", "include/loguru"],
    )
]

if USE_CYTHON:
    from Cython.Build import cythonize

    extensions = cythonize(extensions, compiler_directives={"language_level": 3})

setup(name="blokboi", ext_modules=extensions)
