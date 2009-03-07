
# Initial macros (are they called macros in Makefiles?).
# We're going to be using GCC / MinGW for now so `g++' is sufficient for all
# platforms.
CXX = g++
CXX_CFLAGS = -g3 -Iinclude -pg -Wall
CXX_COMPILE = $(CXX) -c $(CXX_CFLAGS)
#CXX_LINK = $(CXX) $(CXX_CFLAGS) $(LIBS)
INCDIR = include
LIBS += 
NAME = iq
OBJDIR = obj
SRCDIR = src

# List all .o files here.
OBJECTS += $(OBJDIR)/main.o $(OBJDIR)/app.o $(OBJDIR)/timer.o

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
default: $(OBJDIR) $(BIN)

# Link the game (create the executable).
$(BIN): $(OBJECTS)
	$(CXX) $(CXX_CFLAGS) -o $@ $(OBJECTS) $(LIBS)

# -- Compile the source files --
$(OBJDIR)/main.o: $(SRCDIR)/main.cpp $(INCDIR)/main.hpp $(INCDIR)/app.hpp $(INCDIR)/iq.hpp
	$(CXX_COMPILE) -o $@ $<

$(OBJDIR)/app.o: $(SRCDIR)/app.cpp $(INCDIR)/app.hpp $(INCDIR)/iq.hpp
	$(CXX_COMPILE) -o $@ $<

$(OBJDIR)/timer.o: $(SRCDIR)/timer.cpp $(INCDIR)/timer.hpp $(INCDIR)/iq.hpp
	$(CXX_COMPILE) -o $@ $<

# -- Make necessary directories --
$(OBJDIR):
	$(MKDIR) $@

# -- Cleanup --
clean:
	$(RMDIR) $(OBJDIR)
	$(RM) $(BIN) $(OBJECTS) gmon.out

# .PHONYs are targets that don't actually correspond to a file on the file
# system.
.PHONY: clean default

