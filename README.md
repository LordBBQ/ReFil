# ReFil
Open source DIY, affordable 3D Printer filament extruder.

![render1](https://github.com/user-attachments/assets/43c00b16-5836-43ca-93c3-6fa36670307e)

### How it works:
1. Raw pellets are fed through a hopper into the extrusion tube.
2. The pellets are melted down through an auger bit and pushed through a custom CNC milled nozzle
3. The filament passes through two fans to cool it
4. It passes through a diameter sensor which controls the puller speed to keep consistent diameter
5. A gantry aligns the filament on the roll
6. Finally, the spooler rotates and spools the filament

### Notes:
Much of this project was completed outside of HackClub, but I completed much of the final assembly in HackClub.
This component was ENTIRELY custom designed (with a few reference models like the electronics) and custom manufactured involving:
- CNC routing on my custom CNC router (PCBs and metal parts)
- Lasercutting (I used a laser cutter I have access to to lasercut all the panels)
- 3D printing (Corner parts, all other parts made on my Voron 2.4)

### CAD:
Due to the huge size of the CAD model, I cannot realistically upoad it. However, I will provide a link to view the CAD online.
https://myhub.autodesk360.com/ue2cd6b4d/g/shares/SH30dd5QT870c25f12fc5767036ad2bc12d9 (may take a little while to load)

### Features and Specs
- 3D printed and lasercut custom frame
- RAMPS 1.6 and Arduino Mega based controller
- Stepper motor drive for spooler, gantry, and puller
- Custom LCD display mount using an i2c LCD, with a custom button controller
- Custom motor protection board
- 350w 24v meanwell powersupply
- Custom machined 3d nozzle
- Custom machined heater blocks (4x 40w for 160w max)
- Custom PID heat control with thermal runaway, max/mintemp detection

### Other notes:
This design is still not entirely done. Like almost all projects, it will never be perfect, and improvement is always an option. Some of the ways this can be improved:
- The reliability of the extrusion is not ideal and needs more testing
- The extrusion code needs tuning and a little further development
- I would have preferred matte black acrylic which would have looked better, but was unable to source it for a reasonable cost
- I would have liked to have added lighting, and may do so in the future

### Software:
This project uses much software that I custom developed. This can be viewed in this GitHub Repository.


![IMG_2927](https://github.com/user-attachments/assets/6fa8e068-d86c-469d-8fe9-0a4171f980c4)
![IMG_2924](https://github.com/user-attachments/assets/e3ed47f9-d134-4d1e-a631-5a6761f4825a)
