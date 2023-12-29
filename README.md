# Zephyr Application For NPCX4_Demo

This repository contains a Zephyr NPCX4 demo application. This repository contains plenty of Zephyr applications which main purpose is to validate current NPCX4 EC.

## Getting Started

Before getting started, make sure you have a proper Zephyr development
environment. You can follow the official
[Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html).

### Setting Up Zephyr environment

Refer to the official [Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html). while reviewing considerations below during each step.

***ECFW_DEMO has been successfully build on Windows 10/11 x64 and Ubuntu systems.***

#### Install dependencies

Steps 2.1 and 2.2 from Zephyr guide are mandatory.

#### Get Zephyr and Python dependencies

Steps 3.1, 3.2, 3.3, 3.4, 3.5 and 3.7 from Zephyr guide are mandatory. This will retrieve a copy Zephyr RTOS and install all python dependencies.

#### Install Zephyr SDK and toolchain

Steps 4.1, 4.2 and 4.3 from Zephyr guide are mandatory. Step 4.4 is only required if flashing HW via command line instead of using Dediprog.

### Getting EC FW framework code

#### Clone demo repository from nuvoton

Create a folder sandbox and clone ecfw_demo project

```shell
mkdir ~/sandbox
cd ~/sandbox
git clone https://github.com/Nuvoton-EC-FAE/ecfw_demo
```

#### Obtain the dependencies

Navigate to west manifest location inside ecfw_demo and re-initialize west

```shell
cd ecfw_demo
west init -l
```

Retrieve all external project dependencies

```shell
cd ..
west update
```

### Build & Run

#### Build EC FW

Go back to ecfw_demo main folder and build the application and all its dependencies. Below commands indicate details for each board. See Supported hardware section for more details.

```shell
cd ecfw_demo
# Building for NPCX4mnx 
west build -p always -b npcx4mnx_demo app/demo
```

#### Flash EC FW

Utilize the ***Flash Update Tool*** and ***CR-Eclipse*** for Flashing the EC and Debugging