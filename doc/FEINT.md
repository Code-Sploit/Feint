# Feint

### A compiler language written in C and Assembly for x86 systems

**Written by @Code-Sploit**

## Documentation info
*Last edited on: **25-11-2022***<br>
*Current version: **Beta-V0.1***


## Building
**Building Feint for x86 UNIX platforms:**<br>
#### *Installing dependencies*<br>

*Ubuntu/Debian*<br>
`sudo apt install make nasm gcc`<br>

*Arch/Manjaro*<br>
`sudo pacman -S base-devel nasm`<br>

#### *Clone the github repository*<br>
`git clone https://github.com/Code-Sploit/Feint && cd Feint`<br>

#### *Compile Feint*<br>

To compile Feint itself run:<br>
`make compile`<br>

To compile the test tools run:<br>
`make tools`<br>

Warning: Feint has to be built before being able to use tools!

## Features

**Current features:**<br>
1. Defining variables either integer or string.
2. Defining functions
3. Calling functions
4. Basic math operations: add(), sub(), mul(), div()
5. Printing of variables
6. Setting the return value for the main function

## Syntax

**Defining the main function:**<br>
*Important: The definition of the function main is required because the compiler will only run the main function*

```
construct main {
  your code
  ...
  
  return yourreturnvalue;
}
```

**Defining a variable:**<br>
*Integers:*
```
int yourvariablename = yourvalue;
```

*Strings:*
```
str yourvariablename = "yourvalue";
```


**Defining a function:**<br>
*Important: If you define a function make sure to put the keyword 'back' when you want to jump back to the point in the code where the function was called!*

```
construct yourfunctionname {
  your code
  ...
  
  back;
}
```

**Printing a variable:**<br>
```
printvar(yourvariablename);
```

**Calling a function:**<br>
```
call yourfunction();
```

**Math operations:**<br>
*Important: All variables mentioned here need to be initialized*<br>

```
add(a,b,r); ----> Add the value of 'b' to the value of 'a' and store the result in the variable r
sub(a,b,r); ----> Subtract the value of 'b' to from value of 'a' and store the result in the variable r
mul(a,b,r); ----> Multiply the value of 'b' with the value of 'a' and store the result in the variable r
div(a,b,r); ----> Divide the value of 'a' with the value of 'b' and store the result in the variable r
```

## Using tools
1. *Testing the compiler*

```
make tools

./runtools.sh tests
```
