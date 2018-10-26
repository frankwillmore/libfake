CC ?= cc

CFLAGS += -g -O0 -std=gnu99 -Wall -Wextra -fPIC
LIB_LDFLAGS += -shared

SRC = fake.c
LIBS_OBJ = fake.o
SONAME = 1
LIBS = libfake.so.${SONAME}

all:	${LIBS}

%.o:	%.c
	${CC} -o $@ -c ${CFLAGS} ${CPPFLAGS} ${EXTRA_FLAGS} $<

lib%.so.${SONAME}:	%.o
	${CC} -o $@ -Wl,-soname,$@ ${LDFLAGS} ${LIB_LDFLAGS} $< ${LDADD}

clean:
	@rm -f ${LIBS_OBJ} ${LIBS}

.PHONY:	all clean
