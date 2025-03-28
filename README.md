# HW-RoT

## Introduction

This is the repository of the HW Root-of-Trust developed for the work: "Enhanced Security Features on Embedded Systems using a Hardware Root of Trust"

## Description

The content of the HW-ROT library is depicted in the next container tree:
    
    .
    ├── hw-rot            # folder that contains all the files of the HW-RoT.
        .
        ├── bit             # folder to store the bitstream files
        ├── build           # folder to store the shared libraries 
        └── rtl             # folder that contains the RTL sources files
            .
            ├── common      # common files 
            ├── sha3        # SHA3 files 
            ├── sha2        # SHA2 files 
	        ├── eddsa       # EdDSA files
	        ├── x25519      # X25519 files
            ├── trng        # TRNG files
            ├── AES         # AES files
            └── PUF         # PUF files 	    	
        └── src             # folder that contains the sources files of the library
            .
            ├── common      # common files 
            ├── sha3        # SHA3 files 
	        ├── sha2        # SHA2 files 
            ├── eddsa       # EdDSA files
	        ├── x25519      # X25519 files
            ├── trng        # TRNG files
            ├── AES         # AES files
            └── PUF         # PUF files 
    ├── demo                # folder that contains the demo
    ├── hw-rot.h            # header of the library
    ├── Makefile            # To compile the library
    ├── ip_hw_rot_1_0.zip    # The IP Module of the HW RoT
    └── README.md  

## Installation

### Makefile Configuration

The HW-RoT library is ready to perform the communication to the hardware through two different interfaces: AXI-Lite and I2C. All this implementation has been done through the `INTF` variable into the code. 
To select this configuration during the compilation process, it is ***mandatory*** to change the variable `INTERFACE` (`AXI` or `I2C`) and `BOARD` (`ZCU104` or `PYNQZ2`). If `INTERFACE = I2C`, then the variable `BOARD` is not applied.

## Demo FPL
The `demo_fpl.c` is the specific demo for the FPL-2025. It shows the 4 security enhances presented in the work. To compile, go to section `demo` and `make demo-fpl`. 

### Other demos

It has been implemented different type of demo:
- `demo`: the basic demo is working just showing the functionality of the HW-RoT. It will return a ✅ in case the implemented algorithm is working properly or ❌ in other case.
- `demo-speed`: The results will show performance in term of Elapsed Time (ET) of each cryptograhic algorithm. 

For the use type `make XXX-YYY` where `XXX` and `YYY` can be: 

| XXX                   | Meaning   |
| ----------            | --------- |
| demo                  | Functionality Demo                                        |
| demo-speed            | Execution Time Demo                                       |

It is possible to change the behaviour of demo with the file ```config.conf```. The variables ```SHA-2```, ```SHA-3```, etc. represent the type of algorithm to be tested. If ```1``` is set the test will be performed, while a ```0``` will point out that this test won't be performed. The variable ```N_TEST``` set the number of test to be performed to calculate the average execution time.  

For any demo it is possible to type `-v` or `-vv` for different verbose level. For example, `./demo -vv`. *We do not recommend that for long test.*  