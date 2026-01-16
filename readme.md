
# about 
tiny example of UART communication for sending commands and telemetry (RXTX) using STM32 and RTOS

- RX - connected to PA9
- TX - connected to PA10
- PA11 - connected to LED (for debugging) 
- using 38400 baudrate 

# requirements 
- stm32f103c8t6
- usb-ttl adapter 


# usage
to setup the project, clone it recursively (clones all submodules)
```bash
git clone --recursive https://github.com/L0puh/uart-commander
```
then make the libs and source code
```bash
cd libs
make 
cd ..
make
make flash
```
check uart with any serial monitoring tool (i.e. `minicom`, use
`minirc.my_uart` config file)

