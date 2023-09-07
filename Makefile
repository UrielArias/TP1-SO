CC= gcc
CFLAGS= -std=c99 -Wall -pedantic -fsanitize=address
all: application slave view

application: application.c
	$(CC) $(CFLAGS) -o md5 application.c 

slave: slave.c
	$(CC) $(CFLAGS) -o slave slave.c 

view: view.c
	$(CC) $(CFLAGS) -o view view.c 

cleanAll: cleanView cleanApplication cleanSlave
cleanView:
	rm -f view
cleanApplication:
	rm -f md5
cleanSlave:
	rm -f slave