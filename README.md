# Tower Defense Game - Advanced C++ Project

**Author:** Sanjai Ratnarajah  
**Team:** FORTIFY  
**Course:** COMP 345 - Advanced C++  
**Project:** Tower Defense Game  

## 📖 Overview

This Tower Defense Game is an interactive strategy game developed in C++ using SFML 3.0. Players strategically place towers to defend against waves of critters trying to reach the end of a predefined path. The game incorporates multiple design patterns including Observer, Strategy, Factory, and Decorator, showcasing advanced C++ programming concepts.

---

## 🛠️ Features

### Core Gameplay:
- **Level Selection System:** Choose from preset difficulty levels (Easy, Normal, Hard) or create a custom map.
- **Custom Map Builder:** Design your own level with start and end points and a valid path.
- **Tower Strategy Pattern:** Enhanced tower targeting with dynamic strategy selection.
- **Critter Factory Pattern:** Dynamic enemy wave generation with increasing difficulty.
- **Tower Decorator Pattern:** Modular tower upgrades with specialized enhancements.
- **Observer Pattern Implementation:** Event handling and system coordination.
- **Error Logging:** Logs runtime crashes to `game_crash_log.txt`.

### Enhanced Features:
- **Life Points System:** Players now have a limited number of lives. Different critters deduct different amounts of life points when reaching the exit.
- **Entity Inspection:** Hold Alt while hovering over towers or critters to view detailed information about them.
- **Health Bars:** Visual health indicators for all critters to better track battle progress.
- **Responsive UI:** All UI elements automatically adjust to window resizing for both custom and preset maps.
- **Game States:** Properly implemented win and lose conditions with appropriate game over screens.
- **Restart Functionality:** Press R to restart the game after victory or defeat.
- **Tower Targeting Visualization:** See which targeting strategy each tower is using.
- **Controls Display:** A dedicated button displays all key bindings and controls for easy reference during gameplay.
- **Cross-Platform Support:** Game runs on both Windows and Linux operating systems.

---

## 🖥️ Requirements

- **SFML 3.0**  
- **C++ Compiler**  
- **Operating System:** Windows or Linux

---

## ⚙️ Installation Instructions

1. Clone the project files into your working directory.  
2. Ensure SFML 3.0 is installed and linked correctly.  
3. Compile the game using the provided `Makefile`:
   ```sh
   make
   ```
4. Run the game executable:
   ```sh
   ./tower_defense
   ```

---

## 🎮 Gameplay Instructions

1. **Launch Game:** Run the compiled executable.  
2. **Select Level:** Choose from Easy (18x18), Normal (12x12), Hard (8x8), or Custom.

### Custom Map Creation
If you select "Custom", you'll enter the map editor:

1. **Set Map Dimensions:**
   - Enter width and height values (8-20) for your custom map.
   - Use number keys to input values.
   - Press Tab to switch between width and height fields.
   - Click "Confirm" or press Enter when done.

2. **Place Start Tile:**
   - Left-click on a grid cell to place the starting point (green tile).
   - This is where critters will spawn.

3. **Place End Tile:**
   - Left-click on a different grid cell to place the ending point (red tile).
   - This is where critters will try to reach.
   - Note: Start and end tiles cannot be adjacent to each other.

4. **Create Path:**
   - Left-click on cells to create a continuous path (brown tiles) connecting the start and end points.
   - The path must form an unbroken connection.
   - You can only place path tiles on empty (gray) cells.

5. **Validate Map:**
   - A validation message will appear when you've created a valid path.
   - Right-click anywhere to confirm and proceed to the game when the validation message appears.
   - If you make a mistake, press the "Restart" button in the top-right corner to reset the map.

6. **Error Handling:**
   - If you try to create an invalid configuration (e.g., disconnected path), an error message will appear.
   - Try to create a continuous path from start to end without gaps.

### Game Controls

1. **Purchase Towers:**
   - Click the **Shop** button in the bottom-right corner to open the tower purchase menu
   - Select a tower type (Basic, Area, or Special) to view its stats and cost
   - After selecting a tower, click on valid ground tiles to place it
   - Your available gold is displayed at the top of the screen

2. **Place Towers:**  
   - **Left-Click** on valid positions to place towers.  
   - **Shift + Left-Click** on a tower to sell it.  
   - **Right-Click** on a tower to upgrade it.  

