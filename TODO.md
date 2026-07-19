# malloc

Here's a little to-do list for this project

### Optimization
- [] Improve realloc with coalesce
- [] Update TINY_MAX and SMALL_MAX (32 or 64 bits)
- [X] Use __attribute_((constructor((101)))) for init_env()

### Other
- [] Fix calloc history overflow

- [X] Regler le pb du free -> done
- [] Regler le pb du printf -> faire le sien.
- [] Regler le pb du realloc -> passage d'une zone a une autre.
- [X] Constructor (init_env) + Destructor (munmap tiny + small)
