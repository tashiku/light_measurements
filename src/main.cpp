#include<Arduino.h>
#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;  // Pin dla CS karty SD
const int ledPin = 9;  // Pin dla sztucznych źródeł światła
const int lightSensorPins[] = {A0, A1, A2, A3};  // Piny czterech czujników natężenia światła
const int numSensors = 4;  // Liczba czujników

int cycleCount = 1;
File logFile;  // Zadeklarowanie pliku globalnie

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  delay(1000);

  Serial.println("Inicjalizacja karty SD...");

  // Inicjalizacja karty SD
  if (!SD.begin(chipSelect)) {
    Serial.println("Nie można zainicjować karty SD. Sprawdź połączenia.");
    while (1);  // Zatrzymaj dalsze działanie, jeśli karta SD nie działa
  }
  
  Serial.println("Karta SD zainicjowana pomyślnie.");

  // Otwieranie pliku w trybie FILE_WRITE (jeśli plik nie istnieje, zostanie utworzony)
  logFile = SD.open("log.txt", FILE_WRITE);
  if (!logFile) {
    Serial.println("Nie można otworzyć pliku do zapisu.");
    while (1);  // Zatrzymaj dalsze działanie, jeśli plik nie może zostać otwarty
  } else {
    Serial.println("Plik otwarty pomyślnie. Gotowy do zapisu.");
  }
}

void loop() {
  if (logFile) {  // Sprawdzenie, czy plik został poprawnie otwarty
    logFile.print("Cykl ");
    logFile.println(cycleCount);
    Serial.print("Cykl ");
    Serial.println(cycleCount);

    // Zaświecenie żarówki na 30 sekund
    digitalWrite(ledPin, LOW);
    delay(10000);  // Czekaj 10 sekund

    // Odczyt wartości czujników 10 sekund po włączeniu żarówki
    for (int i = 0; i < numSensors; i++) {
      int lightOn = analogRead(lightSensorPins[i]);
      Serial.print("Czujnik ");
      Serial.print(i + 1);
      Serial.print(": Natężenie światła ON:  ");
      Serial.println(lightOn);
      logFile.print("Czujnik ");
      logFile.print(i + 1);
      logFile.print(": Natężenie światła ON:  ");
      logFile.println(lightOn);
    }

    delay(20000);  // Pozostałe 20 sekund

    // Zgaszenie żarówki na 30 sekund
    digitalWrite(ledPin, HIGH);
    delay(10000);  // Czekaj 10 sekund

    // Odczyt wartości czujników 10 sekund po wyłączeniu żarówki
    for (int i = 0; i < numSensors; i++) {
      int lightOff = analogRead(lightSensorPins[i]);
      Serial.print("Czujnik ");
      Serial.print(i + 1);
      Serial.print(": Natężenie światła OFF:  ");
      Serial.println(lightOff);
      logFile.print("Czujnik ");
      logFile.print(i + 1);
      logFile.print(": Natężenie światła OFF:  ");
      logFile.println(lightOff);
    }

    delay(20000);  // Pozostałe 20 sekund

    cycleCount++;  // Zwiększ licznik cykli po zakończeniu cyklu

    // Zapisanie danych na kartę SD
    logFile.flush();
  } else {
    Serial.println("Nie można zapisać do pliku, ponieważ nie został poprawnie otwarty.");
  }
}