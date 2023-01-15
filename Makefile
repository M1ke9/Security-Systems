all: rsa.o logger acmonitor test_aclog

rsa.o: rsa.c
	gcc -Wall -g -c rsa.c

logger: logger.c
	gcc -Wall -g -fPIC -shared rsa.o -o logger.so logger.c -lcrypto -ldl -lm -lgmp

acmonitor: acmonitor.c 
	gcc rsa.o acmonitor.c -o acmonitor -lm -lgmp

test_aclog: test_aclog.c 
	gcc test_aclog.c -o test_aclog

run: logger.so test_aclog
	LD_PRELOAD=./logger.so ./test_aclog

clean:
	rm -rf rsa.o
	rm -rf logger.so
	rm -rf test_aclog
	rm -rf acmonitor
	
