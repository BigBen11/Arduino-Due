# Arduino Due Prototyp-Auto-Steuerung

## Projektübersicht

Dieses Projekt umfasst die Entwicklung eines Prototyp-Autos, das von einem Arduino Due gesteuert wird. Das System integriert verschiedene Funktionen, darunter Motorsteuerung, sensorbasierte Hinderniserkennung und einen Prototypen eines Einparkassistenten. Das Projekt demonstriert verschiedene Aspekte von eingebetteten Systemen und Echtzeitsteuerung.

### Wichtige Funktionen
- **Ultraschallsensor-Messungen**: Echtzeit-Distanzmessungen mit Hilfe von Ultraschallsensoren zur Erkennung von Hindernissen.
- **Motorsteuerung (PWM)**: Die Geschwindigkeit und Richtung des Motors werden durch Pulsweitenmodulation (PWM) gesteuert.
- **Parkassistenzsystem (PID-Regler)**: Ein Prototyp eines Einparkassistenten wird durch einen PID-Regler implementiert, um präzise Abstandsregelungen zu ermöglichen.

### Technische Details
- **Programmiersprache**: Embedded C
- **Mikrocontroller**: Arduino Due für die Hauptsteuerung
- **Debugging**: Raspberry Pi Pico wurde für Debugging-Zwecke eingesetzt
