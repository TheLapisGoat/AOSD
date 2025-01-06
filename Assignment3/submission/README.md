# Assignment 3
Sourodeep Datta (21CS10064)
Krish Khimasia  (21CS10037)

## Creating the Initial Configuration
- Run `make raspberrypi3_defconfig` to load the default configuration for Raspberry Pi 3
- Run `make menuconfig` to open the configuration menu
- ### To display name in banner:
    - Go to `System Configuration` -> `System Banner` and enter the name to be displayed in the banner
- ### To enable the `nano` editor:
    - Go to `Target Packages` -> `Text editors and viewers` -> `nano` and select `nano`
- ### To set a root password:
    - Go to `System Configuration` -> `Root Password` and enter the desired password
- ### To enable ssh server capabilities:
    - Go to `Target Packages` -> `Networking Applications` -> `openssh` and select `openssh`
- ### To enable `net-tools`:
    - Go to `Target Packages` -> `Networking Applications` -> `net-tools` and select `net-tools`
- Save the configuration and exit

## Building the Kernel
- Run `make -j <number of cores>` to build the kernel
- The kernel image will be generated at `output/images/`