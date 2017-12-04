#Archive Format

Most of the files in the game Rockman Dash for Playstation are kept
in a ".BIN" archive format. The files inside the archive are not
compressed. As such their functionality seems to indicate a single
file that can be read from the disk to load all required assets for
a given scene.

There are a few files not contained inside of these archives. Which
seem to be files that are used globally, such as the polygons that
make up the protagonist, and the texture for the gameover screen.

##Bit Layout

|      | 0x00 | 0x04 | 0x08 | 0x0C |
|      | ---- | ---- | ---- | ---- |
| 0x00 | Type | 0000 | 0000 | 0000 |
| 0x10 | 0000 | 0000 | 0000 | 0000 |
| 0x20 | 0000 | 0000 | 0000 | 0000 |
| 0x30 | 0000 | 0000 | 0000 | 0000 |
| 0x40 | ..\O | BJ\S | T03\ | AR00 |
| 0x50 | B\EM | 0600 | .TIM |      |
