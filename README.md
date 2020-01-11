# STM32-SelfieCpp
C++ Software for Selfie Autonomous Car low-level-HW controller (STM32 F7)

## How to contribute
### Konfiguracja gita

1) Forkuj repo na swoje konto
2) clone repo na lokalny folder
- git clone https://github.com/your-login/Selfie-Carolo2020-STM
3) skonfiguruj remote repo "main"
- cd Selfie-Carolo2020-STM
- git remote add main https://github.com/Jarzyn1/Selfie-Carolo2020-STM
4) Teraz masz skonfigurowane 2 remote repos.
- git remote -v
"main" wskazujący na Jarzyn1
"origin" wskazujący na Twój github

### Zmiany
5) Commit&push
- git commit -a -m "treść commita"
- git push origin master

A dodatkowo warto zrealizować konfigurację gita w Atollicu.

6) Pull request między forkami (z origina na main)
github.com -> pull requests

## How to program?
Korzystam z [Betaflight Configurator](https://github.com/betaflight/betaflight-configurator/releases).
Aby komputer odpowiednio wykrywał bootloader potrzebna jest podmiana sterownika DFU przy pomocy [Zadig](https://github.com/betaflight/betaflight/wiki/Installing-Betaflight#dfu-flashing-under-windows---usb-dfu).
Całość odbywa się poprzez złącze USB. 
Wystarczy w zakładce Firmware Flasher wybrać przez Load Firmware [Local] skompilowany plik .hex i kliknąć Flash Firmware. Dopóki STM pracuje poprawnie, nie trzeba nic zwierać, ani wciskać przycisków :).

## How to modify?
Trzeba pamiętać, że w obecnym wydaniu CubeMX nie ma wpływu na kluczowe pliki, m.in. main.cpp i freertos.cpp i ewentualne zmiany trzeba wprowadzać ręcznie.
Taski dodawane są ręcznie, bez klikania w MXie. Wszystkie prócz USB zebrane są w kupę o nazwie AllTasks.cpp. Ma to swoje zalety i wady :)

## How to debug?
Można kombinować ze złączem SWD. Ja sobie radzę wykorzystując USB oraz UART1 (poprzez bluetooth lub ftdi adapter).
Drukuję sobie interesujące mnie informacje. Są nawet statystyki użycia MCU przez RTOSa :)

Obecnie piszę aplikację działająca w Javie do debugowania repo: https://github.com/rvbc1/Selfie_STM32_Debugger
