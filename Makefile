OBJECTS = tokenizer.c history.c

tokenizer: $(OBJECTS)
	gcc -o $@ $^
