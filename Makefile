CC=/usr/bin/cc

CFLAGS= -I$(IDIR) -std=iso9899:1999 -pedantic -pedantic-errors -m64 -g \
	-O0 -march=k8 -mtune=k8 -Wl,-rpath=/usr/local/lib,--enable-new-dtags \
   	-fno-builtin -malign-double -mpc80

CPPFLAGS=-D_TS_ERRNO -D_POSIX_PTHREAD_SEMANTICS -D_LARGEFILE64_SOURCE \
		-D_X_OPEN_SOURCE=600 -D_REENTRANT

VPATH = src

LIBS = -lpthread -lopenal

OBJECTS = $(addprefix obj/, irc.o log.o main.o on_err.o random.o sound.o tcp.o) 

obj/%.o: %.c
	@mkdir -p obj
	$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

bruhbot: $(OBJECTS)
	$(CC) -o $@ $^ -L$(LDIR) $(CFLAGS) $(CPPFLAGS) -D_REENTRANT $(LIBS)

clean:
	rm -rf obj bruhbot
