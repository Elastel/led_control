all: led_ctl

led_ctl: led_ctl.o i2c.o led.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -rf *.o led_ctl