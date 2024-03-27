sym: main.c sym.c ht.c stack.c
	${CC} -g -o $@ $^

clean:
	rm -rf sym

.PHONY: clean
