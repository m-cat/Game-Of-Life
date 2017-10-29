PROGS = life
DEPS = src/*.c

all : $(PROGS)

life : $(DEPS)
	gcc $(DEPS) -lncurses -o $@

clean :
	rm -f *~ *\# $(PROGS)
