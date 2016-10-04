
all: modbus_read

clean:
	rm -f *.o modbus_read

BINDIR  = /usr/bin

modbus_read:  modbus_read.c
	gcc -o modbus_read  modbus_read.c -lmodbus
install:	
	install -m755 -p modbus_read $(DESTDIR)$(BINDIR)/
	ln -f -s $(DESTDIR)$(BINDIR)/modbus_read $(DESTDIR)$(BINDIR)/modbus_write
uninstall:
	rm -f $(DESTDIR)$(BINDIR)/modbus_read $(DESTDIR)$(BINDIR)/modbus_write
