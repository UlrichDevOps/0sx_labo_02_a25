//DA - 2372368
unsigned long currentTime = 0;
int pinBouton = 2;
int ledsPins[] = { 8, 9, 10, 11 };
int ledsPinsSize = sizeof(ledsPins) / sizeof(int);
int minScale = 0;
int maxScale = 20;
int minProgression = 0;
int maxProgression = 100;
int minPWM = 0;
int maxPWM = 1023;
int potentiometrePin = A1;
int potentiometreValue = 0;


void setup() {
  Serial.begin(9600);
  pinMode(pinBouton, INPUT_PULLUP);
  for (int i = 0; i < ledsPinsSize; i++) {
    pinMode(ledsPins[i], OUTPUT);
  }
  Serial.println("Setup completed");
}

void loop() {
  currentTime = millis();
  int value = lirePotentiometre();
  AllumerLeds(value);
  if (detecterClic(currentTime)) {
    afficherProgression(value);
  }
}

int lirePotentiometre() {
  potentiometreValue = analogRead(potentiometrePin);
  int mappedIndex = map(potentiometreValue, minPWM, maxPWM, minScale, maxScale);
  return mappedIndex;
}

void AllumerLeds(int valeur) {
  int mappedIndex = map(valeur, minScale, maxScale, 0, ledsPinsSize - 1);

  for (int i = 0; i < ledsPinsSize; i++) {
    if (i <= mappedIndex) {
      digitalWrite(ledsPins[i], HIGH);
    } else {
      digitalWrite(ledsPins[i], LOW);
    }
  }
}

// Fonction de détection de clic avec anti-rebond
bool detecterClic(unsigned long ct) {
  static int lastState = HIGH;
  static int state = HIGH;
  const int rate = 50;
  static unsigned long previousTime = 0;

  int currentState = digitalRead(pinBouton);
  if (currentState != lastState) {
    previousTime = ct;
  }

  if ((ct - previousTime) > rate) {
    if (currentState != state) {
      state = currentState;

      if (state == LOW) {
        lastState = currentState;
        return true;
      }
    }
  }
  lastState = currentState;
  return false;
}

void afficherProgression(int potentiometerValue) {
  int pourcent = map(potentiometerValue, minScale, maxScale, minProgression, maxProgression);
  Serial.print("[");

  for (int i = minScale; i < maxScale; i++) {
    if (i < pourcent / 5) {
      Serial.print("#");
    } else {
      Serial.print(".");
    }
  }

  Serial.print("] ");
  Serial.print(pourcent);
  Serial.println("%");
}

