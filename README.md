# Walled-Off
A Rogue-lite game made with the Ncurses library in the C programing language;

## Code Guidelines
This guidelines should be followed to obtain uniform code across all contributers:

### General
+ If a expression is compared to a constant, the constant should come second;
```
if (int_max > 2);
```
+ The use of breaks and returns in the middle of a block/function are **prohibited**;
+ Returns should only come ate the end of the function;

### Lines
+ Tabs should equal 2 whitspaces;
+ Lines should not be longer than 100 chars;
+ Consecutive empty lines should not be used;
+ Variable declarations, lines or blocks of code should be followed by an empty line;
+ In a sequence of lines of code, every component should be aligned in agreement with each component of the different lines;
  ```
  int main (){
  int number_of, for_demonstration;
 
  number_of         = 12;
  for_demonstration = 0;
 
  for (; i > 0; i--){
    printf ("HI!!");
    for_demonstration += 1;
  }
  ```
 
 ### Variables
 + Variables and function names should be named with underscores between words;
 + The letters to descrive each variable/function should be lowercase;
  ```
  int time_zone;
    
  very_cool_secondary_function (time_zone);
  ```
 
### Commentary
+ Every commentary should use multiline comment marks, starting and ending with a white space;
  ```
  /* Very cool and informative comment */
  ```
+ A comment placed right after a line of code should be separeted by a tab;
  ```
  very_cool_function (time_zone); /* That does this */
  ```
+ When doing multiline comments, write each one as if they where a single comment;
  ```
  /* Very interesting comment followed by */
  /* an even more interesting comment */
  ```
 
  





 
 

 
