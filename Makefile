CC = gcc

CFLAGS = -Wall -Wextra -g -Iinclude
TARGET = city_manager

SRCS = src/CityManager/city_manager.c \
       src/CityManager/command.c \
       src/CityManager/sanitization.c \
       src/CityManager/cfg_logs_utils.c \
       src/CityManager/cli_parser.c \
       src/CityManager/file_utils.c \
       src/CityManager/filter_utils.c \
       src/CityManager/report_utils.c \

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
