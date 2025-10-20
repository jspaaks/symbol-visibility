# symbol-visibility

Investigating how to control symbol visibility in C ABIs when building via CMake.

## CMake

The project has been initialized with a [CMakeLists.txt](CMakeLists.txt)-based
configuration for building with CMake:

```console
# change into the build directory
$ cd build/

# generate the build files
$ cmake ..

# build the project
$ cmake --build .

# install the project to <repo>/build/dist
$ cmake --install .

# run the program to see if it works
$ ./dist/bin/calculator
```

Should output something like:

```text
-- test compile definitions
   DEBUG compile definition has been defined.

-- test wether math library was linked
   sqrt(144) = 12.000000

-- test c2x / c23 features
   0 1 2 3 4

-- test own library
   divide(2, 3) = 0
   multiply(2, 3) = 6
```

The directory `src/operations/` contains two functions that shouldn't be part of the ABI:

   1. `static_plus_five`, a static function defined in `src/operations/divide.c`, whose scope is limited to the compilation unit `src/operations/divide.c`.
   2. `nonstatic_plus_three`, a nonstatic function defined in `src/operations/helpers.c`, that is local to the liboperations library.

If we momentarily comment out the "visibility" lines in `src/operations/CMakeLists.txt`

```cmake
#set_property(TARGET tgt_lib_operations PROPERTY C_VISIBILITY_PRESET hidden)
...
#set_property(TARGET tgt_lib_operations PROPERTY VISIBILITY_INLINES_HIDDEN YES)
```

Configure, build and install:

```console
$ cmake --fresh .. && cmake --build . && cmake --install .
```

Let's see what gets included in the ABI (when compiling on Linux with GCC):

```console
$ # grep ' F ' shows functions only for brevity:
$ objdump --syms ./dist/lib/liboperations.so | grep ' F '
0000000000001060 l     F .text  0000000000000000 deregister_tm_clones
0000000000001090 l     F .text  0000000000000000 register_tm_clones
00000000000010d0 l     F .text  0000000000000000 __do_global_dtors_aux
0000000000001110 l     F .text  0000000000000000 frame_dummy
0000000000001156 l     F .text  0000000000000013 staticfun_plus_five
0000000000001000 l     F .init  0000000000000000 _init
00000000000011a8 l     F .fini  0000000000000000 _fini
0000000000001169 g     F .text  0000000000000013 nonstaticfun_plus_three
0000000000001119 g     F .text  000000000000003d operations_divide
000000000000117c g     F .text  000000000000002b operations_multiply
```

From that, `staticfun_plus_five` is correctly listed as a local (`l`) function (`F`), while `nonstaticfun_plus_three`, `operations_multiply`, and `operations_divide` are listed as global (`g`) functions (`F`), despite `nonstaticfun_plus_three` not being part of the API in `./dist/include`.

This implies that you can now (incorrectly) do something like:

```console
$ python3
>>> import ctypes
>>> liboperations = ctypes.CDLL('<path to library>')
>>> liboperations.nonstaticfun_plus_three(7)
10
```

So let's correct that by recompiling without commenting out the visibility lines in `src/operations/CMakeLists.txt`:

```cmake
set_property(TARGET tgt_lib_operations PROPERTY C_VISIBILITY_PRESET hidden)
...
set_property(TARGET tgt_lib_operations PROPERTY VISIBILITY_INLINES_HIDDEN YES)
```

Configure, build and install:

```console
$ cmake --fresh .. && cmake --build . && cmake --install .
```

Let's see if that made a difference:

```console
$ # grep ' F ' shows functions only for brevity:
$ objdump --syms ./dist/lib/liboperations.so | grep ' F '
0000000000001040 l     F .text  0000000000000000 deregister_tm_clones
0000000000001070 l     F .text  0000000000000000 register_tm_clones
00000000000010b0 l     F .text  0000000000000000 __do_global_dtors_aux
00000000000010f0 l     F .text  0000000000000000 frame_dummy
0000000000001136 l     F .text  0000000000000013 staticfun_plus_five
0000000000001000 l     F .init  0000000000000000 _init
0000000000001149 l     F .text  0000000000000013 nonstaticfun_plus_three
0000000000001188 l     F .fini  0000000000000000 _fini
00000000000010f9 g     F .text  000000000000003d operations_divide
000000000000115c g     F .text  000000000000002b operations_multiply
```

Now `staticfun_plus_five` and `nonstaticfun_plus_three` are correctly listed as local (`l`) functions (`F`), while `operations_multiply` and `operations_divide` are correctly listed as global (`g`) functions (`F`), which is consistent with the API in `./dist/include`.

## Acknowledgements

_This project was initialized using [Copier](https://pypi.org/project/copier) and the [copier-template-for-c-projects](https://github.com/jspaaks/copier-template-for-c-projects)._

[![Copier](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/copier-org/copier/master/img/badge/badge-grayscale-inverted-border-orange.json)](https://github.com/copier-org/copier)
