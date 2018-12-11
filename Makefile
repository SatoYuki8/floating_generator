HEAD=src/add_gen.h
GCC=gcc
#GCC=~/opt/gcc/bin/gcc #ver6.3.0
#GCC=~/opt/gnat-gpl-2017-x86_64-linux-bin/bin/gcc #ver6.3.1
OPTION=-Wall -lm -O2
OBJECT=src/fpadd_arg_check.o src/fpadd_sub_module.o src/fpadd_generator.o src/main.o src/generate_function.o src/fpadd_step_measurement.o

all:	bin/ comp

comp:	$(OBJECT)
	$(GCC) $^ -o bin/gen $(OPTION)

bin/:
	mkdir bin

src/fpadd_sub_module.o:	src/fpadd_sub_module.c $(HEAD)
src/fpadd_sub_module.c:
	$(GCC) -c $@ $(OPTION)

src/fpadd_arg_check.o:	src/fpadd_arg_check.c $(HEAD)
src/fpadd_arg_check.c:
	$(GCC) -c $@ $(OPTION)

src/fpadd_generator.o:	src/fpadd_generator.c $(HEAD)
src/fpadd_generator.c:
	$(GCC) -c $@ $(OPTION)

src/main.o:	src/main.c $(HEAD)
src/main.c:
	$(GCC) -c $@ $(OPTION)

src/generate_function.o:	src/generate_function.c $(HEAD)
src/generate_function.c:
	$(GCC) -c $@ $(OPTION)

src/fpadd_step_measurement.o:	src/fpadd_step_measurement.c $(HEAD)
src/fpadd_step_measurement.c:
	$(GCC) -c $@ $(OPTION)


clean:
	rm -rf *~ *.sfl src/*~

distclean:
	rm -rf *~ *.out src/*.o src/*~ *.sfl bin/*
