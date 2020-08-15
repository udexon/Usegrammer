
- [Part I](https://github.com/udexon/Usegrammer/blob/master/Phos_BTOSG_I.md)
- [Part II](https://github.com/udexon/Usegrammer/blob/master/Phos_BTOSG_II.md)
- [Part III](https://github.com/udexon/Usegrammer/blob/master/Phos_BTOSG_III.md)


Let us illustrate how Phoscript RPNX handles C++ functions with parameters using `dsSetViewpoint(xyz,hpr)` in line 222 of `demo_buggy.cpp` from Open Dynamics Engine (0.16), as shown in figure 5.:

- https://bitbucket.org/odedevs/ode/src/master/ode/demo/demo_buggy.cpp

- Figure 5
<img src="https://github.com/udexon/Usegrammer/blob/master/Usegrammer/demo_buggy.png" width=600>


Line 222 has been moved to line 378 in our version of `demo_buggy.cpp` as we have inserted quite a number of lines to implement Phoscript.

The Phoscript equivalent of line 378 `dsSetViewpoint(xyz,hpr)` is `sm_proc( hpr, xyz, "svp:" )` in line 374, as shown in figure 6.

- Figure 6
<img src="https://github.com/udexon/Usegrammer/blob/master/Usegrammer/ODE_buggy_svp.png" width=600>


`sm_proc()` is implemented as a recursive variadic function, so as to take a variable number of parameters as shown in figures 7 and 8. The prefix `sm_` stands for "stack machine".


- Figure 7
<img src="https://github.com/udexon/Usegrammer/blob/master/Usegrammer/sm_proc_1.png" width=600>

- Figure 8
<img src="https://github.com/udexon/Usegrammer/blob/master/Usegrammer/sm_proc_2.png" width=600>

Firstly, the order of parameters in `dsSetViewpoint(xyz,hpr)` has been reversed:

```C++
sm_proc( hpr, xyz, "svp:" );
```

This is in accordance to the convention of stack machine, i.e. "last in first out". Later we will demonstrate that the LIFO order can be changed to FIFO using some metaprogramming tricks, i.e. We wish to rewrite the parameters of `sm_proc()` to follow C++ conventions, for reasons also to be disclosed later:

```C++
  sm_proc( "dsSetViewpoint(", xyz, hpr, ")" );
```

Figures 9 to 11 show functions `sm_lastchar()` and `map_func()` required to implement `sm_proc()`. They are located at the lines shown below:

- Lines 113 to 326 https://github.com/udexon/Usegrammer/blob/master/Usegrammer/demo_buggy_4.cpp
- Lines 189 to 367 https://github.com/udexon/btosg/blob/master/examples/car.cpp

- Figure 9
<img src="https://github.com/udexon/Usegrammer/blob/master/Usegrammer/sm_lastchar_1.png" width=600>

- Figure 10
<img src="https://github.com/udexon/Usegrammer/blob/master/Usegrammer/sm_lastchar_2.png" width=600>

- Figure 11
<img src="https://github.com/udexon/Usegrammer/blob/master/Usegrammer/map_func.png" width=600>


The parameters of sm_proc( ... ) are pushed on to a global stack sm_S. A string parameter is broken into a list of tokens (Phoscript tokens or Forth "words") with space as the delimiter. Non-function tokens are again pushed onto the stack. Function tokens are suffixed with a colon (":") and are mapped to host programming language functions and executed. Special tokens include colon definition words as well as flow control words. 

A word in Forth programming language is literally token in the terminology of modern programming languages.

Colon definition words (CDW) is the Forth way of defining new functions. It begins with a colon, a single word function name, a list of words describing the contents of the function, and terminates with a semi colon (";").

```
: new_function_name ... list of words ... ;
```

CDW is what makes Phoscript (to be abbreviated as "Phos") special compared to mainstream programming language. The absence of brackets and the eases of renaming a function with an alias makes it much easier for novice as well as experts programmers to remember an alias or function name. 

It also provides a facility to create standard aliases across different programming languages.
For example, `fi:` was initially created in PHP to map to `file()`. It can eventually be ported to other programming languages to map to functions with the same purpose by different names. 

Further, Phos colon definition words or aliases can be used with Unicode, providing an easy solution to map non Latin words to existing programming languages. This is a great boost to the education of non-Latin speaking population, which actually make up the majority of the world population. 

In the ODE example, `dsSetViewpoint(xyz,hpr)` has been transformed into 
```
sm_proc( hpr, xyz, "svp:" )
```

We believe that `svp:` is easier to remember than `dsSetViewpoint` .

More importantly, Phos provides a facility to alias function names in C++ or any programming language. It is up to the programmer to decide a convenient alias, which can be changed or added at any time. 

Yet another way to rewriting `dsSetViewpoint` in Phos is: 
```
sm_proc( "dsSetViewpoint(", xyz, hpr, ")" );
```

In this way, we minimize the changes made to the original C++ code by preserving the order of function name, parameters and even brackets, where we simply tokenize them.

By passing the parameter `"dsSetViewpoint("` to `sm_proc()`, it can also look up the function prototype for `dsSetViewpoint()`, and thus determine the number and types of its parameters, further automating parameter handling in `sm_proc()`. Currently, the parameters of `sm_svp()` are coded manually, which is still relatively straightforward, as shown below:

```C++
void sm_svp() // set view point
{ 
  float *fa=(float*) sm_pop(); 
  printf("\n\nIn sm_svp\nsm_S: %f\n", fa[0]); 
  float *fb=(float*) sm_pop(); 
  printf("sm_S: %f\n", fb[0]); 
  dsSetViewpoint (fa,fb); 
}
```

