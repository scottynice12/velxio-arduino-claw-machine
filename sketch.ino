// 1. Position struct
struct Position {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

// 2. Motor Interface
class MotorInterface {
public:
    virtual ~MotorInterface() {}
    virtual bool moveAxis(char axis, double targetPosition) = 0;
    virtual void setClawState(bool open) = 0;
    virtual Position getCurrentPosition() const = 0;
};

// 3. Simulation Motors implementation
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

// 4. Claw Controller Logic
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

// 5. Define Pins and Global Variables *after* classes are declared
const int PIN_LEFT = 2;
const int PIN_RIGHT = 3;
const int PIN_DROP = 4;

SimulationMotors myMotors;
ClawController* claw = nullptr;

void setup() {
    Serial.begin(115200);
    delay(500); 
    
    // Configure button pins with internal pull-ups (press = LOW)
    pinMode(PIN_LEFT, INPUT_PULLUP);
    pinMode(PIN_RIGHT, INPUT_PULLUP);
    pinMode(PIN_DROP, INPUT_PULLUP);

    claw = new ClawController(&myMotors);
    Serial.println("=== 🕹️ Interactive Claw Machine Ready! Use Buttons on Pins 2, 3, 4 🕹️ ===\n");
}

void loop() {
    // Check if "Move Left" button (Pin 2) is pressed
    if (digitalRead(PIN_LEFT) == LOW) {
        Serial.println("Button Pressed: Moving Left!");
        claw->targetDestination(10.0, 12.0);
        delay(300); // Debounce delay
    }

    // Check if "Move Right" button (Pin 3) is pressed
    if (digitalRead(PIN_RIGHT) == LOW) {
        Serial.println("Button Pressed: Moving Right!");
        claw->targetDestination(40.0, 12.0);
        delay(300);
    }

    // Check if "Drop" button (Pin 4) is pressed
    if (digitalRead(PIN_DROP) == LOW) {
        Serial.println("Button Pressed: Drop & Grab Sequence!");
        claw->executeDropAndGrab();
        delay(500);
    }
}
