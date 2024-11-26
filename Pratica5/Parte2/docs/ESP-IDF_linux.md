# Short tutorial on how to run an ESP32 application on Linux by terminal

Remember, think it through before running the below commands.

## Installing dependencies

It may vary from distro to distro, here are some usual ones:

Ubuntu:

```bash
sudo apt-get install git wget flex bison gperf python3 python3-pip python3-venv cmake \
 ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0
```

Arch:

```bash
sudo pacman -S git cmake ninja ccache dfu-util python
```

**Clone the repo** 

```bash
git clone -b v5.3 --recursive https://github.com/espressif/esp-idf.git
```

**Install tools** 

```bash
./install.sh esp32
```

## Folders 'n files

Let's start by building the folders and files.

```bash
mkdir ~/Dir

touch ~/Dir/CMakeLists.txt

mkdir ~/Dir/main 

touch ~/Dir/main/CMakeLists.txt

touch ~/Dir/main/main.c

```


Add the following content to ~/Dir/CMakeLists.txt 

 
```CMake
cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(YOUR_PROJECT)
```

Add the following to ~/Dir/main/CMakeLists.txt


```CMake
idf_component_register(SRCS "main.c"
                       INCLUDE_DIRS ".")
```

Add the code in main.c

With those files in place, you can already see your code running.

**Now that you have done it once, you can use the automatic way :)** 

By just running :

```bash
    idf.py create-project $YOUR_PROJECT$
```


## Building and flashing

First, you need to start the virtual environment, by running the following:

```bash
source ./path-to-esp-idf/export.sh

idf.py set-target esp32

idf.py build

idf.py -p /dev/ttyACM0 flash

idf.py -p /dev/ttyACM0 monitor
```


The ttyACM0 corresponds to the USB entry in your computer, so it could have another name, usually ttyUSB0, so change it. Also, see the permissions, you may have to change them, in my case:

```bash
sudo chmod 666 /dev/ttyACM0

```
