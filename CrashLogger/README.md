This is a fork of [DLL](https://github.com/DavidJCobb/tes4-crash-logger) with modifications by me.

This version of Crash Logger comes equipped with a mini-library allowing for on the fly logging of game RTTI classes. It continuously goes through registry and stack and logs any RTTI class it can find.
Additionally, this version picks up PDB information regarding which source files do functions come from.