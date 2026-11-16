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
    SimulationMotors() {
        clawIsOpen = true;
    }

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

    Position getCurrentPosition() const override {
        return currentPos;
    }
};

// Claw Controller Logic with Win/Loss Mechanics
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
        delay(400);

        Serial.println("[Controller] Raising claw back up...");
        hardwareMotors->moveAxis('Z', 0.0);
        delay(400);
        
        // Arcade win/loss probability logic (40% win chance)
        int randomChance = random(1, 101); 
        if (randomChance <= 40) {
            isHoldingPrize = true;
            Serial.println("🎉 YOU WON A PRIZE! 🧸✨ Claw held on successfully!");
        } else {
            isHoldingPrize = false;
            Serial.println("💨 Oh no! The toy slipped out of the claw! Try again.");
        }
    }

    bool hasPrize() { return isHoldingPrize; }
};

// Define Pins for Left, Right, and Drop buttons
const int PIN_LEFT = 2;
const int PIN_RIGHT = 3;
const int PIN_DROP = 4;

SimulationMotors myMotors;
ClawController* claw = nullptr;

void setup() {
    Serial.begin(115200);
    delay(500); 
    
    // Configure all button pins with internal pull-ups (press = LOW)
    pinMode(PIN_LEFT, INPUT_PULLUP);
    pinMode(PIN_RIGHT, INPUT_PULLUP);
    pinMode(PIN_DROP, INPUT_PULLUP);

    randomSeed(analogRead(A0));

    claw = new ClawController(&myMotors);
    Serial.println("=== 🕹️ Interactive Claw Machine Ready! Use Pins 2 (Left), 3 (Right), 4 (Drop) 🕹️ ===\n");
}

void loop() {
    // 1. Move Left Button (Pin 2)
    if (digitalRead(PIN_LEFT) == LOW) {
        Serial.println("\nButton Pressed: Moving Left!");
        claw->targetDestination(10.0, 12.0);
        delay(300); 
    }

    // 2. Move Right Button (Pin 3)
    if (digitalRead(PIN_RIGHT) == LOW) {
        Serial.println("\nButton Pressed: Moving Right!");
        claw->targetDestination(40.0, 12.0);
        delay(300); 
    }

    // 3. Drop Button (Pin 4)
    if (digitalRead(PIN_DROP) == LOW) {
        Serial.println("\nButton Pressed: Drop & Grab Sequence!");
        claw->executeDropAndGrab();
        delay(500);
    }
}
