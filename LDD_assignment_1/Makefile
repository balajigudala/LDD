obj = char_driver
app = app_char_driver.c
obj-m += char_driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
	@sudo insmod $(obj).ko
	@lsmod 
	@sleep 10
	#clear
	@sudo cat /proc/devices 
	@sleep 5
	#clear
	@cc $(app) -o app
	@sudo ./app
	@ls -la

clean:
	@rm -f app
	@lsmod
	@sleep 10
	@sudo cat /proc/devices
	@sleep 5
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
	@sudo rmmod $(obj)
	@ls -la

