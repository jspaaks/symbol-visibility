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

`src/operations/operations.c` defines a local function, `some_local_function`. Let's see if it's present in the ABI:

```
$ $ objdump -t ./dist/lib/liboperations.so

./dist/lib/liboperations.so:     file format elf64-x86-64

SYMBOL TABLE:
0000000000000000 l    df *ABS*  0000000000000000 crtstuff.c
0000000000001040 l     F .text  0000000000000000 deregister_tm_clones
0000000000001070 l     F .text  0000000000000000 register_tm_clones
00000000000010b0 l     F .text  0000000000000000 __do_global_dtors_aux
0000000000004008 l     O .bss   0000000000000001 completed.0
0000000000003e50 l     O .fini_array    0000000000000000 __do_global_dtors_aux_fini_array_entry
00000000000010f0 l     F .text  0000000000000000 frame_dummy
0000000000003e48 l     O .init_array    0000000000000000 __frame_dummy_init_array_entry
0000000000000000 l    df *ABS*  0000000000000000 division.c
0000000000000000 l    df *ABS*  0000000000000000 multiplication.c
0000000000000000 l    df *ABS*  0000000000000000 crtstuff.c
00000000000020f0 l     O .eh_frame      0000000000000000 __FRAME_END__
0000000000000000 l    df *ABS*  0000000000000000 
0000000000003e58 l     O .dynamic       0000000000000000 _DYNAMIC
0000000000004008 l     O .data  0000000000000000 __TMC_END__
0000000000004000 l     O .data  0000000000000000 __dso_handle
0000000000001000 l     F .init  0000000000000000 _init
0000000000002000 l       .eh_frame_hdr  0000000000000000 __GNU_EH_FRAME_HDR
00000000000010f9 l     F .text  0000000000000013 some_local_function
000000000000113c l     F .fini  0000000000000000 _fini
0000000000003fe8 l     O .got.plt       0000000000000000 _GLOBAL_OFFSET_TABLE_
0000000000000000  w      *UND*  0000000000000000 __cxa_finalize
000000000000110c g     F .text  0000000000000017 operations_divide
0000000000000000  w      *UND*  0000000000000000 _ITM_registerTMCloneTable
0000000000001123 g     F .text  0000000000000017 operations_multiply
0000000000000000  w      *UND*  0000000000000000 _ITM_deregisterTMCloneTable
0000000000000000  w      *UND*  0000000000000000 __gmon_start__
```

From that, `operations_multiply` and `operations_divide` are global (`g`) functions (`F`), whereas
`some_local_function` is a local (`l`) function (`F`).

## Acknowledgements

_This project was initialized using [Copier](https://pypi.org/project/copier) and the [copier-template-for-c-projects](https://github.com/jspaaks/copier-template-for-c-projects)._

[![Copier](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/copier-org/copier/master/img/badge/badge-grayscale-inverted-border-orange.json)](https://github.com/copier-org/copier)
