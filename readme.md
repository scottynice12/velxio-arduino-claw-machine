# 🕹️ Velxio Interactive Arduino Claw Machine

A mini arcade claw machine simulation built for **Velxio** (`velxio.dev`), featuring real microcontroller logic, object-oriented C++ architecture, interactive multi-button directional controls, and authentic arcade win/loss probability mechanics.

## 🚀 Features
- **Simulated Hardware Control:** Built for an Arduino Uno board.
- **Multi-Button Controls:** Supports directional movement (Left/Right) and a drop sequence using push buttons configured with internal pull-ups (`INPUT_PULLUP`).
- **Object-Oriented Design:** Clean separation of concerns for mock motors (`MotorInterface`) and game mechanics (`ClawController`).
- **Arcade Probability Logic:** Features a randomized win/loss slip mechanic during the grab sequence.
- **Serial Debugging:** Live console logs streaming game states directly to the Velxio Serial Monitor.

---

## 🛠️ Wiring Layout (Velxio Canvas)

Wire your push buttons to the Arduino Uno as follows:

| Button Action | Arduino Uno Pin | Description |
| :--- | :--- | :--- |
| **Move Left Button** | **Digital Pin 2** | Triggers left translation |
| **Move Right Button** | **Digital Pin 3** | Triggers right translation |
| **Drop / Grab Button** | **Digital Pin 4** | Executes drop and grab routine |
| **All Buttons (Other Leg)** | **GND** | Common ground reference |

---

## 🏗️ Project Architecture
- **`Position` struct:** Tracks 3D coordinates (`x, y, z`) of the claw.
- **`MotorInterface` (Abstract Class):** Defines virtual methods for hardware axis movement and claw state management.
- **`SimulationMotors`:** Implements the motor interface to mock hardware actions and print step-by-step progress via `Serial`.
- **`ClawController`:** Manages high-level game logic, targeting sequences, and a 40% probability win/loss RNG routine.

---

## 💡 How to Run
1. Open [Velxio](https://velxio.dev).
2. Add an **Arduino Uno** board to the canvas.
3. Add push buttons and wire them to **Pins 2, 3, 4** and **GND**.
4. Paste your script into `sketch.ino`, hit **Play**, and open the **Serial Monitor** to play!
