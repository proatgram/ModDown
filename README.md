# ModDown
A simple linux program that downloads mods in a curseforge modpack

This is a simple program that downloads all the mods in a modpack that you download from curseforge if you don't want to install the curseforge app.

This program currently only supports Minecraft ModPacks, but others may be added.

## Building
### Linux
___
#### Prerequisites
- [libzip](https://github.com/nih-at/libzip) (Avaliable in apt repositories or on Github.)
- [json](https://github.com/nlohmann/json) (Avaliable on Github)
- [indicators](https://github.com/p-ranav/indicators) (Avaliable on Github)
### Building & installing
```
mkdir build && cd build
cmake ..
make && make install
```
