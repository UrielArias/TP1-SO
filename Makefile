CC= gcc
CFLAGS= -Wall -pedantic 
all: md5 slave view

md5: application.c
	$(CC) $(CFLAGS) -o md5 sharedMemTAD.c communicationWithView.c communicationWithSlaves.c application.c 

slave: slave.c
	$(CC) $(CFLAGS) -o slave slave.c 

view: view.c
	$(CC) $(CFLAGS) -o view sharedMemTAD.c view.c

cppcheck: 
	cppcheck --quiet --enable=all --force --inconclusive .

pvs-studio:
	pvs-studio-analyzer trace -- make
	pvs-studio-analyzer analyze
	plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3' -t tasklist -o report.tasks PVS-Studio.log

valgrind:
	valgrind --leak-check=full ./md5 ./* | ./view

testAll: cppcheck pvs-studio all valgrind

cleanAll: cleanView cleanApplication cleanSlave cleanResult cleanReports
cleanView:
	rm -f view
cleanApplication:
	rm -f md5 
cleanSlave:
	rm -f slave
cleanResult:
	rm -f result.txt
cleanReports:
	rm -f report.tasks