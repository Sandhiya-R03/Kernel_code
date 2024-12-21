# Example Linux Kernel Module

## Overview
This project demonstrates a comprehensive Linux kernel module written in C++. The module integrates with the kernel as a character device driver and provides additional interaction via the `/proc` filesystem.

## Features
- Registers a character device driver with basic read and write operations.
- Provides `/proc` entry for user-space interaction.
- Extensive logging for educational purposes.
- Counts device usage (open, close, read, and write operations).

## Requirements
- A Linux system with kernel headers installed.
- GCC compiler.
- Basic understanding of kernel development.

## Files
1. **example_module.cpp**: The main source code for the kernel module.
2. **Makefile**: A script to build the kernel module.

## Installation
1. Clone this repository or copy the files to your local system.
2. Navigate to the project directory.
3. Compile the kernel module:
   ```bash
   make
   ```

## Usage
1. Load the module into the kernel:
   ```bash
   sudo insmod example_module.ko
   ```
2. Check kernel logs to verify successful loading:
   ```bash
   dmesg | tail
   ```
3. Interact with the character device:
   - Identify the device file:
     ```bash
     ls /dev/example_char_device
     ```
   - Read from the device:
     ```bash
     cat /dev/example_char_device
     ```
   - Write to the device:
     ```bash
     echo "Hello, Kernel!" > /dev/example_char_device
     ```
4. Interact with the `/proc` entry:
   - Read from the proc entry:
     ```bash
     cat /proc/example_proc
     ```
   - Write to the proc entry:
     ```bash
     echo "Updating proc entry" > /proc/example_proc
     ```
5. Unload the module from the kernel:
   ```bash
   sudo rmmod example_module
   ```
6. Clean up the build files:
   ```bash
   make clean
   ```

## Output Examples
### Kernel Logs:
- On loading the module:
  ```
  ExampleModule: Module initialized successfully
  ExampleModule: Registered with major number 245
  ```
- On interacting with the device:
  ```
  ExampleModule: Device opened (1 times)
  ExampleModule: Wrote 13 bytes (1 writes so far)
  ExampleModule: Read 13 bytes (1 reads so far)
  ```
- On unloading the module:
  ```
  ExampleModule: Module unloaded
  ```

## Limitations
- The device buffer size is limited to 1024 bytes.
- The `/proc` entry buffer is also limited to 1024 bytes.
- Designed for educational purposes; not for production use.

## Future Improvements
- Add support for asynchronous I/O.
- Implement more complex kernel interactions, such as interrupts or timers.
- Provide user-space utilities for easier interaction.

## Disclaimer
This project is for educational purposes only. Use it in a controlled environment such as a virtual machine, as kernel modules run with high privileges and can destabilize the system if misused.

## License
This project is licensed under the GPL (GNU General Public License).

