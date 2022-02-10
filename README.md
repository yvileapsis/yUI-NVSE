
<p align="center">
  <a href="https://github.com/xNVSE/NVSE/releases">
    <img height="300px" src="https://cdn.discordapp.com/attachments/724924815869345803/903218762751299624/660209289637396483.png">
  </a>
</p>

This is an yvilemunity yviletained **New Vegas Script Extender** (NVSE) plugin. 

## Support

For support and bug reports, contact me in the [xNVSE Discord server](https://discord.gg/EebN93s).

## Credits

This NVSE plugin is developed by yvileapsis with liberal borrowing of code from lStewieAl's Tweaks by lStewieAl and kNVSE by korri123.

This plugin wouldn't exist without efforts of many community members such as LuthienAnarion, JazzIsParis, lStewieAl, c6, carxt, korri123 and Demorome and many others.

# yUI-NVSE
NVSE plugin aiming to fix UI issues and add additional UI features.

# yGTM-NVSE
NVSE plugin aiming to resolve issues with global time multiplier. 

hooks a few places, changes a few functions

Contains code from lStewieAl's Tweaks, nvse-example-plugin and JIP-LN-NVSE.

(float) GetGlobalTimeMultiplier *modIndex:int*

SetGlobalTimeMultiplier *multiplier:float*

modIndex:

    -1    - Global Time Multiplier (vanilla JIP behavior)
    
    0     - Default - Local Time Multiplier of the calling mod.
     
    1-254 - Local Time Multiplier of the mod at the corresponding index.
  
    255   - Console.
    
    256   - Killcam.
    
    257   - Turbo.
    
multiplier:

     -1   - Removes all Local Time Multipliers.
    
     0    - Default - Does nothing. (No crash)
     
     1    - Removes Local Time Multiplier of the current mod.
