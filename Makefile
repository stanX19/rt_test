SRCDIR		= srcs
SRCS		:= $(shell find $(SRCDIR) -name '*.cpp')

OBJDIR		= objs
OBJDIRS		= $(sort $(dir $(OBJS)))
OBJS		= $(subst $(SRCDIR)/,$(OBJDIR)/,$(subst .cpp,.o,$(SRCS)))

CWD			:= $(shell pwd)
INCLUDE_DIR	= includes/vulkan/1.4.304.0/x86_64/include  # manually include is better, including everything slow things down
HEADER_DIR	= headers
HEADERS		:= $(shell find $(HEADER_DIR) -name '*.h' -o -name '*.hpp')
HEADER_DIRS := $(sort $(dir $(HEADERS)) $(INCLUDE_DIR)) # $(shell find $(INCLUDE_DIR) -type d -name include))
HEADERS_INC := $(addprefix -I,$(HEADER_DIRS))

IFLAGS		:= -I. $(HEADERS_INC)

SFML_LIBS	= -lsfml-graphics -lsfml-window -lsfml-system
GL_LIBS		= -lglfw -lGLU -lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl
LFLAGS		= $(SFML_LIBS) $(GL_LIBS)

CC			= g++
CFLAGS		= -std=c++20 -Wall -Wextra -Werror -g3 -fsanitize=address
AR			= ar -rcs
RM			= rm -rf
UP			= \033[1A
FLUSH		= \033[2K

NAME		= minirt
ARGV		= 

run: all
	./$(NAME) $(ARGV)

$(NAME): $(OBJDIRS) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(IFLAGS) $(LFLAGS) -o $(NAME)

all: $(NAME)

$(OBJDIRS):
	mkdir -p $@
	@echo "$(UP)$(FLUSH)$(UP)$(FLUSH)$(UP)"

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@
	@echo "$(UP)$(FLUSH)$(UP)$(FLUSH)$(UP)$(FLUSH)$(UP)"

clean:
	@$(RM) $(OBJS)

fclean:	clean
	@$(RM) $(NAME)
	@$(RM) $(TESTDIR)
	@$(RM) $(OBJDIRS)
	@$(RM) ./a.out

re: fclean $(NAME)

push:
	@echo -n "Commit name: "; read name; make fclean;\
	git add .; git commit -m "$$name"; git push;

.PHONY: all clean fclean re bonus push	