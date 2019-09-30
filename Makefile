CC = gcc

CFLAGS += -g -O0 -std=gnu11 -Wall -Wextra -fPIC
LIB_LDFLAGS += -shared

MOD = module rm trackdeps ; module load gcc

SRC = fake.c
LIBS_OBJ = fake.o
SONAME = 1
LIBS = libfake.so.${SONAME}

all:	${LIBS}

%.o:	%.c
	${MOD} ; ${CC} -o $@ -c ${CFLAGS} ${CPPFLAGS} ${EXTRA_FLAGS} $<

lib%.so.${SONAME}:	%.o
	${MOD} ; ${CC} -o $@ -Wl,-soname,$@ ${LDFLAGS} ${LIB_LDFLAGS} $< ${LDADD}

clean:
	@rm -f ${LIBS_OBJ} ${LIBS}

.PHONY:	all clean
