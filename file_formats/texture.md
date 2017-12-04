# Texture Image Map

Texture Image Map or "TIM" files are the texture format for the 
Playstation's memory bugger. A detailed explanation of their format
can be found at [this wiki](http://wiki.qhimm.com/view/PSX/TIM_format).
Rockman Dash uses this file format
as well, but the file header has been adjusted to use the format
of the file archive. The table below shows the byte layout for
how the attributes are defined in the archive.

## Byte Layout

|   Offset   | 0x00 | 0x04 | 0x08 | 0x0C |
|------|------|------|------|------|
| 0x00 | 1 | Size | 0x0A or 0x12 | Frame Buffer Pallet X |
| 0x10 | Frame Buffer Pallet Y | Number Pallet Colors | Number Pallets | Frame Buffer Image X |
| 0x20 | Frame Buffer Image Y | Width | Height | 0000 |
| 0x20 | 0000 | 0000 | 0000 | 0000 |
| 0x30 | 0000 | 0000 | 0000 | 0000 |

As stated in the Archive document, the first dword is defined as
**1** for the int type of TIM. The second dword is the size in bytes
of the image file in the archive. The following dword at 0x08 has
the value of either 0x0A or 0x12, but it's unknown what it's functionality
at this time.  

The dword at 0x0C is the framebuffer pallet X, and the dword at 0x10 is the
framebuffer pallet Y. At 0x14 is the number of pallet colors, which can be
0x10 for 16 colors (4 bit indexed) or 0x100 for 256 colors (8 bit indexed).
The dword at 0x18 is the number of pallets. These pallets are used to swap 
out pallets for different types of enemies, or used to adjust lighting levels. 

The dword at 0x1C is the Image buffer X, and 0x20 is Image buffer Y. The dword
at 0x24 is the width as defined in words. So if the Number of Pallet colors
is 256, then the width in pixels is multiplied by two. If the Number of Pallet
colors is 16, then the width in pixels is multiplied by four. Lastly the
height is defined at 0x28.

## Framebuffer

![alt text](https://github.com/kion-dgl/Dash-Texture-Viewer/blob/master/file_formats/fig/frame_buffer.png?raw=true "Framebuffer")
