# EzWow


special thanks to [ownedcore](https://www.ownedcore.com) community!


this project uses
* shared object injection via LD_PRELOAD 
* opengls GlxSwapBuffer hook to draw imgui ingame menu [ref](https://www.ownedcore.com/forums/world-of-warcraft/world-of-warcraft-bots-programs/wow-memory-editing/330135-linux-ld_preload-injection-without-patching-wine.html)


[![unknown.png](https://i.postimg.cc/zvRW54qM/unknown.png)](https://postimg.cc/YLMjznGf)


## prepare

```sh
# install wine
sudo apt-get install wine
# stuff to build
sudo apt-get install g++ git cmake g++-multilib
sudo apt-get install build-essential libgl1-mesa-dev 

# window handling & input related stuff
sudo apt-get install libxmu-dev
sudo apt-get install libxtst-dev
```

## clone & build

```sh
# clone repo with dependencies and build
git clone --recursive git@github.com:al-eax/ezwow.git
cd ezwow
# make all sh files executable
find . "*.sh" -execdir chmod u+x {} +
# build libezwow.so
./build.sh
```

## configure

```sh
# replace the path to your Wow.exe in these files
editor inject_via_ldpreload.sh
editor inject_debug_via_ldpreload.sh
```

## build, run & inject

```sh
# build libezwow.so
./build.sh

# build libezwow.so, start Wow.exe via wine and inject libezwow.so
./inject_via_ldpreload.sh

# attach gdb to Wow
./gdb_wow.sh

# start Wow via winedbg and inject libezwow.so
./inject_debug_via_ldpreload
```

## third party stuff

* subhook
* imgui
