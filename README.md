# Useful C Constructs #

A collection of C files with useful functions and macros for general use.
The directory tree also mirrors how I'd typically structure
a medium-sized project.

Eventually the goal is to have everything be C89-compliant and non-POSIX
portable. That could be a while...

## dbg.h

Helpful C debugging macros, courtesy of Zed Shaw's book "Learn C the Hard Way."

## minunit.h

Minimalist unit-testing framework for C files.

## utils.h

Helpful utilities for common tasks (MIN, MAX, ABS, etc.)

## dequeu.c/h

Doubly-linked list data structure.

## TODO

Other files to add when I have time:
- fileio.c - handy c89 compliant file IO functions
- network.c - generic TCP/UDP client/server code
- str_manip.c - common string manipulation functions