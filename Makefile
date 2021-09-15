BUILD_DIR = ./objects
SRC_DIR = ./source
GAME_DIR = ./source/Game
FONT_DIR = ./source/Font
IMAGE_DIR = ./source/Image
VIDEO_DIR = ./source/Video

CFILES = $(wildcard $(SRC_DIR)/*.c)
OFILES = $(CFILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

GAMECFILES = $(wildcard $(GAME_DIR)/*.c)
GAMEOFILES = $(GAMECFILES:$(GAME_DIR)/%.c=$(BUILD_DIR)/%.o)

FONTCFILES = $(wildcard $(FONT_DIR)/*.c)
FONTOFILES = $(FONTCFILES:$(FONT_DIR)/%.c=$(BUILD_DIR)/%.o)

IMAGECFILES = $(wildcard $(IMAGE_DIR)/*.c)
IMAGEOFILES = $(IMAGECFILES:$(IMAGE_DIR)/%.c=$(BUILD_DIR)/%.o)

VIDEOCFILES = $(wildcard $(VIDEO_DIR)/*.c)
VIDEOOFILES = $(VIDEOCFILES:$(VIDEO_DIR)/%.c=$(BUILD_DIR)/%.o)

CFLAGS = -W -Wall -g3  -O3  #-ffreestanding -nostdinc  -nostdlib -nostartfiles
LDFLAGS =  -nostartfiles  -nostdlib 

all: clean kernel8.img run

$(BUILD_DIR)/start.o: $(SRC_DIR)/start.S
	aarch64-none-elf-gcc  -c $< -o $@
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	aarch64-none-elf-gcc $(CFLAGS) -c $< -o $@ 
	
$(BUILD_DIR)/%.o: $(GAME_DIR)/%.c
	aarch64-none-elf-gcc $(CFLAGS) -c $< -o $@ 

kernel8.img: $(BUILD_DIR)/start.o $(OFILES) $(GAMEOFILES)
	aarch64-none-elf-ld  $(BUILD_DIR)/start.o  $(LDFLAGS) $(OFILES) $(GAMEOFILES) -T $(SRC_DIR)/link.ld -o $(BUILD_DIR)/kernel8.elf
	aarch64-none-elf-objcopy -O binary $(BUILD_DIR)/kernel8.elf kernel8.img

font:
	aarch64-none-elf-gcc $(GCCFLAGS) -c $(SRC_DIR)/start.S -o $(BUILD_DIR)/start.o
	aarch64-none-elf-gcc $(CFLAGS) -c $(SRC_DIR)/main.c -o $(BUILD_DIR)/main.o
	aarch64-none-elf-gcc $(CFLAGS) -c $(SRC_DIR)/framebf.c -o $(BUILD_DIR)/framebf.o
	aarch64-none-elf-gcc $(CFLAGS) -c $(SRC_DIR)/mbox.c -o $(BUILD_DIR)/mbox.o
	aarch64-none-elf-gcc $(CFLAGS) -c $(SRC_DIR)/uart.c -o $(BUILD_DIR)/uart.o
	aarch64-none-elf-gcc $(CFLAGS) -c $(FONT_DIR)/font.c -o $(BUILD_DIR)/font.o
	aarch64-none-elf-ld  $(BUILD_DIR)/start.o  $(LDFLAGS) $(OFILES) $(FONTOFILES) -T $(SRC_DIR)/link.ld -o $(BUILD_DIR)/kernel8.elf
	aarch64-none-elf-objcopy -O binary $(BUILD_DIR)/kernel8.elf kernel8.img
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial null -serial stdio

image: 
	aarch64-none-elf-gcc $(GCCFLAGS) -c $(SRC_DIR)/start.S -o $(BUILD_DIR)/start.o
	aarch64-none-elf-gcc $(CFLAGS) -c $(SRC_DIR)/main.c -o $(BUILD_DIR)/main.o
	aarch64-none-elf-gcc $(CFLAGS) -c $(SRC_DIR)/framebf.c -o $(BUILD_DIR)/framebf.o
	aarch64-none-elf-gcc $(CFLAGS) -c $(SRC_DIR)/mbox.c -o $(BUILD_DIR)/mbox.o
	aarch64-none-elf-gcc $(CFLAGS) -c $(SRC_DIR)/uart.c -o $(BUILD_DIR)/uart.o
	aarch64-none-elf-gcc $(CFLAGS) -c $(IMAGE_DIR)/image.c -o $(BUILD_DIR)/image.o
	aarch64-none-elf-ld  $(BUILD_DIR)/start.o  $(LDFLAGS) $(OFILES) $(IMAGEOFILES) -T $(SRC_DIR)/link.ld -o $(BUILD_DIR)/kernel8.elf
	aarch64-none-elf-objcopy -O binary $(BUILD_DIR)/kernel8.elf kernel8.img
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial null -serial stdio

video: 
	aarch64-none-elf-gcc $(GCCFLAGS) -c $(SRC_DIR)/start.S -o $(BUILD_DIR)/start.o
	aarch64-none-elf-gcc $(CFLAGS) -c $(SRC_DIR)/main.c -o $(BUILD_DIR)/main.o
	aarch64-none-elf-gcc $(CFLAGS) -c $(SRC_DIR)/framebf.c -o $(BUILD_DIR)/framebf.o
	aarch64-none-elf-gcc $(CFLAGS) -c $(SRC_DIR)/mbox.c -o $(BUILD_DIR)/mbox.o
	aarch64-none-elf-gcc $(CFLAGS) -c $(SRC_DIR)/uart.c -o $(BUILD_DIR)/uart.o
	aarch64-none-elf-gcc $(CFLAGS) -c $(VIDEO_DIR)/video.c -o $(BUILD_DIR)/video.o
	aarch64-none-elf-ld  $(BUILD_DIR)/start.o  $(LDFLAGS) $(OFILES) $(VIDEOOFILES) -T $(SRC_DIR)/link.ld -o $(BUILD_DIR)/kernel8.elf
	aarch64-none-elf-objcopy -O binary $(BUILD_DIR)/kernel8.elf kernel8.img
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial null -serial stdio

clean:
	delete.bat
#	del .\objects\kernel8.elf .\objects\*.o

run:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial null -serial stdio
	
music:
	cmd /C start music.mp3
test: all run