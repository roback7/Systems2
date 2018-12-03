##
## Makefile for user
##
## Made by xsyann, Updated by Kenny Roback
## Contact <contact@xsyann.com>
##
## Started on  Wed Mar 26 13:48:49 2014 xsyann
## Last update Fri Apr 18 22:13:17 2014 xsyann
##

TARGET	= user
SRC	= user.c
OBJ	= $(SRC:.c=.o)
CC	= gcc
CFLAGS	+= -W -Wall
RM	= rm -vf

$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

all	: $(TARGET)

clean	:
	$(RM) $(OBJ) *~ \#*#

fclean	: clean
	$(RM) $(TARGET)

re	: fclean all clean

.PHONY	: clean fclean re
