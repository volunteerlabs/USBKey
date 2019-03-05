# Electronic Dumb Key Software

This program is the USB Key software that I use for a dynamically set Bad USB attack or "Electronic Dumb Key" as I'm calling it. This software, once written to an STM32F103C8 (Mini-maple) will simulate a USB keyboard typing in a username and password set by the user. The user must set the username and password prior to use.

## Getting Started

Hardware:
You will need to have a Mini-Maple board like the ones found here: https://www.amazon.com/initeq-STM32F103CBT6-USB-Programmable-Development-Compatibility/dp/B07989TGV8/ref=sr_1_37
These boards are clones of a micro-controller originally produced by LeafLabs, and now produced by Baite.

Software:
I used Arduino's IDE to flash the software. To setup your development environment instructions can be found here: https://fludwerks.com/2018/10/20/diy-usb-keys/

Once your IDE is setup, you can start flashing Mini-Maples and this program will be no different than any other microcontroller program.


## Authors

* **Lesley Gushurst** - *Initial work* - [Volunteer Research Laboratories LLC](https://volunteerlabs.us)


## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

