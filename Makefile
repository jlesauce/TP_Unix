include Make/global.mk
include project.mk

ifdef SYSTEMROOT
	include Make/windows.mk
else
ifeq ($(shell uname), Linux)
	include Make/linux.mk
endif
endif

SOURCES := $(shell find $(SRCDIR) -type f -name *.cpp -o -name *.c)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

LDINCLUDES := 

LIBS       := -lpthread

INCLUDES   := -I$(SRCDIR) \

LDFLAGS         =
CFLAGS          = -O0 -g3 -Wall -c -D_REENTRANT -D_GNU_SOURCE
EXTRA_CFLAGS   += -Wextra -Wshadow

.PHONY: clean debug

all: exercise1 exercise2.1 exercise2.2 exercise3

%: $(OBJDIR)/%.o
	@echo "# Generate $(BINDIR)/$@"
	@mkdir -p $(dir $(BINDIR)/$@)
	$(LD) -o bin/$@ $(LDFLAGS) $(OBJDIR)/$@.o $(LDINCLUDES) $(LIBS)
	@echo "# Generation complete!"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	@echo "# Cleanup project..."
	$(RM) -v $(OBJECTS)
	$(RM) -v $(OBJDIR)
	@echo "# Cleanup complete!"

debug:
	@echo "# Run debug mode"
	@echo "# Makefile settings"
	@echo "    RM = $(value RM)"
	@echo "    NOW = $(value NOW)"
	@echo "    HOST = $(value HOST)"
	@echo "# Project settings"
	@echo "    PROJECT_PATH = $(value PROJECT_PATH)"
	@echo "    SRCDIR = $(value SRCDIR)"
	@echo "    BINDIR = $(value BINDIR)"
	@echo "    OBJDIR = $(value OBJDIR)"
	@echo "# Compiler settings"
	@echo "    LD = $(value LD)"
	@echo "    CC = $(value CC)"
	@echo "    CPP = $(value CPP)"
	@echo "    INCLUDES = $(value INCLUDES)"
	@echo "    SOURCES = $(value SOURCES)"
	@echo "    OBJECTS = $(value OBJECTS)"
