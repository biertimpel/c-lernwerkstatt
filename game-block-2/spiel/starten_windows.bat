@echo off
cd /d "%~dp0"
where gcc >nul 2>nul
if errorlevel 1 (
  echo GCC nicht gefunden. START_HIER.html im uebergeordneten Ordner bietet die Browser-Vorschau.
  pause
  exit /b 1
)
gcc -std=c17 -Wall -Wextra -Wpedantic -Iinclude src/main.c src/spiel.c src/objekt.c src/inventar.c src/eingabe.c src/speichern.c src/anzeige.c -o tresorraub.exe
if errorlevel 1 (
  pause
  exit /b 1
)
tresorraub.exe
pause
