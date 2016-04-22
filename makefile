#Jacob Lehmer 
#3/18/16
#This is the makefile for the ansi fighting game

CC= gcc
CFLAGS = -lpthread
TARGET = Source/game_sync.c Source/graphics.c Source/main.c Source/user_input.c Source/communications.c
OUT = ANSI_FIGHTER

all: $(OUT)

$(OUT) : $(TARGET)
	$(CC) $(CFLAGS) $(TARGET) -o $(OUT)

clean:
	$(RM) $(TARGET)
