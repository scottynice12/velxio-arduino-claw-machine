# 🕹️ Velxio Interactive Arduino Claw Machine

A mini arcade claw machine simulation built for **Velxio** (`velxio.dev`), featuring real microcontroller logic, object-oriented C++ architecture, and interactive push-button inputs.

## 🚀 Features
- **Simulated Hardware Control:** Built for an Arduino Uno board.
- **Interactive Input:** Real-time push-button integration configured with internal pull-up resistors (`INPUT_PULLUP`).
- **Object-Oriented Design:** Uses clean separation of concerns for mock motors (`MotorInterface`) and game mechanics (`ClawController`).
- **Serial Debugging:** Live console logs streaming game states directly to the Velxio Serial Monitor.

---

## 🛠️ Wiring Layout (Velxio Canvas)

To run the interactive controls, wire your push button as follows:

| Component Pin | Arduino Uno Pin | Description |
| :--- | :--- | :--- |
| **Button Left Leg** | **Digital Pin 2** | Signal line (Left/Move trigger) |
| **Button Right Leg** | **GND** | Ground reference |

---

## 📂 Code (`sketch.ino`)

The entire project is packaged into a clean, single-file Arduino sketch for easy loading and saving:

```cpp
// Simple Object Structure for Position
struct Position {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

// Motor Interface
class MotorInterface {
public:
    virtual ~MotorInterface() {}
    virtual bool moveAxis(char axis, double targetPosition) = 0;
    virtual void setClawState(bool open) = 0;
    virtual Position getCurrentPosition() const = 0;
};

// Simulation Motors implementation
class SimulationMotors : public MotorInterface {
private:
    Position currentPos;
    bool clawIsOpen;

public:
    SimulationMotors() { clawIsOpen = true; }

    bool moveAxis(char axis, double targetPosition) override {
        Serial.print("[Hardware] Moving axis ");
        Serial.print(axis);
        Serial.print(" to ");
        Serial.print(targetPosition);
        Serial.println("...");
        delay(100); 
        
        if (axis == 'X') currentPos.x = targetPosition;
        else if (axis == 'Y') currentPos.y = targetPosition;
        else if (axis == 'Z') currentPos.z = targetPosition;
        
        return true;
    }

    void setClawState(bool open) override {
        clawIsOpen = open;
        Serial.print("[Hardware] Claw state updated: ");
        Serial.println(clawIsOpen ? "OPEN" : "CLOSED");
    }

    Position getCurrentPosition() const override { return currentPos; }
};

// Claw Controller Logic
class ClawController {
private:
    MotorInterface* hardwareMotors;
    bool isHoldingPrize;

public:
    ClawController(MotorInterface* motors) {
        hardwareMotors = motors;
        isHoldingPrize = false;
    }

    bool targetDestination(double targetX, double targetY) {
        Serial.println("[Controller] Navigating to target coordinates...");
        bool movedX = hardwareMotors->moveAxis('X', targetX);
        bool movedY = hardwareMotors->moveAxis('Y', targetY);
        return (movedX && movedY);
    }

    void executeDropAndGrab() {
        Serial.println("[Controller] Opening claw...");
        hardwareMotors->setClawState(true);
        delay(400);

        Serial.println("[Controller] Lowering claw (Z-axis)...");
        hardwareMotors->moveAxis('Z', -10.0);
        delay(400);

        Serial.println("[Controller] Closing claw to grab prize...");
        hardwareMotors->setClawState(false);
        isHoldingPrize = true; 
        delay(400);

        Serial.println("[Controller] Raising claw back up...");
        hardwareMotors->moveAxis('Z', 0.0);
        delay(400);
        
        Serial.println("[Controller] Drop and grab sequence completed successfully!");
    }

    bool hasPrize() { return isHoldingPrize; }
};

const int PIN_LEFT = 2;
const int PIN_DROP = 4;

SimulationMotors myMotors;
ClawController* claw = nullptr;

void setup() {
    Serial.begin(115200);
    delay(500); 
    
    pinMode(PIN_LEFT, INPUT_PULLUP);
    pinMode(PIN_DROP, INPUT_PULLUP);

    claw = new ClawController(&myMotors);
    Serial.println("=== 🕹️ Interactive Claw Machine Ready! ===\n");
}

void loop() {
    if (digitalRead(PIN_LEFT) == LOW) {
        Serial.println("Button Pressed: Moving Left!");
        claw->targetDestination(10.0, 12.0);
        delay(300);
    }

    if (digitalRead(PIN_DROP) == LOW) {
        Serial.println("Button Pressed: Drop & Grab Sequence!");
        claw->executeDropAndGrab();
        delay(500);
    }
}
