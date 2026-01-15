CXX = g++
CXXFLAGS = -Iinclude -Wall -g -std=c++11

SRCDIR = src
OBJDIR = obj
TARGET = oopproj_2025

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

# Ο κύριος κανόνας
all: $(TARGET)

# Κανόνας για τη σύνδεση (link) των αρχείων
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Κανόνας για τη μετάφραση (compile) κάθε αρχείου
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Καθαρισμός αρχείων
clean:
	rm -rf $(OBJDIR)
	rm -f $(TARGET)
