#define BZ 4
#define btn1 16
#define btn2 17
#define btn3 18
#include <pitches.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

// Jingle Bells
int melody[] = {
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
  NOTE_D5, NOTE_G5
};

int tempo[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8,
  2,
  8, 8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4
};

// We Wish You a Merry Christmas
int wish_melody[] = {
  NOTE_B3,
  NOTE_F4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4,
  NOTE_D4, NOTE_D4, NOTE_D4,
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4,
  NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_A4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_D4, NOTE_B3, NOTE_B3,
  NOTE_D4, NOTE_G4, NOTE_E4,
  NOTE_F4
};

int wish_tempo[] = {
  4,
  4, 8, 8, 8, 8,
  4, 4, 4,
  4, 8, 8, 8, 8,
  4, 4, 4,
  4, 8, 8, 8, 8,
  4, 4, 8, 8,
  4, 4, 4,
  2
};

// Santa Claus is Coming to Town
int santa_melody[] = {
  NOTE_G4,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, NOTE_C5,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_G4, NOTE_F4, NOTE_F4,
  NOTE_E4, NOTE_G4, NOTE_C4, NOTE_E4,
  NOTE_D4, NOTE_F4, NOTE_B3,
  NOTE_C4
};

int santa_tempo[] = {
  8,
  8, 8, 4, 4, 4,
  8, 8, 4, 4, 4,
  8, 8, 4, 4, 4,
  8, 8, 4, 2,
  4, 4, 4, 4,
  4, 2, 4,
  1
};

int switchOne = 0;
int switchTwo = 0;
int switchThree = 0;

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2); 
  lcd.println("Feliz Navidad   ");
  lcd.setCursor(0, 1);
  lcd.println("UTNG            ");
  pinMode(BZ, OUTPUT);
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);
}

void loop() {
  switchOne = digitalRead(btn1);
  switchTwo = digitalRead(btn2);
  switchThree = digitalRead(btn3);
  if (switchOne == LOW) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("'Jingle Bells'");
    sing(1);
    lcd.clear();
    lcd.println("Feliz Navidad   ");
    lcd.setCursor(0, 1);
    lcd.println("UTNG            ");
  } else if (switchTwo == LOW) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println("'Santa Claus is ");
    lcd.setCursor(0, 1);
    lcd.println("coming to town' ");
    sing(2);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println("Feliz Navidad   ");
    lcd.setCursor(0, 1);
    lcd.println("UTNG            ");
  } else if (switchThree == LOW) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println("'We wish you a  ");
    lcd.setCursor(0, 1);
    lcd.println("Merry Christmas'");
    sing(3);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println("Feliz Navidad   ");
    lcd.setCursor(0, 1);
    lcd.println("UTNG            ");
  }
}

int song = 0;

void sing(int s) {
  song = s;

  if (song == 3) {
    Serial.println(" 'We wish you a Merry Christmas'");
    playMelody(wish_melody, wish_tempo, sizeof(wish_melody) / sizeof(int));
  } else if (song == 2) {
    Serial.println(" 'Santa Claus is coming to town'");
    playMelody(santa_melody, santa_tempo, sizeof(santa_melody) / sizeof(int));
  } else {
    Serial.println(" 'Jingle Bells'");
    playMelody(melody, tempo, sizeof(melody) / sizeof(int));
  }
}

void playMelody(int *melodyArray, int *tempoArray, int size) {
  for (int thisNote = 0; thisNote < size; thisNote++) {
    int noteDuration = 1000 / tempoArray[thisNote];
    buzz(melodyArray[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    buzz(0, noteDuration);
  }
}

void buzz(long frequency, long length) {
  if (frequency == 0) {
    digitalWrite(BZ, LOW);
    delay(length);
    return;
  }
  
  long delayValue = 1000000 / frequency / 2;
  long numCycles = frequency * length / 1000;

  for (long i = 0; i < numCycles; i++) {
    digitalWrite(BZ, HIGH);
    delayMicroseconds(delayValue);
    digitalWrite(BZ, LOW);
    delayMicroseconds(delayValue);
  }
}