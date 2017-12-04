# Archive Format

Most of the files in the game Rockman Dash for Playstation are kept
in a ".BIN" archive format. The files inside the archive are not
compressed. As such their functionality seems to indicate a single
file that can be read from the disk to load all required assets for
a given scene.

There are a few files not contained inside of these archives. Which
seem to be files that are used globally, such as the polygons that
make up the protagonist, and the texture for the gameover screen.

## Byte Layout

|   Offset   | 0x00 | 0x04 | 0x08 | 0x0C |
|------|------|------|------|------|
| 0x00 | Type | Size | 0000 | 0000 |
| 0x10 | 0000 | 0000 | 0000 | 0000 |
| 0x20 | 0000 | 0000 | 0000 | 0000 |
| 0x20 | 0000 | 0000 | 0000 | 0000 |
| 0x30 | 0000 | 0000 | 0000 | 0000 |
| 0x40 | ..\O | BJ\S | T03\ | AR00 |
| 0x50 | B\EM | 0600 | .TIM |      |

The table above shows how the header of each file inside the .bin
archive. Files are organized such that headers appear in offsets of
0x400 bytes. The first dword declares an int for the file type.
The second dword gives the size of the embedded file in bytes. Though for
some files, this value will not always be correct.
0x40 - 0x60 gives the filename. The bytes 0x08 through 0x40 will define
different attributes depending on the file type declared. In the case of 
TIM files, pallets may be declared at the offset of
0x100. The actual data of all of types will start at 0x800. 

## File Types


| Type  | Extension | Explanation |
|------|------|------|
| 0 | .EBD | Enemy / Environment Polygon Data |
| 0 | .PBD | Player Polygon Data |
| 1 | .TIM | "Texture Image Map" |
