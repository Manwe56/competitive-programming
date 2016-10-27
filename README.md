[![Build Status](https://travis-ci.org/Manwe56/competitive-programming.svg?branch=master)](https://travis-ci.org/Manwe56/competitive-programming)

# Why this project

It's been a while (2010) I race in various online programming competitions (started with google ai challenge, then mostly hackerrank and codingame) under the nickname Manwe. I am quite proud of the results: once winner, often in the top 1%, always in the top 5%. 
It is a real pleasure each time and I am thrilled when a new contest is announced! 

Time after time, challenges are different for sure, but I accumulated a set of tools classes and algorithms that might be helpful from a challenge to another. Of course it comes with a set of best practices I discovered over time :)
This project aims at sharing with the community those Java and C++ code I appreciate in the challenges and are helping you gain time, allowing you to develop in several files with a one click build, and sharing a common set of codes between challenges. Don't reinvent the wheel, focus on the subject!

In the docs folder you will find a pdf with advices if you launch yourself in competitive programming

Do not hesitate to contribute, fork and make pull requests!

# constraints

Those challenges are usually online and includes a code editor. If you are free to use another IDE, your code is usually in a single file that make you intensely use inner classes. To ease your work, there is a builder that allows you to build a single file class from a main class and will grab all its dependencies. You can now split your code across several files and directly build your file in a single operation. It is certainly a prerequisite to share easily some code between your different challenges

Another constraint is that you usually don't have access to any third party software, and you must use the native libraries. That's why this project is not taking advantage of third party software. The objective is really focusing on this kind of challenges instead of providing state of the art artificial intelligence library. Note also that you might have a file size limit, and in this case you should not have too much dependencies

# how to use the builder

Builder is that class in charge of building a single file from you different classes. It is writen in Java and located in the builder package. Have a look at the javadoc for more info.
THe builder supports both Java and C++ files so you can use it for both.

# hints

You will often find "Hint" sections in the comments. They are here to help you use the tools, or give you some advice you might find useful when programming for a contest. They are not documentation as is but more ideas that can make the difference.

# build and test

This project is built using gradle 3.1. This choice has been made because it is a multiple language codeline and it will build and test in a single operation both C++11 and Java 8 codes.

So to launch the compilation and then run all the unit tests in both C++ and Java, simply launch the command:

gradle check

Note: On windows, several versions of the google test lib are available and depending on which version and compiler you are using, you might want to uncomment the relevant line in the method findGoogleTestCoreLibForPlatform

# for Java developers:

Code uses the Java version 8.

## code layout

The source code is in the src/main/java folder, and the tests are in the src/test/java folder.
You will find a builder package that contains only the utilities allowing you to build your "single file" program.
The rest of the code is in the competitive.programming package and then by theme.
If you want to contribute, feel free to add contest independent code in the competitive.programming package.

I would advice you create your own package by contest when using this project so that you are not packaging several files with a main :)

## mounting the project in eclipse

I use eclipse as IDE, and the project includes directly an eclipse project.
You can also use maven to build project files for various IDE

# for C++ developers:

Code uses C++ 11.

## code layout

I wanted to have the code in the src/main/cpp folder, but encountered issues with Travis CI to build and test the codeline. So I ended up with a code layout that is completely different.
So header files could be found in the src/competitiveProgramming/headers folder and the tests are located in the src/competitiveProgrammingTest/cpp folder.

## coding conventions

Since I wanted the builder to be able to concatenate the different files in a single file without investing too much in its complexity, I took decisions that would not be valid in a real production project:
All the source code is in the header files. Include path are always from the "headers" folder. If you not respect this convention, the builder will fail to resolve your include file path and will ignore it.

