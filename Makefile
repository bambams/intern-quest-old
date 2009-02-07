
# Initial macros (are they called macros in Makefiles?).
# We're going to be using GCC / MinGW for now so `g++' is sufficient for all
# platforms.
CXX = g++
CXX_CFLAGS = -Iinclude
CXX_COMPILE = $(CXX) -c $(CXX_CFLAGS)
#CXX_LINK = $(CXX) $(LIBS)
INCDIR = include
LIBS += 
NAME = iq
OBJDIR = obj
SRCDIR = src

# List all .o files here.
OBJECTS += $(OBJDIR)/main.o $(OBJDIR)/iq/app.o

# Some conditional stuff stolen from the SpeedHack Makefile.
ifndef WINDOWS
	ifdef MINGDIR
		WINDOWS = 1
	endif
endif

ifdef WINDOWS
	LIBS += -lalleg
	BIN = $(NAME).exe
	ifdef MINGDIR
		# Link pthreads for Win32 library -- GNU without exceptions.
		LIBS += -lpthreadGC
	endif

	MKDIR = mkdir
	RM = del /q
	RMDIR = rmdir /s /q
else
	BIN = $(NAME)
	LIBS += `allegro-config --libs` -lpthread
	MKDIR = mkdir -p
	RM = rm -f
	RMDIR = rm -fR
endif

# The default will make sure all subdirectories exist, compile, and link the
# project.
default: $(OBJDIR) $(OBJDIR)/iq $(BIN)

# Link the game (create the executable).
$(BIN): $(OBJECTS)
	$(CXX) -o $@ $? $(LIBS)

# -- Compile the source files --
$(OBJDIR)/main.o: $(SRCDIR)/main.cpp $(INCDIR)/main.hpp
	$(CXX_COMPILE) -o $@ $<

$(OBJDIR)/iq/app.o: $(SRCDIR)/iq/app.cpp $(INCDIR)/iq.hpp $(INCDIR)/iq/app.hpp
	$(CXX_COMPILE) -o $@ $<

# -- Make necessary directories --
$(OBJDIR)/iq: $(OBJDIR)
	$(MKDIR) $@

$(OBJDIR):
	$(MKDIR) $@

# -- Cleanup --
clean:
	$(RMDIR) $(OBJDIR) $(OBJDIR)/iq
	$(RM) $(BIN) $(OBJECTS)

# .PHONYs are targets that don't actually correspond to a file on the file
# system.
.PHONY: clean default

