# laa

tiny C tool to patch the large address aware flag on windows executables.

### huh?

lets 32-bit applications (`.exe` or `.dll`) use more than 2GB of RAM when running in a 64-bit envrionment (e.g. old games like final fantasy xi),

pure C, lightweight, no dependencies.

### how

pre-compiled binaries for linux and windows are here, with instructions in the zip:

[**releases page**](https://github.com/kkslidermight/laa/releases)

### build from source

#### linux:

install `gcc` and `make`

clone the repo and run `make linux`

```bash
git clone https://github.com/kkslidermight/laa.git
cd laa
make linux
```

the binary will be at `build/linux/laa`. copy to the path-accessible directory of your choice or just run it from there.

#### windows:

install `gcc` from [MinGW-w64](https://www.mingw-w64.org/) or have `cl.exe` from visual studio build tools in your PATH.

clone the repo and run `make windows`

```bash
git clone https://github.com/kkslidermight/laa.git
cd laa
make windows
```

the binary will be at `build/windows/laa.exe`.

### usage

run `./laa` (linux) or `.\laa.exe` (windows) from the terminal and point it to your desire executable.  
be sure to back up the original exe first.

example:
```bash
#### linux
./laa mygame.exe

#### windows (powershell or cmd)
.\laa.exe mygame.exe

### License

BSD 3-Clause "New" or "Revised" License
