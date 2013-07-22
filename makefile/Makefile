# Build a ckodo static library
STATICLIB = libckodo.a
STATICLIBSRC = ../src/ckodo/ckodo.cpp

# Build a ckodo shared library
SHAREDLIB = libckodo.so

# Build an example to be statically linked with libckodo.a
EXAMPLE = encode_decode_simple
EXAMPLESRC = ../examples/encode_decode_simple/encode_decode_simple.c

# Specify the compilers
AR = ar
CC = gcc
CXX = g++

# Put the created object files in OBJDIR
OBJDIR = obj
$(shell   mkdir -p $(OBJDIR))

# Specify the path to every dependency in the standalone package

# The include path to the boost sources
BOOST_DIR = ../bundle_dependencies/boost-91e411/1.0.0

# The include path to the sak sources
SAK_DIR = ../bundle_dependencies/sak-2baed8/10.2.0/src

# The include path to the fifi sources
FIFI_DIR = ../bundle_dependencies/fifi-f85dcd/9.1.0/src

# The include path to the kodo sources
KODO_DIR = ../bundle_dependencies/kodo-f3a9b9/11.1.0/src

# Flags for C++ compiler
INCLUDES = -I $(BOOST_DIR) -I $(SAK_DIR) -I $(FIFI_DIR) -I $(KODO_DIR)
CXXFLAGS = -std=c++0x -fPIC -O2 -ftree-vectorize
ARFLAGS = rcs
SHAREDLIBFLAGS = -shared -s
# Produce self-contained shared library
#SHAREDLIBFLAGS = -shared -s -static-libgcc -static-libstdc++

# Flags for C compiler
CCFLAGS= -I ../src
LDFLAGS =
CCLIBS = -Wl,-Bstatic -L. -lckodo -Wl,-Bdynamic -lstdc++

.PHONY: all
all: $(STATICLIB) $(SHAREDLIB) $(EXAMPLE)

# Build the static library
$(STATICLIB): $(OBJDIR)/ckodo.o
	$(AR) $(ARFLAGS) $@ $^
	
# Build the shared library 
$(SHAREDLIB): $(OBJDIR)/ckodo.o
	$(CXX) $< -o $@ $(SHAREDLIBFLAGS) 

$(OBJDIR)/ckodo.o: $(STATICLIBSRC)
	$(CXX) $< -o $@ -c $(CXXFLAGS) $(INCLUDES)

# Build the example 
$(EXAMPLE): $(OBJDIR)/$(EXAMPLE).o $(STATICLIB)
	$(CC) $(LDFLAGS) $< $(CCLIBS) -o $@

$(OBJDIR)/$(EXAMPLE).o: $(EXAMPLESRC)
	$(CC) $< -o $@ -c $(CCFLAGS)


.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ $(STATICLIB) $(SHAREDLIB) $(EXAMPLE)
