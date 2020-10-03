# Skyrim-Behavior-Editor-
![statemachine](https://user-images.githubusercontent.com/8378075/45464759-e8048880-b709-11e8-9448-0abd26b31944.png)

This tool is a GUI viewer/editor for Havok’s Behavior files utilized
by the Creation Engine powering the video game Skyrim. Havok Behavior
is used in this engine to select/manipulate actor animations in
the game.

Internally this tool uses hkxcmd to convert between packed binary
hkx and hkx xml. It parses the hkx xml files associated with a character
project, graphs all the data in each behavior file and exposes all
of the important data to the user via a number of different widgets.

<b>NOTE: As hkxcmd cannot work with SSE animation/behavior files, you will need to use the original Skyrim animation/behavior files for this tool to work! See the bottom of this readme for how to set up your project!</b>

It also parses the animation text files that contain data representing
animation translation and rotation data along with information used
by the Creation Engine’s animation caching system.

Features of the tool include:

• Open existing projects and create new projects.

• Remove or add new animations to any project.

• Append, inject and remove objects from the behavior graph via
a type-safe interface.

• View and edit the data of over 70 different Havok and Bethesda
behavior classes.

• Extensive error checking and error correction routines.

• View, edit, remove or add translations and rotations to any animation
in the project.

• View and edit the animation cache data.

• Expand or collapse branchs while viewing a behavior graph.

• Pan the view around and zoom in or out while viewing a behavior
graph.

• Locate and focus on objects via a string filterable table of object
names.

• Convert and export project or behavior files to the game directory.

• Merge projects or single behavior files together.


<b><u>SETTING UP PROJECTS USING <a href ="https://www.nexusmods.com/skyrimspecialedition/mods/974" target="_blank">BSA EXTRACTOR</a></u></b>

<b>Extract the "meshes/actors/character/" directory from the 'Update.bsa'.</b>

![update](https://user-images.githubusercontent.com/8378075/94980095-a4faf500-051e-11eb-8376-9af79e69bb1d.png)
![Update.bsa](https://user-images.githubusercontent.com/8378075/94980098-a5938b80-051e-11eb-965e-09143daf4831.png)

<b>Extract the "meshes/actors/character/animations" directory from the 'DragonBorn.bsa'.</b>

![dragonborn](https://user-images.githubusercontent.com/8378075/94980090-a4625e80-051e-11eb-81a2-eb17ad78a5a6.png)
![dragonborn.bsa](https://user-images.githubusercontent.com/8378075/94980093-a4625e80-051e-11eb-8e17-4f20bb13b36a.png)

<b>Extract the "meshes/actors/character/character assets" and "meshes/actors/character/character assets female" directories from the 'Animations.bsa'.</b>

![animations](https://user-images.githubusercontent.com/8378075/94980087-a3c9c800-051e-11eb-9984-953ef3f74ee9.png)
![animations.bsa](https://user-images.githubusercontent.com/8378075/94980088-a4625e80-051e-11eb-88e9-e92e291bcdb0.png)

<b>What the Character project directory should look like. To open the project, select either 'defaultmale.hkx' or 'defaultfemale.hkx'</b>

![character](https://user-images.githubusercontent.com/8378075/94980096-a5938b80-051e-11eb-8882-c33f0bae8fc0.png)
