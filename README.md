# GD Chess C++

Single threaded chess engine written as a Godot addon

## Poject Setup

### Inside directory: `gd-chess-cpp-ext`

Install requried pkgs for c++ development

```shell
sudo apt-get install build-essential manpages-dev
```

Install scons for building

```shell
sudo apt-get install scons
```

### Windows builds

Install MinGW for windows platforms

```shell
sudo apt install mingw-w64
```

### Web assembly builds

Install Emscripten and add to your path

```shell
sudo apt install cmake python3 python3-pip
```

setup Emscripten (you might want to clone this outside your project)

```shell
git clone https://github.com/emscripten-core/emsdk.git
```

```shell
cd emsdk
./emsdk install latest
./emsdk activate latest
```

Activate PATH for emsdk

```shell
source ./emsdk_env.sh
```

### Andoid builds

Instructions for android dependencies here

## Building the project

### Linux

```shell
scons platform=linux target=template_debug arch=x86_64
scons platform=linux target=template_release arch=x86_64
```

### Windows

```shell
scons platform=windows target=template_debug arch=x86_64
scons platform=windows target=template_release arch=x86_64
```

### Web

```shell
scons platform=web target=template_debug threads=yes
scons platform=web target=template_release threads=yes
scons platform=web target=template_debug threads=no
scons platform=web target=template_release threads=no
```

### Android

Too lazy to implement

### MacOS

Too lazy to implement

### IOS

Too lazy to implement
