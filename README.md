<!-- @format -->

# DSA - Collage Assignment 1 – Search in dynamic sets

## About

Essentially, this is a simple implementation of two data structures: a **binary search tree** and a **hash table**. Each with two different balancing/collision resolution algorithms.

### The hash table

The hash table is implemented as a linked list of buckets, each of which contains a linked list of key-value pairs. It is used to store the data. The hash table has collision resolution using **chaining** and **open addressing(Double hashing)**.

### The binary search tree

The binary search tree is implemented as a linked list of nodes, each of which contains a key and a value. It is used to store the keys in sorted order. The binary tree is also self-balancing, using the **AVL** algorithm and **Red-Black trees**.

---

## Compiling

### Compiling With GUI

#### Linux

First make sure you have `libglfw3` and `pkg-config` installed on your system. If you don't, then install it using your package manager. For example, on Ubuntu:

    sudo apt-get install libglfw3-dev pkg-config

or on Arch Linux:

    sudo pacman -S glfw pkg-config

After you have `glfw3` installed, you can compile the program with `make gui=1` or `make` if you don't want to use the GUI.

#### Windows

If you're on Windows the necessary libraries are already included in the repository. You can just compile the program with `.\build.bat gui [32|64]` or `.\build.bat` if you don't want to use the GUI.

Make sure you have MingGW(9.2.0) installed and added to your PATH. You can download at graphical MinGW installer [here](https://osdn.net/projects/mingw/downloads/68260/mingw-get-setup.exe/). Once you have it installed, just select the base and g++ packages and install them (Installation > Apply Changes).

Once you installed it or already have MingGW installed. Run `.\build.bat gui [32|64]`. If you run the build gui without specifying the architecture, It will not build, but it will tell you what architecture you need to specify. (quirk of the build script)

#### MacOS

MacOS... Well, good luck. Just kidding, you can either install glfw3 with `brew install glfw pkg-config` or download the precompiled binaries from [here](http://www.glfw.org/download.html). If you install it with `brew`, you could probably just run `make gui=1` and it will work.

### Compiling Without GUI

If you don't want to bother with the GUI, you can compile the program without it. Just run `make gui=0` or `make`.

Whenever changing between no gui and gui, you need to run `make clean` first so that the old binaries are deleted.

```bash
make clean
make gui=0 # or make gui=1
```

This is also true for compiling with the GUI.

### Running

After you have compiled the program, you can run it by executing the following command in the root directory of the project:

```bash
./bin/searchInDSA # or ./bin/searchInDSA.exe on Windows
```

## Usage

You can see the usage of the program by running it with the `--help` flag:

```bash
./bin/searchInDSA --help
```

To run the GUI, just run the program without any arguments:

```bash
./bin/searchInDSA
```

or with the `run` flag:

```bash
./bin/searchInDSA run [a|r|c|o]
```

### GUI Controls

`MouseWheel` - Zoom in/out  
`MouseDrag` - Pan around

You can also use the wigets to do stuff I guess.

### Generating test data

It's a good idead to first generated fixed test data, so that the benchmarks are consistent. To do that, run the program with the `test gen` flag:

```bash
./bin/searchInDSA test gen {testFileName} 1000,10000,100000,1000000 unique #example
# More info about the test command can be found by running ./searchInDSA --help
```

### Running benchmarks

```bash
./bin/searchInDSA test bench fixed {testFileName} a #example
```

For more commands and command info, run `./bin/searchInDSA --help`.

## Libraries

This project uses the following libraries:

- [GLFW](http://www.glfw.org/) - For window creation and input handling

- [Dear ImGui](<(https://www.github.com/ocornut/imgui)>) - For the GUI

## Acknowledgments

This project was made as a part of the course **Data Structures and Algorithms** at the [Faculty of Information Technology](http://fiit.sk) of the [Slovak University of Technology](http://stuba.sk) in Bratislava, Slovakia.
