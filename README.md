# CHIP-8 emulator

## Requirements

### Build `raylib`

```bash
sudo apt install build-essential cmake

sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev \
                 libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev \
                 libxinerama-dev libwayland-dev libxkbcommon-dev

git clone https://github.com/raysan5/raylib.git raylib
cd raylib
mkdir build && cd build
cmake ..
make
sudo make install
sudo ldconfig
```

## Build

```bash
cmake -B build && cmake --build build
```

## Run

```bash
./build/chip8
```

## Usage

### Program execution

Press `SPACE` to step forward.
Press `C` to continue;
Press `ESC` to close the window and exit the program.

### Debugging

Press `M` to print memory around I register.
Press `R` to print registers.
