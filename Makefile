# Detect OS
ifeq ($(OS),Windows_NT)
    # Windows settings
    CXX = g++
    CXXFLAGS = -I"C:\C++ Librairies\SFML-3.0.0\include" -DSFML_STATIC -std=c++17 -Wall
    LDFLAGS = -L"C:\C++ Librairies\SFML-3.0.0\lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lsfml-main-s -lopengl32 -lfreetype -lwinmm -lgdi32
    EXEC = tower_defense.exe
    RM = powershell -Command "if (Test-Path $(EXEC)) { Remove-Item $(EXEC) -Force -ErrorAction SilentlyContinue }"
    RUN = @cmd /c start $(EXEC)
else
    # Assume Linux if not Windows
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CXX = g++
        CXXFLAGS = -I"/usr/include/SFML/" -DSFML_STATIC -std=c++17 -Wall
        LDFLAGS = -L"/usr/lib/SFML/" -L"/usr/lib/x86_64-linux-gnu" -lsfml-graphics -lsfml-window -lsfml-system -lfreetype -lGL
        EXEC = tower_defence
        RM = rm -f $(EXEC)
        RUN = ./$(EXEC)
    endif
endif

# Source files
SRC = main.cpp MapGenerator.cpp Tile.cpp Critter.cpp CritterSpawner.cpp tower.cpp TowerManager.cpp TowerShop.cpp Projectile.cpp TargetingStrategy.cpp DynamicTargetingManager.cpp
OBJ = $(SRC:.cpp=.o)

# Default rule
all: $(EXEC)

# Compile each .cpp into a .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link object files into the final executable
$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Clean up object files and executables
clean:
	rm -f $(OBJ)
	$(RM)

# Run the game
run: $(EXEC)
	$(RUN)

# Rebuild everything
rebuild: clean all

.PHONY: all clean run rebuild

