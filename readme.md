# cutiepc's Evoworks Evo80 QMK Firmware

**WARNING: Use this firmware at your own risk! I am not responsible if your keyboard breaks after flashing this firmware!**

This repo holds the codebase of the firmware of my Evoworks Evo80. Changes have been made on top of the manufacturer's codebase to suit my personal needs, including:
- Changed project structure to be more clean
- Made adjustment to default keymaps to better suit my needs (so I can start using the board right away after flashing!)
- Caps Lock and Scroll Lock indicators now extend to the RGB Logo strip
- Implemented Layer Lock, with yellow-ish indicator at the Layer Lock key and the RGB Logo strip
- Implemented Word Select, with respect to Mac machines when keyboard is set to Mac mode
- Battery indicator now also shows from the Q key to the P key (10 LEDs), and also the RGB strip (the strip shows the progress, the circle LED mirrors the state of the Esc key)
- Enabled link-time optimizations, reworked and refactored code and logic for better performance. Excluded functions that would break when the firmware is compiled with LTO from being inlined
- Implemented logic for extra configurations (light toggles, debounce configuration, NKRO, Windows key Lock and Mac mode) to the VIA configuration GUI (new channel logics and custom config values). Changes are live on my personal forks of VIA's [keyboards](https://github.com/QuanTrieuPCYT/via-keyboards) and [app](https://github.com/QuanTrieuPCYT/via-app) repositories

To-do:
- Properly reverse engineer and draft a fully open sourced implementation of the `rdr_common` library.

Below is the original description of the QMK Firmware repository:

# Quantum Mechanical Keyboard Firmware

[![Current Version](https://img.shields.io/github/tag/qmk/qmk_firmware.svg)](https://github.com/qmk/qmk_firmware/tags)
[![Discord](https://img.shields.io/discord/440868230475677696.svg)](https://discord.gg/Uq7gcHh)
[![Docs Status](https://img.shields.io/badge/docs-ready-orange.svg)](https://docs.qmk.fm)
[![GitHub contributors](https://img.shields.io/github/contributors/qmk/qmk_firmware.svg)](https://github.com/qmk/qmk_firmware/pulse/monthly)
[![GitHub forks](https://img.shields.io/github/forks/qmk/qmk_firmware.svg?style=social&label=Fork)](https://github.com/qmk/qmk_firmware/)

This is a keyboard firmware based on the [tmk\_keyboard firmware](https://github.com/tmk/tmk_keyboard) with some useful features for Atmel AVR and ARM controllers, and more specifically, the [OLKB product line](https://olkb.com), the [ErgoDox EZ](https://ergodox-ez.com) keyboard, and the [Clueboard product line](https://clueboard.co).

## Documentation

* [See the official documentation on docs.qmk.fm](https://docs.qmk.fm)

The docs are powered by [Docsify](https://docsify.js.org/) and hosted on [GitHub](/docs/). They are also viewable offline; see [Previewing the Documentation](https://docs.qmk.fm/#/contributing?id=previewing-the-documentation) for more details.

You can request changes by making a fork and opening a [pull request](https://github.com/qmk/qmk_firmware/pulls), or by clicking the "Edit this page" link at the bottom of any page.

## Supported Keyboards

* [Planck](/keyboards/planck/)
* [Preonic](/keyboards/preonic/)
* [ErgoDox EZ](/keyboards/ergodox_ez/)
* [Clueboard](/keyboards/clueboard/)
* [Cluepad](/keyboards/clueboard/17/)
* [Atreus](/keyboards/atreus/)

The project also includes community support for [lots of other keyboards](/keyboards/).

## Maintainers

QMK is developed and maintained by Jack Humbert of OLKB with contributions from the community, and of course, [Hasu](https://github.com/tmk). The OLKB product firmwares are maintained by [Jack Humbert](https://github.com/jackhumbert), the Ergodox EZ by [ZSA Technology Labs](https://github.com/zsa), the Clueboard by [Zach White](https://github.com/skullydazed), and the Atreus by [Phil Hagelberg](https://github.com/technomancy).

## Official Website

[qmk.fm](https://qmk.fm) is the official website of QMK, where you can find links to this page, the documentation, and the keyboards supported by QMK.