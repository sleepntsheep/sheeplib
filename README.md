# sheeplib

This repository is collection of my Single Header Library for C,
None of these library depend on each other.

These library support C99, but may not support C++.

- log.h
    : nice interface for logging and debugging
    warn("Example warn %d", 105);
        
- dynarray.h 
    : dynamic array for all type in C
    by allocating small extra memory in the 
    front of array containing information
    about the dynarray

- algo.h 
    : mergesort, binarysearch, upperbound, lowerbound

- xmalloc.h 
    : just normal x-wrapper function,
    which might be considered harmful by many

- str.h 
    : C-string is disaster, trying to fix that

- sjson.h
    : json serializer, deserializer

# installation:

simply copy header.h to your project tree.

# usage:

you will need to define SHEEP_HEADER_IMPLEMENTATION before using it
(this applys even if you are using shared library)

    #define SHEEP_ALGO_IMPLEMENTATION
    #include <sheeplib/algo.h>

for further information, read the document in header file itself

# instructions

these are single header library, just copy it to your project.
and read the comments in the library for further instructions.

# test

compile and run test.c with C11 or later compiler

