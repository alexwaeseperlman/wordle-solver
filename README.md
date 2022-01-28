# Wordle solver
This program solves wordle in ~3.6 moves on average. 
It works by keeping track of the set of words that are potential solutions (words that could have led to the same sequence of colorings), and guessing the word that maximizes expected information gain, or entropy, for each round. I extracted the word list from the wordle website.

## Example usage:
`>>` refers to output from the program, and `<<` means input from the user.
After each guess the user should input the resulting colors with 1 corresponding to yellow and 2 to green.
```
>> Guess: soare
<< 0 0 0 1 1
>> Guess: tined
<< 0 0 0 1 0
>> Guess: plack
<< 2 0 0 0 1
>> Answer: perky
```
## Compilation
This program uses `std::execution` to run in parallel, which is a c++17 feature. I compile it with the following command:
```
g++ solver.cpp -O3 -ltbb
```
