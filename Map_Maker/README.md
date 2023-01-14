# DOOM
42 project Doom Nukem

### Table Of Contents
* [Objective](#objective)
* [Installation](#installation)
* [Usage](#usage)
* [Project Structure](#project-structure)
* [Dependencies](#dependencies)
* [Subject PDF](#subject-pdf)
* [Keep In Touch](#keep-in-touch)

#### About 42  
[42][42] is a free, non profit, project-based, peer-to-peer learning coding school. It originated in France and now has over 20 campuses all over the world. More information can be found [here][42].

<br>

---

<br>


## Objective  
Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.

---  


## Installation
Download and compile repo:  
``` 
git clone https://github.com/rpeepz/DOOM.git  
cd DOOM  
make  
```  
Executable `DOOM` will be created  
## Installation for Map Maker
**_Update the Makefile at lines 8 & 9 with the respective versions of glew and sdl2 before continuing_**
```
cd Map_Maker
make
```
Executable `Map_Maker` will be created
<br>


## Usage  
Standard Usage
```
./DOOM [file]  
```
Usage of Map Maper
```
./Map_Maker
```
Make sure you have the required [dependencies](#dependencies) before proceeding.  
For instructions and/or greater detail refer to the project's [pdfs](#subject-pdf)  

<br>

---  

<br>

## Project Structure

### Source files

```
src/
├── xxx.c
├── xxx.c
├── xxx.c
├── xxx.c
├── xxx.c
├── xxx.c
├── xxx.c
├── xxx.c
├── xxx.c
├── xxx.c
├── xxx.c
├── xxx.c
├── xxx.c
└── xxx.c

Map_Maker/
├── xxx.c
├── xxx.c
├── xxx.c
├── xxx.c
├── xxx.c
├── xxx.c
└── xxx.c

X directories, X files
```

<br>

--- 

<br>

## Dependencies  
* A C language compiler most common would be GCC or Clang.
* Cmake to run the custom Makefile for fast instalation.
* [Homebrew](https://brew.sh) for downloading the 2 following dependencies
* Glew @^2.2.0_1
* SDL2 @^2.0.20
* Code was written to run on Unix based systems, likely work on a windows machine too but is currently untested.

<br>

## Subject PDF
[Lem-in][pdf1]  

---  

## Keep in Touch  
Found a bug? Report it by contacting me on a social below  😃  
or if you feel like you can solve it, make a [pull request]!  
<br>  

You can find me on:
* [Github](https://github.com/rpeepz)  
* [Twitter](https://twitter.com/papagna94) 
* [LinkedIn](https://www.linkedin.com/in/rpapagna-510) 
<!-- * [Medium](https://medium.com/@themichaelbrave)  -->
<!-- * [Home] -->

<br>

---

_This project was attempted for learning purposes only. This repo may not be kept up to date and I cannot guarantee that it will work on your future machines. You are welcome to use and learn from this as you want, but do not implement code that you do not understand. These sources listed above have been invaluable and I advise you to do research on the subject before trying to copy the code you see here._

[42]: http://42.us.org "42 USA"
[pdf1]:  https://none.pdf "DOOMNUKEM"
[pull request]: https://github.com/rpeepz/DOOM/pulls "pull away!"