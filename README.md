# sheeplib

This repository is collection of my Single Header Library for C,
None of these library depend on each other

- log.h:
    nice interface for logging and debugging
    warn("Example warn %d", 105);
        
- dynarray.h 
    dynamic array for all type in C
    by allocating small extra memory in the 
    front of array containing information
    about the dynarray

- xmalloc.h 
    just normal x-wrapper function,
    which might be considered harmful by many

- str.h 
    C-string is disaster, trying to fix that
