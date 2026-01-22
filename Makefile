SRC = real_deal.c
TARGET = real_deal
FLAGS = -O0 -Wl,-s

all: $(TARGET)

$(TARGET): $(SRC)
	gcc $(FLAGS) -o $@ $<

clean:
	rm -f $(TARGET)
