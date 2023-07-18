CC := gcc
CFLAGS := $(shell pkg-config --cflags sofia-sip-ua)
LIBS := $(shell pkg-config --libs sofia-sip-ua)

TARGET := send_sip_message
SRCS := send_sip_message.c
OBJS := $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
