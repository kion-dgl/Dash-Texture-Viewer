# Texture Image Map

Texture Image Map or "TIM" files are the texture format for the 
Playstation's memory bugger. Rockman Dash uses this file format
as well, but the file header has been adjusted to use the format
of the file archive. The table below shows the byte layout for
how the attributes are defined in the archive.


|   Offset   | 0x00 | 0x04 | 0x08 | 0x0C |
|------|------|------|------|------|
| 0x00 | 1 | Size | 0000 | Frame Buffer Pallet X |
| 0x10 | Frame Buffer Pallet Y | Number Pallet Colors | Number Pallets | Frame Buffer Image X |
| 0x20 | Frame Buffer Image Y | Width | Height | 0000 |
| 0x20 | 0000 | 0000 | 0000 | 0000 |
| 0x30 | 0000 | 0000 | 0000 | 0000 |
