# Terminal Game Engine
A Game Engine that is played in the terminal WIP

I started a  project to make a terminal 2D renderer, can be seen in my C-Projects repo, and with an abundance of time despite having exams soon, I decided to work on something I always wanted to make

ONLY ON LINUX RIGHT NOW

# Use
1. [Installation](#Installation)
2. [Setup](#Setup)
3. [Writing Code](#WritingCode)
4. [Compiling](#Compiling)
5. [Running](#Running)

[Controls](#Controls)<br name="Installation">
# Installation
```git clone https://github.com/DASPELLER4/Terminal-Game-Engine/```

cd into the folder and run make, the GameEngine binary should appear<br name="Setup">
# Setup
```./GameEngine```

type ```:np <project name>``` to create a project and ```:o <project name>```  to open it

to create your first obejct just type ```:no <object name> <object character> <inverse object size>``` and draw the polygons in the gui, and save and exit with s and q in the gui

to link a script to it type ```:ns <object name>``` IT IS REQUIRED TO USE AN EXISTING OBJECT FOR THIS <br name="WritingCode">
# Writing Code
Open the script file elsewhere, for example vim

In your script file, there are 2 functions, <scriptname>start and <scriptname>loop, in the first function, you can write any code you want. The self object has many attributes, such as relPosX and relPosY, so to change this, you can use this example!
  
```
int scriptstart(Object *self){
  self->relPosX = 10;
  self->relPosY = 20;
}
```
In the loop function, you can use the tsleep function so that you can wait time befor executing again (warning tsleep is bad)
  
```
int scriptloop(Object *self, char c){
  if(tsleep(30,5)) // here the 5 refers to approximate seconds, and the 30 is arbitrary but only can be used once
    self->relPosX+=1
}
```
You can also accept user input with the <code>c</code> variable containing the current key pressed

```
int scriptloop(Object *self, char c){
  if(c==' ')
    self->relPosY+=10;
  if(tsleep(12,0.2)){
    if(self->relPosY>0)
      self->relPosY-=10;
    if(self->relPosY<0)
      self->relPosY = 0;
  }
}
```

<br name="Compiling">
<h1>Compiling</h1>
just type <code>:c</code> <br name="Running">
<h1>Running</h1>
<b>cd into the project dir</b> and run ./bin <br name="Controls">
<h1>Controls</h1>
:q - quit<br>
:n +
  - p - new project
  - o - new object, takes three parameters, name, colour (a single ascii character) and size (inverse, 2 is the best option)
  - s - new script, needs to have the same name as an object

:p - preview an object<br>
:o - open project
