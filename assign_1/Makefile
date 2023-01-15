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