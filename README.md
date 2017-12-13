# Dash-Texture-Viewer

![DashGL Texture Viewer](https://github.com/kion-dgl/Dash-Texture-Viewer/blob/master/file_formats/fig/step_36.png?raw=true)

A texture viewer for "Megaman Legends" or "Rockman Dash" on the Playstation.
To use the program, hit the dropdown menu in the top left and select "Open".
From there, choose a ".BIN" file from the game disk. The names of all of the
textures in the scene will be displayed in the listbox area on the left.

To preview a texture, click on it. Each texture is comprised of an image body
and a pallet. The number of pallets for each texture are displayed as radio
buttons on the right. To cycle through the textures for a given file, click
on each radio button. 

The current texture preview with pallet selected can be exported to a PNG file.
With a texture selected, click the dropdown menu in the upper left and select
"Export" from the menu. You will be prompted to select a save location for the
PNG file.

In addition all of the textures for a given scene can be batch exported. With a
".BIN" file loaded, goto the top left dropdown menu and select "Export All".
You will be prompted to select a file location. All of the files in the scene 
will be exported as PNG files, with the name given in the ".BIN" file with
the PNG extension. In this case any files that may share the same name will
be overwritten without being prompted. The author takes no responcibility 
for any loss of data should you do something stupid.

A side note, that while mostly stable, this program has been extensively tested.
There could be compatibility issues with different textures or files or versions
of the game. In addition there could be memory leaks and other problems. 
The purposes of this software are mostly to act as a test and reference for source
code.

### Repository Structure

Some information for file types and data structures in included in the "file_formats"
folder. Command line tools for exporting TIM files from a Rockman Dash ".BIN" file
are included in the "tools" folder if you want to port the functionality to another
language / libary combination.  

The code for the user interface is included in the "src" folder. A number is kept for
each version of the code. So "00", is the eariliest version, "36" or later is the last
version. As this was my first attempt at making a general purpose GTK application I
separated out all of the steps as a reference for how the UI was tested and built.

### License

Copyright (C) 2017  Benjamin Collins [DashGL Project](https://dashgl.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
