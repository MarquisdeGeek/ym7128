# ym7128
An Arduino library to handle the Yamaha digital delay chip, YM7128

## Circuit
Connect the chip's SCI, A0, and Din pins to Arduino pins 5, 6, 7, respectively.

Add a button to pin 2, tied to ground and pulled high when pressed. Then upload.

See the schematic at http://www.dtech.lv/files_ym/ym7128.pdf if you're unsure of
how this chip works.

The basic demo include 4 basic patterns.

## Dependency

Uses the Armstrong library (https://github.com/MarquisdeGeek/armstrong) to handle button debounce.
