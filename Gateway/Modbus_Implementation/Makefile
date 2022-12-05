
all: main.c modbus.c rs485.c device_functions.c
	gcc main.c modbus.c rs485.c device_functions.c -o modbus_implementation -pthread

debug:
	gcc main.c modbus.c rs485.c device_functions.c -o modbus_implementation_DEBUG -pthread -DDEBUG=1

rs485_test: rs485_test.c rs485.c
	gcc rs485_test.c rs485.c device_functions.c -o rs485_test 
