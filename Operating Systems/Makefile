#Projeto realizado por:
# - Andre Oliveira - ist193686
# - Pedro Xavier - ist193747
# Turno: Segunda-feira 16h


# Makefile, versao 1
# Sistemas Operativos, DEI/IST/ULisboa 2019-20

SOURCES = main.c fs.c sema.c tecnicofs-client-api.c
SOURCES+= lib/*.c
OBJS_NOSYNC = $(SOURCES:%.c=%.o)
OBJS_MUTEX  = $(SOURCES:%.c=%-mutex.o)
OBJS_RWLOCK = $(SOURCES:%.c=%-rwlock.o)
OBJS = $(OBJS_NOSYNC) $(OBJS_MUTEX) $(OBJS_RWLOCK)
CC   = gcc
LD   = gcc
CFLAGS =-Wall -std=gnu99 -I../ -g
LDFLAGS=-lm -pthread
TARGETS = tecnicofs

.PHONY: all clean

all: $(TARGETS)

$(TARGETS):
	$(LD) $(CFLAGS) $^ -o $@ $(LDFLAGS)

tecnicofs-client-api.o: tecnicofs-client-api.c tecnicofs-client-api.h
lib/inodes.o: lib/inodes.c lib/inodes.h
lib/hash.o: lib/hash.c lib/hash.h
sema.o: sema.c sema.h
lib/inodes.o: lib/inodes.c lib/inodes.h

### RWLOCK ###
lib/bst-rwlock.o: CFLAGS+=-DRWLOCK
lib/bst-rwlock.o: lib/bst.c lib/bst.h

fs-rwlock.o: CFLAGS+=-DRWLOCK
fs-rwlock.o: fs.c fs.h lib/bst.h

main-rwlock.o: CFLAGS+=-DRWLOCK
main-rwlock.o: main.c fs.h lib/bst.h lib/hash.h lib/inodes.h sema.h lib/inodes.h tecnicofs-client-api.h
tecnicofs: lib/bst-rwlock.o fs-rwlock.o main-rwlock.o lib/hash.o lib/inodes.o sema.o lib/inodes.o tecnicofs-client-api.o


%.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo Cleaning...
	rm -f $(OBJS) $(TARGETS)
