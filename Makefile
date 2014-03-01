PROGS = life
DEPS = life.c util.c

all : $(PROGS)

life : $(DEPS)
	gcc $^ -lncurses -o $@

clean :
	rm -f *~ *# $(PROGS)
