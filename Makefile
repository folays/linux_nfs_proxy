NAME	= nfs_proxy
CC	= gcc
RM	= rm -f

CFLAGS	= $(pkg-config --cflags libtirpc)
LIBS	= $(pkg-config --libs libtirpc)

SRC	= nfs_proxy.c rpc_nfsd.c rpc_mountd.c xdr.c
OBJ	= $(SRC:.c=.o)

all	: $(NAME)

$(NAME)	: $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LIBS)

re	: fclean all

clean	:
	-$(RM) $(OBJ) *~

fclean	: clean
	-$(RM) $(NAME)
