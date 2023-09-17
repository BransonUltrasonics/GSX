sudo make CROSS_COMPILE=/home/builder4/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf- ARCH=arm distclean
#sudo make CROSS_COMPILE=/home/builder4/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf- ARCH=arm O=u-boot-sdcard_sc am57xx_evm_nodt_defconfig
sudo make CROSS_COMPILE=/home/builder4/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf- ARCH=arm O=u-boot-sdcard_sc am57xx_evm_defconfig
sudo make CROSS_COMPILE=/home/builder4/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf- ARCH=arm O=u-boot-sdcard_sc
