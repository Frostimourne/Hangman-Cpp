# Overview

This program demonstrates getting "back into the saddle" with programming for me.  As mentioned in my short Hello World ReadMe, I originally started programming in C++ around 10 years ago, and so felt it was fitting to do an actual project, albeit a small one, in C++ to freshen up on some simple behaviors of the language.

With the goal of creating a simple, classic hangman game, I wanted to demonstrate the usage of simple variable types, arrays, vectors, class structures, variable casting, and file manipulation for both reading and writing.  Words are read in from the provided WordList.txt into a vector of strings, where a word is then randomly selected from to be the secrete word the user must guess.

From there, play loops with the user being prompted for a guess, that guess being either correct or incorrect, until either the entire hangman is drawn or the secret word is guessed.  The user can then play again as many times as they would like using the current word list, and at the end of their play session, the program will output their stats to a text file for them to view. 

You can find a very brief code walkthrough and demonstration at the following link:

[Software Demo Video](https://youtu.be/vxtVKF5-7_E)

# Development Environment

All coding was done inside of Visual Studio code, as I just can't seem to get away from Windows yet.  VS Code conveniently contains my terminal, CMake, and Intellisense for quickly being able to test and debug C++ quickly.

While newer languages are easier, I haven't found too much trouble dipping back into a more "mature" language like C++.  Modern IDE's are very good at catching missing semicolons, curly braces, 

# Useful Websites

As always, Google is a wonderful assistant when programming to be able to check syntax and learn about libraries and their functions.  The most common sites I used for troubleshooting and learning are:

- [Stack Overflow](http://www.stackoverflow.com/)
- [CPlusPlus.com](http://cplusplus.com/)

# Future Work

While I'm not planning on releasing this incredible program in any commercial capacity to actual users, there are a number of improvements and additions that could be made:

- Optimizations to handling ASCII image generation/manipulation.  Currently very hard-coded and non-scalable
- More error handling: While this program does check for valid guesses, it does not parse and validate the input .txt file, and so could be fed impossible "words" that contain invalid characters
- Tracking stats over multiple sessions, or tracking stats for multiple users over multiple sessions, more and better stats
- Main Menu: Options to select word list file to load into program, difficulty options, user selection, a nice ASCII logo, etc.