
TARGET_CC = /Library/Developer/Toolchains/Hikari_LLVM19.1.7.xctoolchain/usr/bin/clang
TARGET_CXX = /Library/Developer/Toolchains/Hikari_LLVM19.1.7.xctoolchain/usr/bin/clang++
TARGET_LD = /Library/Developer/Toolchains/Hikari_LLVM19.1.7.xctoolchain/usr/bin/clang++

THEOS_PACKAGE_SCHEME = rootless
ARCHS = arm64
DEBUG = 0
FINALPACKAGE = 1
FOR_RELEASE = 1
STRIP = 1
TARGET := iphone:clang:latest:14.0
# SDK_PATH = $(THEOS)/sdks/iPhoneOS16.5.sdk/
# $(info ===> Setting SYSROOT to $(SDK_PATH)...)
# SYSROOT = $(SDK_PATH)
THEOS_DEVICE_IP=192.168.1.71

PACKAGE_TIMESTAMP=$(shell date +%s)
PACKAGE_VERSION="${PACKAGE_TIMESTAMP}"

include $(THEOS)/makefiles/common.mk

TWEAK_NAME = antisocial_gucci

# for string ecncryption - __attribute__( ( annotate( "strenc" ) ) ) 

HIKARI_FLAGS = -mllvm -hikari \
               -mllvm -enable-bcfobf \
               -mllvm -acd-use-initialize \
               -mllvm -bcf_prob=100 \
               -mllvm -bcf_loop=1 \
               -mllvm -bcf_cond_compl=5 \
               -mllvm -bcf_junkasm \
               -mllvm -bcf_junkasm_minnum=4 \
               -mllvm -bcf_junkasm_maxnum=8 \
               -mllvm -indibran-enc-jump-target \
			   -mllvm -enable-funcwra \
               -mllvm -fw_prob=65 \
               -mllvm -fw_times=2 \
               -mllvm -enable-adb \
               -mllvm -adb_prob=30 \
               -mllvm -acd-use-initialize \
			   -mllvm -acd-rename-methodimp \
			  #-mllvm -enable-splitobf \
			   #-mllvm -split_num=2 \

#  CoreAudio  
$(TWEAK_NAME)_FRAMEWORKS =  UIKit Foundation Security QuartzCore CoreGraphics CoreText  AVFoundation Accelerate GLKit SystemConfiguration GameController

# $(HIKARI_FLAGS) 
$(TWEAK_NAME)_CCFLAGS = $(HIKARI_FLAGS) -I./antisocial/thirdparty/network -I./antisocial -I./antisocial/src/cheat -I./antisocial/src -I./antisocial/thirdparty -I./antisocial/thirdparty/freetype2 -I./antisocial/thirdparty/imgui -O3 -std=c++17 -fno-rtti -fno-exceptions -DNDEBUG -fvisibility=hidden -Wc++11-narrowing -Wno-narrowing -Wundefined-bool-conversion -Wreturn-stack-address -Wno-error=format-security -fvisibility=hidden -fpermissive -fexceptions -w -Wno-error=format-security -fvisibility=hidden -Werror -fpermissive -Wall -fexceptions -Wno-module-import-in-extern-c
$(TWEAK_NAME)_CFLAGS = $(HIKARI_FLAGS) -I./antisocial/thirdparty/network -I./antisocial -I./antisocial/src/cheat -I./antisocial/src -I./antisocial/thirdparty -I./antisocial/thirdparty/freetype2 -I./antisocial/thirdparty/imgui -O3 -fobjc-arc -Wno-deprecated-declarations -Wno-unused-variable -Wno-unused-value -fvisibility=hidden -Wc++11-narrowing -Wno-narrowing -Wundefined-bool-conversion -Wreturn-stack-address -Wno-error=format-security -fvisibility=hidden -fpermissive -fexceptions -w -Wno-error=format-security -fvisibility=hidden -Werror -fpermissive -Wall -fexceptions -Wno-module-import-in-extern-c

$(TWEAK_NAME)_FILES = $(shell find . -type f \( -iname "*.cpp" -o -iname "*.m" -o -iname "*.c" -o -iname "*.mm" -o -iname "*.xm" \))

$(TWEAK_NAME)_LDFLAGS += -L./lib/
$(TWEAK_NAME)_LDFLAGS += -lz -stdlib=libc++ -lcurl -lcrypto -lssl -lnghttp2 -ldobby -lfreetype

include $(THEOS_MAKE_PATH)/tweak.mk