# DSA - Collage Assignment 1 – Search in dynamic sets

## About  

Essentially, this is a simple implementation of two data structures: a **binary search tree** and a **hash table**. Each with two different balancing/collision resolution algorithms.

### The hash table
The hash table is implemented as a linked list of buckets, each of which contains a linked list of key-value pairs. It is used to store the data.
The hash table has collision resolution using **chaining** and **open addressing(Double hashing)**.  

### The binary search tree
The binary search tree is implemented as a linked list of nodes, each of which contains a key and a value. It is used to store the keys in sorted order.
The binary tree is also self-balancing, using the **AVL** algorithm and **Red-Black trees**.


---

## Usage

First make sure you have glfw3 installed on your system. If you don't, then install it using your package manager. For example, on Ubuntu:

    sudo apt-get install libglfw3-dev
or on Arch Linux:

    sudo pacman -S glfw

If you're on Windows or MacOS... Well, good luck. Just kidding, you can download the precompiled binaries from [here](http://www.glfw.org/download.html).
But everything from there on is for you to figure out.

After you have `glfw3` installed, clone the repository and run the following commands in the root directory of the project:

    make
That's it! The program will be compiled and the executable will be placed in the `bin` directory.

    ./bin/searchInDSA

## Controls

`MouseWheel` - Zoom in/out  
`MouseDrag` - Pan around

You can also use the wigets to do stuff I guess.

## Libraries

This project uses the following libraries:

* [GLFW](http://www.glfw.org/) - For window creation and input handling

* [ImGui]((https://www.github.com/ocornut/imgui)) - For the GUI

## Acknowledgments

This project was made as a part of the course **Data Structures and Algorithms** at the [Faculty of Information Technology](http://fiit.sk) of the [Slovak University of Technology](http://stuba.sk) in Bratislava, Slovakia.


