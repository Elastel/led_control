# led_control
led control for EG500

# Get started
$ git clone https://github.com/Elastel/led_control.git

$ cd led_control

$ make clean && make

$ ./led_ctl --help

led_ctl -m < system | wifi | alarm | online | sig > -s <on | off | stop | none | low | mid | high>

# eg. LTE led
# signal = 99 (no network)
$ ./led_ctl -m sig -s none

# signal <= 11
$ ./led_ctl -m sig -s low

# signal > 11 && signal < 18
$ ./led_ctl -m sig -s mid

# signal >= 18 && signal <= 31
$ ./led_ctl -m sig -s high
