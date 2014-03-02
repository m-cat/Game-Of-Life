PROGS = life
DEPS = life.c util.c

all : $(PROGS)

life : $(DEPS)
	gcc $(DEPS) -lncurses -o $@

clean :
	rm -f *~ *\# $(PROGS)
