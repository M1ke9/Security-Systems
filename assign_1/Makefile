CC = gcc
DBUG = -g
CCFLAGS = -O2 -Wall -pedantic
OBJFILES1 = DifHellmann.c DifHellman.h MainDifHellman.c
OBJFILES2= RSA.c RSA.h MainRSA.c 

TARGET1 = dh_assign_1
TARGET2 = rsa_assign_1


all: $(TARGET1)
all: $(TARGET2)

$(TARGET1): $(OBJFILES1)
	$(CC) $(CFLAGS) $(DBG) -o $(TARGET1) $(OBJFILES1) -lm

$(TARGET2): $(OBJFILES2)
	$(CC) $(CFLAGS) $(DBG) -o $(TARGET2) $(OBJFILES2) -lm -lgmp

clean:
	rm -f $(TARGET) *.o
	rm -f $(TARGET) *.o
	
	
TARGET1=client
TARGET2=server

CC=gcc


CFLAGS1= -Wall -o client slient.c  
CFLAGS2= -Wall -o server server.c 

normal:	$(TARGET1)
	client: client.c
	$(CC) $(CFLAGS1)  -o client.c -L/usr/lib -lssl -lcrypto


normal:	$(TARGET2)
server: server.c
	$(CC) $(CFLAGS2) -o server.c -L/usr/lib -lssl -lcrypto
clean:
	$(RM) $(TARGET1)
	$(RM) $(TARGET2)