3. **Start Waves:** Press the **Start Wave** button to initiate enemy waves.  

4. **Defend Your Base:** Prevent critters from reaching the end by strategically placing towers.

5. **Life Points:** Monitor your remaining lives shown in the top-left corner. Game ends when lives reach zero.

6. **Inspect Entities:** Hold **Alt** while hovering over towers or critters to see detailed information.

7. **View Controls:** Click the **Controls** button in the bottom-left corner to display key bindings.

8. **Restart:** After winning or losing, press **R** to restart the game.

---

## 🧩 Design Patterns Implementation

### **Strategy Pattern (Tower Targeting)**
The Strategy pattern is implemented with multiple concrete targeting strategies that towers can dynamically switch between:

- **Smart Targeting Strategy:** A balanced approach considering multiple factors like critter health and position
- **Exit Proximity Strategy:** Prioritizes critters closest to the exit point
- **Strongest First Strategy:** Targets critters with the highest health
- **Weakest First Strategy:** Targets critters with the lowest health
- **Closest First Strategy:** Targets critters nearest to the tower

The system includes a **DynamicTargetingManager** that automatically selects the most appropriate strategy based on the current game conditions:
- Number of critters
- Presence of Level 3 boss critters
- Tower type (Basic, Area, Special)
- Critter proximity to exit

This implementation allows towers to intelligently adapt their targeting behavior as the battle evolves without requiring player intervention.

### **Factory Pattern (Critter Wave Generation)**
The Factory pattern is used to create different types of critter waves based on game progression:

- **Easy Wave Factory:** Creates waves of basic critters (early game, Wave 1)
- **Medium Wave Factory:** Creates mixed waves of basic and medium critters (mid game, Wave 2)
- **Hard Wave Factory:** Creates waves with medium and boss critters (late game, Wave 3+)

This pattern provides a clean separation between wave creation logic and the rest of the game.

### **Decorator Pattern (Tower Upgrades)**
The Decorator pattern is used to dynamically enhance towers with various upgrades:

- **Base Decorators:**
  - **RangeUpgradeDecorator:** Increases tower range
  - **PowerUpgradeDecorator:** Increases tower damage
  - **FireRateUpgradeDecorator:** Increases tower attack speed

- **Special Decorators:**
  - **Level3CritterFocusDecorator:** Prioritizes level 3 critters
  - **AoERadiusEnhancerDecorator:** Increases area of effect radius
  - **RefundValueBoosterDecorator:** Increases tower sale value

This pattern allows for flexible tower customization without creating an explosion of tower subclasses.

### **Observer Pattern (UI Updates)**
The Observer pattern enables efficient communication between game components:

- **MapObserver:** Monitors changes to the map and updates the visual representation
- **TowerObserver:** Tracks tower state changes and ensures proper rendering
- **CritterObserver:** Keeps the critter visuals in sync with game state changes

---

## 🏰 Towers

### **Tower Types:**

| Tower Type  | Cost  | Range  | Power | Fire Rate | Special Ability |
|------------|------|--------|-------|-----------|----------------|
| **Basic Tower** | 100  | Medium | Medium | Fast | Balanced stats |
| **Area Tower** | 150  | Short  | High   | Slow   | Splash damage |
| **Special Tower** | 125  | Long  | High   | Medium | Deals extra damage to Level 3 Critters |

- **Upgrades:** Towers can be upgraded twice, applying various decorators:
  - **Level 1→2 Upgrade:** Applies specific decorators based on tower type
  - **Level 2→3 Upgrade:** Applies more powerful decorators with special abilities

- **Upgrade Costs:**
  - **Basic Tower:** Level 1→2: $100, Level 2→3: $390
  - **Area Tower:** Level 1→2: $100, Level 2→3: $260
  - **Special Tower:** Level 1→2: $100, Level 2→3: $325

- **Selling Towers:** Players receive **75% refund** when selling a tower.
  - RefundValueBooster decorator can increase this value.
- **Inspection:** Hold Alt while hovering over a tower to see its stats, level, type, current targeting strategy, and upgrade cost.

---

## 🐛 Critters

| Critter Type | Health | Speed  | Reward | Life Points Cost | Special Trait |
|-------------|--------|--------|--------|--------------|---------------|
| **Basic Critter** | Low    | Medium | 5-10   | 1 | No special traits |
| **Advanced Critter** | Medium | Fast   | 10-60  | 2 | Faster movement |
| **Boss Critter** | High   | Slow   | 50-300 | 3 | Very high health |

