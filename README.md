# led_control
led control for EG500

$ git clone https://github.com/Elastel/led_control.git

$ cd led_control

$ make clean && make

$ ./led_ctl --help
led_ctl -m < system | wifi | alarm | online | sig > -s <on | off | stop | none | low | mid | high>
