
<p align="center">
  <a href="https://github.com/xNVSE/NVSE/releases">
    <img height="300px" src="https://cdn.discordapp.com/attachments/724924815869345803/903218762751299624/660209289637396483.png">
  </a>
</p>

This is a repository of **New Vegas Script Extender** (NVSE) plugins developed and maintained by me, yvileapsis.

This repository consists of multiple projects, external libraries and NVSE files, allowing for interaction with NVSE API and game's classes.

## Support

For support and bug reports, contact me in the [xNVSE Discord server](https://discord.gg/EebN93s).

## Credits

These NVSE plugins are developed by yvileapsis with liberal borrowing of code from lStewieAl's Tweaks by lStewieAl and kNVSE by korri123.

These plugins wouldn't exist without efforts of many community members such as LuthienAnarion, JazzIsParis, lStewieAl, c6, carxt, korri123 and Demorome and many others.

## yUI
NVSE plugin aiming to fix issues and add additional features. Currently includes several bugfixes and Sorting and Icons module, Time Multiplier module and others.

## Sorting and Icons module

## Time Multiplier module
This module aims to resolve issues with global time multiplier. Mainly this is achieved by adding local time multipliers for each mod. What this means is that every time multiplier change goes through yUI and yUI decides whether to alter time multiplier or leave it be based on new arguments of SGTM.

(float) GetGlobalTimeMultiplier *modIndex:int*

SetGlobalTimeMultiplier *multiplier:float* *immediateChange:int* *modIndex:int*

modIndex:

    -1    - Global Time Multiplier (vanilla JIP behavior)
    
    0     - Default - Local Time Multiplier of the calling mod.
     
    1-254 - Local Time Multiplier of the mod at the corresponding index.
  
    255   - Console.
    
    256   - Killcam.
    
    257   - Turbo.
    
multiplier:
    
     0    - Default - Removes all Local Time Multipliers. (No crash)
     
     1    - Removes Local Time Multiplier of the current mod.
