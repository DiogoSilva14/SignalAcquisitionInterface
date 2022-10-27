
all: main.c modbus.c
	gcc main.c modbus.c rs485.c device_functions.c -o modbus_implementation

rs485_test: rs485_test.c rs485.c
	gcc rs485_test.c rs485.c device_functions.c -o rs485_test 