CXX = g++
CXXFLAGS = -Iinclude -Wall -g -std=c++11

SRCDIR = src
OBJDIR = obj
TARGET = main

# Λίστα με τα source files (π.χ. src/Main.cpp src/Simulation.cpp ...)
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
# Μετατροπή των source files σε object files (π.χ. obj/Main.o obj/Simulation.o ...)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

# Ο κύριος κανόνας
all: $(TARGET)

# Κανόνας για τη σύνδεση (link) των αρχείων
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Κανόνας για τη μετάφραση (compile) κάθε αρχείου
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@if not exist "$(OBJDIR)" mkdir "$(OBJDIR)"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Καθαρισμός αρχείων
clean:
	@if exist "$(OBJDIR)" rmdir /S /Q "$(OBJDIR)"
	@if exist "$(TARGET).exe" del "$(TARGET).exe"
