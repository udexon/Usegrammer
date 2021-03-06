## Usegrammer = (Super) User + Programmer &mdash; Driving Free Software Revolution 2.0

- [Part I](https://github.com/udexon/Usegrammer/blob/master/Phos_BTOSG_I.md)
- [Part II](https://github.com/udexon/Usegrammer/blob/master/Phos_BTOSG_II.md)
- [Part III](https://github.com/udexon/Usegrammer/blob/master/Phos_BTOSG_III.md)

Usegrammer, is the combination of "user" and "programmer". The "superuser" is implied as all Linux users are "superuser" of their own system, and it conveys the special status as in the Tron universe.

It is made possible by Phoscript, a lightweight Forth derivative that can be implemented as a shell script within any known programming language, thus making it easier for users to make modifications to an existing program, to enhance its functionalities, as well as help project teams solve one of its most pressing problems, both in the free software and corporate world &mdash; finding manpower to sustain or expand development activities.

Underlying this is the "programming language fragmentation" problem &mdash; the emergence of mutually incompatible programming languages over the past few decades results in the fragmentation of programmer communities, reducing the number of programmers able to work on some older projects, written in older programming languages.

This seemingly intractable problem actually has a very simple solution &mdash; so simple that it usually causes disbelief in less experienced programmers, as well as dismay amongst more experienced programmers, who for better or worse, naturally invoke some kind of defense mechanisms such as, "this is new to me, so I had better attack it so as not to embarrass myself when questioned about it."

We illustrate the significance of "Usegrammers" with 2 relatively little known C++ projects:

- Open Dynamics Engine (ODE)
- Bullet Open Scene Graph (BTOSG) https://github.com/miguelleitao/btosg

ODE and BTOSG are low level physics simulation libraries that can be used in games and simulation applications.

[Phoscript Tutorials](https://github.com/udexon/Multiweb/blob/master/Phoscript_Tutorials.md) gives a list of tutorials with sample code of Phoscript implemented on various platforms and programming languages. 

[SymForth](https://github.com/udexon/SymForth) is SymEngine with Forth (Phoscript) wrapper,   &mdahs; a fast symbolic manipulation library, written in C++, now with Reverse Polish Notation support. It consists of samples of fundamental data structure and execution framework that will be deployed in the current project.

Readers are encouraged to refer to the articles mentioned above as fundamental principles of Phoscript code in this project.

In the original [`car.cpp`](https://github.com/udexon/btosg/blob/master/examples/car.cpp) in `miguelleitao/btosg`, lines 197 to 203 create a 4 wheeled car, as shown in figure 1:

- Figure 1
<img src="https://github.com/udexon/Usegrammer/blob/master/Usegrammer/Car1.png" width=600>

We have duplicated lines 197 to 203 with [modifications](https://github.com/udexon/btosg/blob/master/examples/car.cpp) , in order to create 4 additional cars as shown in figure 2:

- Figure 2
<img src="https://github.com/udexon/Usegrammer/blob/master/Usegrammer/Car5.png" width=600>

Our primary goal is to create a text input window in the application, where user (programmer) (hence "Usegrammer") can enter commands in Phoscript (Forth like Reverse Polish Notation), to ___FULLY CONTROL and MANIPULATE___ all objects and elements of the application (program), much like how a Unix shell is able to control and manipulate all elements in the operating system.

The modifications to `car.cpp` so far aim to verify if BTOSG code structure is flexible and robust enough to create a new object easily. Subsequemtly we will introduce Phoscript constructs like those available in ODE example and other articles in Phoscript Tutorials, to achieve a fully functional programmable shell within the program itself.

A Forth like Reverse Polish Notation expression (RPNX) is typically a space delimited list of words, e.g.

```
... a b c d ...
```

Compared to a LISP statement which includes an opening and closing bracket, RPNX is slightly more economical, but we shall explore the more fundamental significance of their differences later.

As such, "simplifying C++ code using RPNX" may convey two categories of features, according to the levels of expertise of the user or programmer.

For novice programmers, simplifying one C++ statement or a block of C++ statements to an RPNX _word_ (a special term referring to a token in Forth terminology) or a list of RPNX words, is the most obvious, trivial and yet practical benefit.

For expert programmers, representing C++ statements using RPNX has many metaprogramming benefits, which are of great significance.

From lines 387 to 410 in the modified `car.cpp`, we demonstrated 4 different ways of writing the code to create a new car, as shown in figure 3:

- Figure 3
<img src="https://github.com/udexon/Usegrammer/blob/master/Usegrammer/car_create_4.png" width=600>

The original code to create a new car is shown below:
```
    // Car
    myVehicle = new btosgVehicle(&myWorld);
    myVehicle->setPosition(btosgVec3(up*3.));
    myVehicle->setName("Vehicle");
    myVehicle->setMass(800.);
    myWorld.addObject( myVehicle );
    myVehicle->printInfo();
```

Lines 391 to 396 simply use a slightly different variable name to create a new car, just to make sure BTOSG is robust enough to create a new car without problems.

Lines 399 to 405 use variables in Phoscript convention.

Line 407 calls `sm_newcar()` which packagesl the code above in a separate function. The `sm_` prefix is an abbreviation for "stack machine".

Line 409 `sm_proc()` maps the input string `newcar:` to the function `sm_newcar()`.

`sm_proc( "newcar:" )`illustrates a trivial case of Phoscript RPNX where a host (C++) function is called WITHOUT any parameter. The mapping is accomplished in line 234 in function `map_func()` as shown in figure 4:

- Figure 4
<img src="https://github.com/udexon/Usegrammer/blob/master/Usegrammer/map_func_car.png" width=600>
