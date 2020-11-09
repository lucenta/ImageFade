CFLAGS = -Xpreprocessor -fopenmp -lomp
OBJS = main.o ppmFile.o
ppmfilt: $(OBJS)	
	clang $(CFLAGS) -o $@ $?
clean:
	rm -rf $(OBJS) ppmfilt