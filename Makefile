
# Initial macros (are they called macros in Makefiles?).
# We're going to be using GCC / MinGW for now so `g++' is sufficient for all platforms.
CXX = g++
CXX_CFLAGS = -Iinclude
CXX_COMPILE = $(CXX) -c $(CXX_CFLAGS)
CXX_LINK = $(CXX) $(LIBS)
# Might as well call the executable something descriptive (right...) of our project. :P
EXE = iq
LIBS += 
OBJDIR = obj

# List all .o files here.
OBJ += $(OBJDIR)/main.o

# Some conditional stuff stolen from the SpeedHack Makefile.
ifndef WINDOWS
	ifdef MINGDIR
		WINDOWS = 1
	endif
endif

ifdef WINDOWS
	LIBS += -lalleg

	ifdef MINGDIR
		# Link pthreads for Win32 library -- GNU without exceptions.
		LIBS += -lpthreadGC
	endif

	MKDIR = mkdir
	RM = del /q
	RMDIR = rmdir /s /q
else
	LIBS += `allegro-config --libs` -pthread
	MKDIR = mkdir -p
	RM = rm -f
	RMDIR = rm -fR
endif

# The default will make sure all subdirectories exist, compile, and link the project.
default: $(OBJDIR) $(EXE)

# Link the game (create the executable).
$(EXE): $(OBJ)
	$(CXX_LINK) -o $@ $?

# -- Compile the source files --
$(OBJDIR)/main.o: src/main.cpp include/main.hpp
	$(CXX_COMPILE) -o $@ $<

# -- Make necessary directories --
$(OBJDIR):
	$(MKDIR) $@

# -- Cleanup --
clean:
	$(RM) $(EXE)
	$(RMDIR) $(OBJDIR)

# PHONYs are targets that don't actually have any results, IIRC.
# Add all header files to PHONY dependencies.
PHONY: default include/main.hpp

