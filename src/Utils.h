#ifndef UTILS_H
#define UTILS_H

#define ERR(...) fprintf(stderr, __VA_ARGS__)

#define Assert(expr)                                                      \
    do {                                                                  \
        if (!(expr)) {                                                    \
            ERR("%s:%d (%s): Assert failed with the expresion \"%s\".\n", \
                  __FILE__, __LINE__, __func__, #expr);                   \
            __debugbreak();                                               \
        }                                                                 \
    } while(0)

#define SWAP(var1, var2, type) \
    do {		               \
        type temp = (var1);    \
		(var1) = (var2);       \
		(var2) = temp;         \
    } while (0);

#define NO_DEFAULT default: { Assert(false && "There should not be a default."); } break;

#define FieldOffset(structType, field) ((WordSize)&(((structType *)0)->field))

#define foreach(type, name, array)                                          \
    for (type *name = (array).A; name < ((array).A + (array).size); ++name)

// defer
#define defer(retVal) do { ret = (retVal); goto defer; } while(0)

#define INFINITE_LOOP 1

#endif //UTILS_H
