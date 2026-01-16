# Compiler Settings
CXX = g++
CXXFLAGS = -Iinclude -Wall -g -std=c++11

# Directories
SRCDIR = src
OBJDIR = obj

# Target Name
BASE_TARGET = avs

# Detect Operating System
ifeq ($(OS),Windows_NT)
	# Windows Settings
	TARGET = $(BASE_TARGET).exe
	MKDIR_CMD = if not exist $(OBJDIR) mkdir $(OBJDIR)
	RM_OBJ_CMD = if exist $(OBJDIR) rmdir /S /Q $(OBJDIR)
	RM_TARGET_CMD = if exist $(TARGET) del /F /Q $(TARGET)
else
	# Linux/Unix Settings
	TARGET = $(BASE_TARGET)
	MKDIR_CMD = mkdir -p $(OBJDIR)
	RM_OBJ_CMD = rm -rf $(OBJDIR)
	RM_TARGET_CMD = rm -f $(TARGET)
endif

# Source and Object files
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

# Phony Targets (commands that are not files)
.PHONY: all clean

# Default Rule
all: $(TARGET)

# Link Rule
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Compile Rule
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@$(MKDIR_CMD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean Rule
clean:
	@$(RM_OBJ_CMD)
	@$(RM_TARGET_CMD)