**Factory Implementation:**
- **Wave 1:** Primarily basic critters with gradually increasing stats
- **Wave 2:** Mix of basic and medium critters with enhanced stats
- **Wave 3+:** Medium and boss critters with significant scaling based on wave number

**Health Bars:** All critters display a health bar above them, showing their current health percentage.

**Inspection:** Hold Alt while hovering over a critter to see its type, current health, and reward value.

**Effects when reaching the end:**
- **Gold Penalty:** Gold is deducted based on critter type
- **Life Points Loss:** Life points are reduced based on critter type:
  - **Basic Critter:** -1 life
  - **Advanced Critter:** -2 lives
  - **Boss Critter:** -3 lives

---

## 🎮 Game States

### **Gameplay States:**
- **Normal Gameplay:** Placing towers, managing waves, and defending your base
- **Victory:** Achieved after successfully completing all three waves
- **Game Over:** Occurs when life points reach zero

### **UI Elements:**
- **Life Points Counter:** Shows remaining lives with heart icon
- **Currency Display:** Shows available gold for purchasing and upgrading towers
- **Wave Indicator:** Shows current wave number
- **Victory Screen:** Displays when all waves are defeated
- **Game Over Screen:** Appears when all lives are lost
- **Restart Prompt:** Press R to restart the game after victory or defeat
- **Controls Button:** Located in the bottom-left corner, displays a comprehensive list of all game controls
- **Shop Button:** Located in the bottom-right corner, opens a menu for purchasing different tower types with detailed stats and descriptions

---

## 🔍 UI and Visual Features

- **Responsive UI:** All interface elements automatically adjust when window is resized
- **Health Bars:** Visual indicators of critter health with color-coded states
- **Tower Range Indicators:** Visual radius display when selecting towers
- **Entity Inspection Tooltips:** Detailed information about towers and critters
- **Damage Flash Effects:** Visual feedback when critters take damage
- **AoE Indicators:** Visual display of area effect radius for area towers
- **Controls Dialog:** Easily accessible reference for all key bindings and game controls
- **Shop Interface:** Tower selection menu with tower types, costs, and descriptions for strategic planning
- **Custom Map Validation:** Visual feedback when a valid path is created in the map editor

---

## 💡 Advanced Implementation Features

1. **Dynamic Strategy Selection:** Towers automatically choose the best targeting strategy based on battlefield conditions
2. **Life Points System:** Adds strategic depth by forcing players to prioritize threats
3. **Entity Inspection:** Provides detailed information for better tactical decisions
4. **Responsive Design:** Ensures consistent gameplay experience regardless of window size
5. **Custom Map Validation:** Ensures player-created maps have valid paths with visual feedback
6. **Restart Functionality:** Seamlessly resets game state without requiring application restart
7. **Win/Lose Conditions:** Clearly defined victory and defeat states with appropriate feedback
8. **Cross-Platform Support:** Game functions properly on both Windows and Linux operating systems
9. **Controls Reference:** Easy access to game instructions during gameplay

---

## 🔧 Technical Implementation Notes

### UI Resizing:
The UI components dynamically adjust to window size changes, maintaining proper aspect ratios and relative positioning. This applies to both the game interface and the custom map editor.

### Life Points System:
Life points are tracked in the CritterSpawner class, with different critter types deducting varying amounts when reaching the exit. The game over state is triggered when life points reach zero.

### Entity Inspection:
Inspection tooltips are implemented using the Alt key modifier and provide real-time information about towers and critters. The data is formatted and displayed in readable tooltips.

### Health Bar System:
Critter health bars use SFML shapes with dynamic sizing based on current health percentage. The system provides visual feedback through color changes and positioning.

### Cross-Platform Compatibility:
The game uses preprocessor directives to handle platform-specific code, allowing it to run seamlessly on both Windows and Linux systems.

### Controls Dialog:
A dedicated UI component shows all game controls when the Controls button is clicked. The dialog is positioned centrally with clear formatting and a semi-transparent overlay for better readability.

### Shop System:
The shop interface allows players to select from different tower types, displaying relevant statistics and costs. The system integrates with the player's currency system to enforce purchase limitations based on available gold.


*Developed as part of the COMP 345 Advanced C++ Programming course.*

### **Enjoy the game! 🏆**# Tower-Defense-Replica
