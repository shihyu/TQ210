├── 4.4.6  [toolchain]
├── Kernel_3.0.8_TQ210_for_Android_v1.4
├── mkimage
├── TQ210_Android_4.0.4_V1.4
└── TQBoardDNW for linux

### 編譯環境設定
- sudo apt-get install gcc-4.4 g++-4.4 g++-4.4-multilib
``` 
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.4 44
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 48
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.4 44
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 48
```

- sudo update-alternatives --config gcc

###安裝arm-linux- 交叉編譯器環境
```
1. #~/.bashrc 加入
  export PATH="/home/shihyu/data/tq210_note/4.4.6/bin/:$PATH"
2. . ~/.bashtc
3. $ arm-linux-gcc -v


Using built-in specs.
Target: arm-embedsky-linux-gnueabi
Configured with: /opt/EmbedSky/build-croostools/.build/src/gcc-4.4.6/configure --build=i686-build_pc-linux-gnu --host=i686-build_pc-linux-gnu --target=arm-embedsky-linux-gnueabi --prefix=/opt/EmbedSky/4.4.6 --with-sysroot=/opt/EmbedSky/4.4.6/arm-embedsky-linux-gnueabi/embedsky --enable-languages=c,c++ --disable-multilib --with-arch=armv7-a --with-cpu=cortex-a8 --with-tune=cortex-a8 --with-fpu=neon --with-float=softfp --with-pkgversion='for TQ210 EmbedSky Tech' --with-bugurl=http://www.embedsky.net --disable-sjlj-exceptions --enable-__cxa_atexit --disable-libmudflap --disable-libgomp --disable-libssp --with-gmp=/opt/EmbedSky/build-croostools/.build/arm-embedsky-linux-gnueabi/build/static --with-mpfr=/opt/EmbedSky/build-croostools/.build/arm-embedsky-linux-gnueabi/build/static --with-mpc=/opt/EmbedSky/build-croostools/.build/arm-embedsky-linux-gnueabi/build/static --with-ppl=no --with-cloog=no --with-host-libstdcxx='-static-libgcc -Wl,-Bstatic,-lstdc++,-Bdynamic -lm' --enable-threads=posix --enable-target-optspace --without-long-double-128 --with-local-prefix=/opt/EmbedSky/4.4.6/arm-embedsky-linux-gnueabi/embedsky --disable-nls --enable-c99 --enable-long-long
Thread model: posix
gcc version 4.4.6 (for TQ210 EmbedSky Tech)
```


###燒錄環境設定 RS232

```
1. sudo apt-get install gtkterm
2. sudo gtkterm
3. 設定 正確ㄉ的 port  /dev/ttyUSB0    , Rate 115200
4. USB to RS232 線接在板子 COM1 
```

- 如果沒畫面或是無法啟動按一下板子的KEYON , 設定好開電源 , 按板子上 reset 鍵 , 按住鍵盤空白鍵會進入EmbedSky USB download mode

```
##### Boot for TQ210 Main Menu #####
##### EmbedSky USB download mode #####

[1] Download bootloader (u-boot or bootimage) to Nand Flash
[2] Download WinCE NK image (NK.bin) to Nand Flash
[3] Download Linux Kernel (zImage.bin) to Nand Flash
[4] Download LOGO Picture (logo.bin) to Nand Flash
[5] Download UBIFS image (root.ubi) to Nand Flash
[6] Download YAFFS image (root.bin) to Nand Flash
[7] Download Program to SDRAM and Run it
[8] Boot the system
[9] Format the Nand Flash
[0] Set the boot parameters
[a] Download User Program
[n] Enter TFTP download mode menu
[r] Reboot u-boot
[t] Test Linux Image (zImage)
[u] Download bootloader to SD Card
Enter your selection:

選一個要燒的mode 例如 t  , [t] Test Linux Image (zImage)
4. UBS 燒錄image , 之後就可以啟動 TQBoardDNW_64 連接usb 成功
sudo ./TQBoardDNW_64  
```



###安裝檔案系統壓縮工具
```
 export PATH="/home/shihyu/data/tq210_note/mkimage/:$PATH" 
```


###編譯核心
```
1. cp config_for_TQ210_Android_v1.4_CoreB .config
2. make menuconfig  // save 後離開
3. time make zImage -j8
```

###編譯android 平台
```
time ./build_coreB_android.sh 
```

### ubuntu13.04下編譯android系統新錯誤
```
dalvik/vm/native/dalvik_system_Zygote.cpp: In function 'int setrlimitsFromArray(ArrayObject*)': 
dalvik/vm/native/dalvik_system_Zygote.cpp:194: error: aggregate 'rlimit rlim' has incomplete type and cannot be defined 
dalvik/vm /native/dalvik_system_Zygote.cpp:217: error: 'setrlimit' was not declared in this scope 
make: *** [out/host/linux-x86/obj/SHARED_LIBRARIES/libdvm_intermediates/native/dalvik_system_Zygote.o] Error 1 
make: *** Waiting for unfinished jobs…. 
marsuu@marsuu-ubuntu:~/BulidPlace/android/ics$ vim dalvik/vm/native/dalvik_system_Zygote.cpp


解決方法
add  #include <sys/resource.h>  to  dalvik/vm/native/dalvik_system_Zygote.cpp

mkimage -A arm -O linux -T ramdisk -C none -a 0x30A00000 -n "ramdisk" -d out/target/product/tq210/ramdisk.img out/target/product/tq210/ramdisk-uboot.img
/bin/bash: mkimage：命令找不到
make: *** [out/target/product/tq210/ramdisk.img] Error 127
make: *** Deleting file `out/target/product/tq210/ramdisk.img'
make: *** Waiting for unfinished jobs....
```



###製作檔案Image
```
sudo apt-get install uboot-mkimage
cd out/target/product/tq210
mkyaffs2image_for_TQ210   rootfs_dir/   root_android4.bin
```
