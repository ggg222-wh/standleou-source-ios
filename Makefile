include $(THEOS)/makefiles/common.mk

TWEAK_NAME = antisocial_gucci

# Пути к исходникам
antisocial_gucci_FILES = $(shell find . -type f \( -iname "*.cpp" -o -iname "*.m" -o -iname "*.c" -o -iname "*.mm" -o -iname "*.xm" \))

# Фреймворки
antisocial_gucci_FRAMEWORKS = UIKit Foundation Security QuartzCore CoreGraphics CoreText AVFoundation Accelerate GLKit SystemConfiguration GameController

# Флаги компиляции (без Hikari)
antisocial_gucci_CFLAGS = -I./antisocial -I./antisocial/src -I./antisocial/thirdparty -I./antisocial/thirdparty/freetype2 -I./antisocial/thirdparty/imgui -I./antisocial/thirdparty/network -O3 -fobjc-arc -std=c++17 -Wno-deprecated-declarations -Wno-unused-variable -Wno-unused-value -Wno-narrowing -w

# Ссылки с библиотеками
antisocial_gucci_LDFLAGS = -L./lib/ -lz -stdlib=libc++ -lcurl -lcrypto -lssl -lnghttp2 -ldobby -lfreetype

include $(THEOS_MAKE_PATH)/tweak.mk
