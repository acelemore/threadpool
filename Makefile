objects = main3.o condition.o threadpool.o

a.out:$(objects)
	gcc $(objects) -pthread -o a.out

$(objects):common.h

.PHONY:clean

clean:
	rm a.out $(objects)
