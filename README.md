# Zephyr Application For NPCX4_Demo

This repository contains a Zephyr NPCX4 demo application. This repository contains plenty of Zephyr applications which main purpose is to validate current NPCX4 EC.

## Getting Started

Before getting started, make sure you have a proper Zephyr development
environment. You can follow the official
[Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html).

### Setting Up Zephyr environment

Refer to the official [Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html). while reviewing considerations below during each step.

***ECFW_DEMO has been successfully build on Windows 10/11 x64 and Ubuntu systems.***

#### [1. Install dependencies](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#install-dependencies)

Install some host dependencies using your package manager.
Dependencies minimum required:
```
Cmake: 3.20.5
Python: 3.8
Devicetree compiler: 1.4.6
```

#### [2. Get Zephyr and Python dependencies](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#get-zephyr-and-install-python-dependencies)

This will retrieve a copy Zephyr RTOS and install all python dependencies.

```
sudo apt install --no-install-recommends git cmake ninja-build gperf \
  ccache dfu-util device-tree-compiler wget \
  python3-dev python3-pip python3-setuptools python3-tk python3-wheel xz-utils file \
  make gcc gcc-multilib g++-multilib libsdl2-dev libmagic1
```

> **Note:** For the Windows environment, please refer to the guide for operations on Windows.

#### [3. Install Zephyr SDK and toolchain](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#install-the-zephyr-sdk)

The Zephyr Software Development Kit (SDK) contains toolchains for each of Zephyr’s supported architectures, which include a compiler, assembler, linker and other programs required to build Zephyr applications.

```
# Download and verify the Zephyr SDK bundle
cd ~
wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.4/zephyr-sdk-0.16.4_linux-x86_64.tar.xz

# Extract the Zephyr SDK bundle archive:
tar xvf zephyr-sdk-0.16.4_linux-x86_64.tar.xz

#Run the Zephyr SDK bundle setup script:
cd zephyr-sdk-0.16.4
./setup.sh
```

> **Note:** For the Windows environment, please refer to the guide for operations on Windows.

### Getting EC FW framework code

#### Clone demo repository from nuvoton

Create a folder sandbox and clone ecfw_demo project

```
mkdir ~/sandbox
cd ~/sandbox
git clone https://github.com/Nuvoton-EC-FAE/ecfw_demo
```

#### Obtain the dependencies

Navigate to west manifest location inside ecfw_demo and re-initialize west

```
cd ecfw_demo
west init -l
```

Retrieve all external project dependencies

```
cd ..
west update
```

#### Switch different version supports under ecfw_demo

```
# E.g. Switching from version v3.2 to v3.5
cd ecfw_demo
git checkout v3.5
# Retrieve all external project dependencies
cd ..
west update
```

> **Note:** To switch to npcx-v3.6-branch, refer to the configuration in west.yml.


### Build & Run

#### Build EC FW

Go back to ecfw_demo main folder and build the application and all its dependencies. Below commands indicate details for each board. See Supported hardware section for more details.

```
cd ecfw_demo
# Building for NPCX4mnx 
west build -p always -b npcx4mnx_demo app/demo
```

#### Flash EC FW

Utilize the ***Flash Update Tool*** and ***CR-Eclipse*** for Flashing the EC and Debugging
