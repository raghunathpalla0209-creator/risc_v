# risc_v
This is for the coding challenge for risc_v framework enablement
This project implements a UART CLI communication tool that interacts with external devices (e.g., Arduino) over serial interfaces.
It demonstrates low-level communication concepts useful in embedded systems, Linux device handling, and RISC-V environments.

# Features
UART communication via CLI
Configurable device and baud rate
Send and receive messages
Debug logging support
Works with devices like Arduino

# Help Command

# make -> build the code 

# execution 
./uart_app --help
Output
UART CLI Tool
Usage:
  ./uart_app -d <device> -s <baudrate> "message"

Options:
  -d <device>     UART device (e.g., /dev/ttyACM0)
  -s <baudrate>   Baud rate (e.g., 9600, 115200)
  -h, --help      Show this help message

Example:
  ./uart_app -d /dev/ttyACM0 -s 9600 "test"



./uart_app -d /dev/ttyACM0 -s 9600 "test"

Device : /dev/ttyACM0
Baud   : 9600
Message: test
RX:
DEBUG RX: test
hello from arduino!!


  


  
