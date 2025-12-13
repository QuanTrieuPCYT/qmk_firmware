# evo80

A customizable 88key keyboard.

![evo80](https://github.com/user-attachments/assets/3f05bc33-f93a-46b8-849c-274705aea193)


* Keyboard Maintainer: [QuanTrieuPCYT](https://github.com/QuanTrieuPCYT)
* Hardware Supported: evo80 PCB with es32fs026 microcontroller

Make example for this keyboard (after setting up your build environment):

    make evoworks/evo80:default
    
To compile with VIA support:

    make evoworks/evo80:via

Flashing example for this keyboard:

    make evoworks/evo80:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader:

* **Bootmagic reset**: Hold down the key at (0,0) in the matrix (Esc key) and plug in the keyboard
