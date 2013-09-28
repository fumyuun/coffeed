coffeed
=======

A daemon to organize a coffee retrieval pool using joystick buttons for everyone

The system works as follows:
* A user wants coffee and presses a button.
* A timer starts, allowing colligues to press their button as well.
* At the end of the timer, a random person from the coffee pool is chosen to get coffee for everyone.

The idea is simple, however using an actual button can add some fun to it! Don't think limited by actual joystick devices; get (or hack together) a PCB that is recognised as a joystick. Build some buttons in nice boxes, or desks! Connect them to the PCB, and you're good to go.

Requirements: a linux system with a joystick recognised by the basic joystick driver. It might work on other unix systems too; I don't know if their joystick drivers work the same or not. Outputting currently uses ansi escape codes, so your terminal should support this too.