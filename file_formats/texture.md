# Texture Image Map

Texture Image Map or "TIM" files are the texture format for the 
Playstation's memory bugger. Rockman Dash uses this file format
as well, but the file header has been adjusted to use the format
of the file archive. The table below shows the byte layout for
how the attributes are defined in the archive.


|   Offset   | 0x00 | 0x04 | 0x08 | 0x0C |
|------|------|------|------|------|
| 0x00 | 1 | Size | 0000 | 0000 |
| 0x10 | 0000 | Nb Pallet Colors | Nb Pallets | 0000 |
| 0x20 | 0000 | Width | Height | 0000 |
| 0x20 | 0000 | 0000 | 0000 | 0000 |
| 0x30 | 0000 | 0000 | 0000 | 0000 |
| 0x40 | ..\O | BJ\S | T03\ | AR00 |
| 0x50 | B\EM | 0600 | .TIM |      |
