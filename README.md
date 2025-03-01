# Introduction :
Wakeme is an alarm clock based on the Arduino nano board. I built this clock to include some features that are not present in smartphone apps or alarm clocks. I wanted my alarm clock to be a separate device so I don't have to worry about charging my phone. It was also a personal challenge to see if I can build a clock using components I already had without having to get new electronic components for this project.
Over the course of developing this clock I learned how to use cad software to create a custom case for housing my electronics and the challenges involved in 3d printing it. This was a great learning exercise and I'm happy with the end product which I have developed after several iterations.

# Features of the clock that are different from other alarm clocks:
1)  Bi-hourly reminders : When this function is turned on, it reminds the user about the passage of time by beeping every 30 mins. I included this feature because I forget what time it is. It helps me to reschedule my work and stay on track.
2) Night mode : This is similar to the Do not disturb (DND) feature present on smartphones. When this is enabled, the clock does not make any sound. It reduces the brightness of the screen so that it does not light up the entire room at night time. This does not disable the alarm functionality of the clock and it will ring if the time has been set and the user. It also suppresses button sounds. (When any of the buttons are pressed, the clock beeps to let the user know that they have pressed it. This is turned off in the case of night mode.)
3) Alarm behaviour : Regular alarms turn off after a minute or ring every 5 or 2 minutes if the user does not wake up. I have added a feature where the alarm does not turn off until I press the button. This means the alarm can ring forever.
# Detailed demonstration:
I have uploaded a YouTube video to demonstrate all the features of the clock and how it is meant to be operated.

[![Click this link](https://github.com/karthik4j/arduino-clock/blob/main/pictures-of-clock/top.jpg?raw=true)](https://www.youtube.com/watch?v=m_OycEsYVDI)

# Components used :
1) Arduino Nano board
2) DS1307 RTC module
3) 16x2 LCD display
4) 12C module for driving the display
5) 5V active buzzer
6) Prototype PCB board
7) Panel mount push buttons x 3
8) Wires for connecting the components

# CAD files :
I designed the case using FreeCAD. The Savefile and the STL files are included in this repository. I am using the Creality Ender 3 printer to print the files using PLA filament.

# Assembly :
All the components are connected together using 24 AWG wires. The connections on the PCB are made using 30 AWG solid core wires. I am using M3 nuts and bolts to assemble the two halves and the LCD screen.
