CC= gcc
CFLAGS= -Wall -pedantic
all: md5 slave view

md5: application.c
	$(CC) $(CFLAGS) -o md5 sharedMemTAD.c communicationWithView.c communicationWithSlaves.c errorHandler.c application.c 

slave: slave.c
	$(CC) $(CFLAGS) -o slave errorHandler.c slave.c 

view: view.c
	$(CC) $(CFLAGS) -o view sharedMemTAD.c errorHandler.c view.c

cleanAll: cleanView cleanApplication cleanSlave cleanResult 
cleanView:
	rm -f view
cleanApplication:
	rm -f md5 
cleanSlave:
	rm -f slave
cleanResult:
	rm -f result.txt
