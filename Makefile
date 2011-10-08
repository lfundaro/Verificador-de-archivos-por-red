GCC = gcc
OPS = -g
OBJS = main.o fetcher.o entry_list.o parser.o differ.o updater.o printer.o utils.o

all: $(OBJS) data_structures.h Makefile
	$(GCC) $(OPS) $(LINK_OPS) $(OBJS) -o verific

main.o : main.c
	$(GCC) $(OPS) -c main.c

dispatcher: dispatcher.c dispatcher.h
	$(GCC) $(OPS) -c dispatcher.c

fetcher.o: fetcher.c fetcher.h
	$(GCC) $(OPS) -c fetcher.c

parser.o: parser.c parser.h
	$(GCC) $(OPS) -c parser.c

differ.o: differ.c differ.h
	$(GCC) $(OPS) -c differ.c

updater.o: updater.c updater.h
	$(GCC) $(OPS) -c updater.c

printer.o: printer.c printer.h
	$(GCC) $(OPS) -c printer.c

entry_list.o: entry_list.c entry_list.h
	$(GCC) $(OPS) -c entry_list.c

utils.o: utils.c utils.h
	$(GCC) $(OPS) -c utils.c

check-syntax:
	$(GCC) $(OPS) $(LINK_OPS) -o nul -S ${CHK_SOURCES}

clean:
	rm -rf givnaarial .\#* \#* *.o .*~ *~ *.gch
