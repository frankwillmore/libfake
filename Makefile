CC = gcc

#CFLAGS += -g -O0 -std=gnu11 -Wall -Wextra -fPIC
CFLAGS += -g -O0 -std=gnu99 -Wall -Wextra -fPIC
LIB_LDFLAGS += -shared

MOD = module rm trackdeps ; module load gcc

SRC = fake.c
LIBS_OBJ = fake.o
SONAME = 1
LIBS = libfake.so.${SONAME}

all:	${LIBS}

%.o:	%.c
#	${MOD} ; ${CC} -o $@ -c ${CFLAGS} ${CPPFLAGS} ${EXTRA_FLAGS} $<
	${CC} -o $@ -c ${CFLAGS} ${CPPFLAGS} ${EXTRA_FLAGS} $<

lib%.so.${SONAME}:	%.o
#	${MOD} ; ${CC} -o $@ -Wl,-soname,$@ ${LDFLAGS} ${LIB_LDFLAGS} $< ${LDADD}
	${CC} -o $@ -Wl,-soname,$@ ${LDFLAGS} ${LIB_LDFLAGS} $< ${LDADD}

tests:	test_getgrgid test_getgrgid_r

test_getgrgid: test_getgrgid.c
	${CC} ${CFLAGS} test_getgrgid.c -o test_getgrgid

test_getgrgid_r: test_getgrgid_r.c
	${CC} ${CFLAGS} test_getgrgid_r.c -o test_getgrgid_r

check-preload:tests
	export LD_PRELOAD=`pwd`/libfake.so.1 && ./test_getgrgid
	export LD_PRELOAD=`pwd`/libfake.so.1 && ./test_getgrgid_r

check-no-preload: tests
	unset LD_PRELOAD && ./test_getgrgid
	unset LD_PRELOAD && ./test_getgrgid_r

check-both: check-preload check-no-preload

clean:
	@rm -f ${LIBS_OBJ} ${LIBS}

.PHONY:	all clean check-preload check-no-preload check-both
