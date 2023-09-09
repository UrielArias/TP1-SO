CC= gcc
CFLAGS= -std=c99 -Wall -pedantic -fsanitize=address
EXEC = 
all: application slave view

application: application.c
	$(CC) $(CFLAGS) -o app application.c 

slave: slave.c
	$(CC) $(CFLAGS) -o slave slave.c 

view: view.c
	$(CC) $(CFLAGS) -o view view.c 

test: 
	make all
	cppcheck --quiet --enable=all --force --inconclusive .
	pvs-studio-analyzer trace -- make
	pvs-studio-analyzer analyze
	plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3' -t tasklist -o report.tasks PVS-Studio.log
	valgrind --leak-check=full ./app

cleanAll: cleanView cleanApplication cleanSlave
cleanView:
	rm -f view
cleanApplication:
	rm -f md5
cleanSlave:
	rm -f slave