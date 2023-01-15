CC = gcc
DBUG = -g
CCFLAGS = -Wall 
OBJFILES1 = client.c
OBJFILES2= server.c

TARGET1 = client
TARGET2 = server


all: $(TARGET1)
all: $(TARGET2)

$(TARGET1): $(OBJFILES1)
	$(CC) $(CFLAGS) $(DBG) -o client client.c -L/usr/lib -lssl -lcrypto

$(TARGET2): $(OBJFILES2)
	$(CC) $(CFLAGS) $(DBG) -o server server.c -L/usr/lib -lssl -lcrypto

clean:
	rm -f $(TARGET1) *.o
	rm -f $(TARGET2) *.o