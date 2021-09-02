BUILD_DIR = ./objects
SRC_DIR = ./source
GAME_DIR = ./source/Game

CFILES = $(wildcard $(SRC_DIR)/*.c)
OFILES = $(CFILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
GAMECFILES = $(wildcard $(GAME_DIR)/*.c)
GAMEOFILES = $(GAMECFILES:$(GAME_DIR)/%.c=$(BUILD_DIR)/%.o)

CFLAGS =   -O3  #-ffreestanding -nostdinc  -nostdlib -nostartfiles
LDFLAGS =  -nostartfiles  -nostdlib 

all: clean kernel8.img run
$(BUILD_DIR)/start.o: $(SRC_DIR)/start.S
	aarch64-none-elf-gcc  -c $< -o $@
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	aarch64-none-elf-gcc  -c $< -o $@ $(CFLAGS)
	
$(BUILD_DIR)/%.o: $(GAME_DIR)/%.c
	aarch64-none-elf-gcc  -c $< -o $@ $(CFLAGS)

kernel8.img: $(BUILD_DIR)/start.o $(OFILES) $(GAMEOFILES)
	aarch64-none-elf-ld  $(BUILD_DIR)/start.o  $(LDFLAGS) $(OFILES) $(GAMEOFILES) -T $(SRC_DIR)/link.ld -o $(BUILD_DIR)/kernel8.elf
	aarch64-none-elf-objcopy -O binary $(BUILD_DIR)/kernel8.elf kernel8.img

clean:
	delete.bat
#	del .\objects\kernel8.elf .\objects\*.o

run:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial null -serial stdio
	
music:
	cmd /C start music.mp3
test: all run