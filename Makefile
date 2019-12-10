CC = gcc

CFLAGS += -g -O0 -std=gnu11 -Wall -Wextra -fPIC
#CFLAGS += -g -O0 -std=gnu99 -Wall -Wextra -fPIC
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

tests:	all test_getgrgid test_getgrgid_r

test_getgrgid: test_getgrgid.c
	${CC} ${CFLAGS} test_getgrgid.c -o test_getgrgid

test_getgrgid_r: test_getgrgid_r.c
	${CC} ${CFLAGS} test_getgrgid_r.c -o test_getgrgid_r

check-preload: tests
	LD_PRELOAD=`pwd`/libfake.so.1 ./test_getgrgid > test_preload_getgrgid.out 2>&1 
	LD_PRELOAD=`pwd`/libfake.so.1 ./test_getgrgid_r > test_preload_getgrgid_r.out 2>&1 

check-no-preload: tests
	LD_PRELOAD="" ./test_getgrgid > test_getgrgid.out 2>&1
	LD_PRELOAD="" ./test_getgrgid_r > test_getgrgid_r.out 2>&1

check-both: check-preload check-no-preload 
	diff test_preload_getgrgid.out test_getgrgid.out
	diff test_preload_getgrgid_r.out test_getgrgid_r.out
	echo "Preload and original outputs match."

clean:
	@rm -f ${LIBS_OBJ} ${LIBS} *.out test_getgrgid test_getgrgid_r

.PHONY:	all clean check-preload check-no-preload check-both tests
