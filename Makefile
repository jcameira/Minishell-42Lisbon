NAME	= minishell

CC					=	cc
CFLAGS				=	-Wall -Wextra -Werror -g
IFLAGS				=	-I $(INCLUDES)
SANITIZE			=	-g -fsanitize=thread -pthread
RANDOM_MALLOC		=	-Xlinker --wrap=malloc
AR					=	ar rcs
RM					=	rm -rf

INCLUDES			=	includes/
DEFAULT_INCLUDES	=	-I /usr/local/includes

SRCS				=	#malloc.c
SRCS_PATH			=	srcs/

OBJ_DIR				=	objects/
OBJS				=	$(SRCS:%.c=$(OBJ_DIR)%.o)
ALL_OBJECTS			=	$(OBJ_DIR)*.o

TOTAL_SRCS			=	$(words $(SRCS))
TOTAL_OBJS			=	$(words $(wildcard $(OBJ_DIR)*))
FILES				=	0

$(OBJ_DIR)%.o:		$(SRCS_PATH)%.c
					@$(CC) $(CFLAGS) $(IFLAGS) $(DEFAULT_INCLUDES) -c $< -o $@ && \
					$(eval FILES=$(shell echo $$(($(FILES) + 1)))) \
					$(call PRINT_PROGRESS,$(TOTAL_SRCS),$(GRN),$(YELLOW)Compiling$(DEFAULT) $@)

all:				$(NAME)

$(NAME):			$(OBJ_DIR) $(OBJS)
					@$(CC) $(CFLAGS) $(IFLAGS) $(OBJS) -o $@
					@echo "\033[2F\033[0K$(CYAN)$(NAME)$(DEFAULT) successfully created\033[E"
					@if norminette philo | grep -q -v "OK!"; then \
						norminette philo | grep -v OK!; echo "Norminette has$(RED) errors!$(DEFAULT)"; \
					else \
						echo "Norminette$(GRN) OK!!$(DEFAULT)"; \
					fi

sanitize:			$(OBJ_DIR) $(OBJS)
					@$(CC) $(CFLAGS) $(IFLAGS) $(SANITIZE) $(OBJS) -o $(NAME)
					@echo "\033[2F\033[0K$(CYAN)$(NAME)$(DEFAULT) successfully created\033[E"

random_m:			$(OBJ_DIR) $(OBJS)
					@$(CC) $(CFLAGS) $(IFLAGS) $(SANITIZE) $(RANDOM_MALLOC) $(OBJS) -o $(NAME)
					@echo "\033[2F\033[0K$(CYAN)$(NAME)$(DEFAULT) successfully created\033[E"

bonus:				$(NAME)

$(OBJ_DIR):
					@mkdir -p $@

clean:				
					@$(foreach file,$(wildcard $(OBJ_DIR)*), \
						$(eval FILES=$(shell echo $$(($(FILES) + 1)))) \
						$(call PRINT_PROGRESS,$(TOTAL_OBJS),$(RED),$(YELLOW)Deleting$(DEFAULT) $(file)); \
						$(RM) $(file); \
					)
					@if [ -d "$(OBJ_DIR)" ]; then \
						$(RM) $(OBJ_DIR); \
						echo "\033[2F\033[0K$(PURPLE)$(OBJ_DIR)$(DEFAULT) deleted\033[E"; \
					fi
					$(eval FILES=0)

fclean:				clean
					@if [ -e "$(NAME)" ]; then \
						$(RM) $(NAME); \
						echo "$(PURPLE)$(NAME)$(DEFAULT) deleted"; \
					fi

re:					fclean all

.PHONY:				all sanitize random_m clean fclean re bonus sanitize_b random_m_b

define PRINT_PROGRESS
    if [ "$(FILES)" -eq "1" ]; then \
        printf "\033[0K$(3)\n["; \
    else \
        printf "\033[0K\033[1F\033[0K$(3)\n["; \
    fi
    @for i in `seq 1 $(shell expr $(FILES) \* 70 / $(1))`; do \
        printf "$(2)=\033[0m"; \
    done
    @for i in `seq 1 $(shell expr 70 - $(FILES) \* 70 / $(1))`; do \
        printf " "; \
    done
    @printf "] $(shell echo $$(($(FILES) * 100 / $(1))))%%"
	if [ "$(FILES)" -eq "$(1)" ]; then \
        printf "\n"; \
	fi
endef

CYAN				=	\033[36m
PURPLE				=	\033[35m
YELLOW				=	\033[33m
GRN					=	\033[32m
RED					=	\033[31m
DEFAULT				=	\033[0m