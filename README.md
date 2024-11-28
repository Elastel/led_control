# led_control
led control for EG500

# Get started
$ git clone https://github.com/Elastel/led_control.git

$ cd led_control

$ make clean && make

$ ./led_ctl --help

led_ctl -m < system | wifi | alarm | online | sig > -s <on | off | stop | none | low | mid | high>

# eg. LTE led
# No network | CSQ = 99
$ ./led_ctl -m sig -s none

# < -105dBm | CSQ < 11
$ ./led_ctl -m sig -s low

# < -85dBm, > -105dBm | 11 < CSQ < 18
$ ./led_ctl -m sig -s mid

# > -85dBm | 18 < CSQ < 31
$ ./led_ctl -m sig -s high
