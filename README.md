[![Build Status](https://travis-ci.org/Manwe56/competitive-programming.svg?branch=master)](https://travis-ci.org/Manwe56/competitive-programming)

# Why this project

It's been a while (2010) I race in various online programming competitions (started with google ai challenge, then mostly hackerrank and codingame) under the nickname Manwe. I am quite proud of the results: once winner, often in the top 1%, always in the top 5%. 
It is a real pleasure each time and I am thrilled when a new contest is announced! 

Time after time, challenges are different for sure, but I accumulated a set of tools, algorithms, physics that might be helpful from a challenge to another. Of course it comes with a set of best practices I discovered over time :)
This project aims at sharing with the community those code I appreciate in the challenges and are helping you gain time, allowing you to develop in several classes with a one click build, and sharing a common set of codes between challenges. Don't reinvent the wheel, focus on the subject!

Do not hesitate to contribute, to fork and issue pull requests!

# constraints

Those challenges are usually online and includes a code editor. If you are free to use another IDE, your code is usually in a single file that make you intensely use inner classes. To ease your work, there is a builder that allows you to build a single file class from a main class and will grab all its dependencies. You can now split your code across several packages and directly build your file in a single click. It is certainly a prerequisite to share easily some code between your different challenges

Another constraint is that you usually don't have access to any third party software, and you must use the JDK built-in libraries. That's why this project is not taking advantage of must have third party software and is really focusing on this kind of challenges instead of providing state of the art artificial intelligence library. Note also that you might have a file size limit, and in this case you should not have too much dependencies

# mounting the project in eclipse

I use eclipse as IDE, and the project includes directly an eclipse project.
You can also use maven to build project files for various IDE

# how to use the builder

Builder is that class in charge of building a single file from you different classes. It is located in the builder package. Have a look at the javadoc for more info


# hints

You will often find "Hint" sections in the javadoc. They are here to help you use the tools, or give you some advice you might find useful when programming for a contest. They are not documentation as is but more ideas that can make the difference.

# code layout

The source code is in the src folder, and the tests are in the test folder.
You will find a builder package that contains only the utilities allowing you to build your "single file" program.
The rest of the code is in the competitive.programming package and then by theme.
If you want to contribute, feel free to add contest independent code in the competitive.programming package.

I would advice you create your own package by contest when using this project.