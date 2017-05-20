TARGET=smtp-client
CFLAGS=-Wall

all: $(TARGET)

clean:
	rm -f $(TARGET)


$(TARGET): $(TARGET).c

archive: Makefile README $(TARGET).c
	tar cvzf RELEASES/smtp-client.tar.gz Makefile README $(TARGET).c
