# Pac-Man for LandTiger Board

## Project Overview
This project brings the classic **Pac-Man** game to the **LandTiger 1768 development board**, powered by an **ARM Cortex-M3** processor. The implementation closely follows the original Pac-Man mechanics while incorporating hardware-specific features, such as:

- **Joystick controls** for movement
- **AI-driven ghost behavior**
- **Sound effects and background music**
- **CAN bus communication** for game state transmission

## Requirements

### **Extrapoint 1 - Basic Pac-Man Game**

#### **Labyrinth and Pills**
- The game renders a **maze** on the LandTiger screen, featuring exactly **240 Standard Pills**.
- A **central box** is reserved for possible future use.
- **6 Power Pills** appear at **random positions and times**.

#### **Game Mechanics**
- Pac-Man moves continuously in the **last chosen direction** until he hits a wall or receives new input.
- Players can control Pac-Man's movement using the **joystick** (left, right, up, down).
- The game speed is **optimized for smooth gameplay**.
- **Teleportation**: Pac-Man reappears on the **opposite side** of the maze when reaching a teleport point.

#### **Scoring System**
- **Standard Pills**: +10 points
- **Power Pills**: +50 points
- **Extra life** awarded every **1000 points**.

#### **Additional Features**
- **Pause Functionality**: Pressing **INT0** pauses or resumes the game.
- **Countdown Timer**: The game starts with **60 seconds**; the player must complete the level before time runs out.
- **Winning Condition**: Pac-Man wins by consuming **all pills**.
- **Losing Condition**: The countdown timer reaches **zero**.

### **Extrapoint 2 - Advanced Features**

#### **AI-Controlled Ghost (Blinky)**
- **Blinky actively chases Pac-Man**, switching between two movement strategies:
  1. **Chase Mode**: Uses an **AI algorithm** (e.g., A* pathfinding) to pursue Pac-Man.
  2. **Frightened Mode**: When Pac-Man eats a Power Pill:
     - Blinky turns **blue** and **flees** for **10 seconds**.
     - If Pac-Man catches Blinky, he earns **+100 points**.
     - Blinky **respawns** in the central box after **3 seconds**.

#### **Audio Integration**
- The speaker plays **sound effects** and **background music**.

#### **CAN Bus Communication**
- Transmits **score, remaining lives, and countdown timer** over the **CAN bus**.
- Configures **CAN1 (sender)** and **CAN2 (receiver)** for **external loopback mode**.

## Development Environment
- **Hardware**: LandTiger Board with ARM Cortex-M3
- **IDE**: Keil µVision
- **Build Targets**:
  - **SW_Debug** (for emulator testing)
  - **Physical Board** (for full hardware functionality)

## How to Build and Run
1. **Compile** the code in **Keil µVision**.
2. **Flash** the program onto the **LandTiger board**.
3. **Use the joystick** to control Pac-Man.
4. **Monitor** the game state on the **display** and **CAN bus**.
5. **Listen** to sound effects from the speaker.
