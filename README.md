# Why this project

It's been a while (2010) I race in various online programming competitions (started with google ai challenge, then mostly hackerrank and codingame) under the nickname Manwe. I am quite proud of the results: once winner, often in the top 1%, always in the top 10%. 
It is a real pleasure each time and I am thrilled when a new contest is announced! 

Time after time, challenges are different for sure, but I accumulated a set of tools, algorithms, physics modelisation that might be helpfull from a challenge to another. Of course it comes with a set of best practices I discovered over time :)
This project aims at sharing with the community those code I appreciate in the challenges and are helping you gain time, allowing you to develop in several classes with a one click build, and sharing a common set of codes between challenges. Don't reinvent the wheel, focus on the subject!

Do not hesitate to contribute, to fork and issue merge requests!

# status

[![Build Status](https://travis-ci.org/Manwe56/competitive-programming.png)](https://travis-ci.org/Manwe56/competitive-programming)

# constraints

Those challenges are usually online and includes a code editor. If you are free to use another IDE, your code is usually in a single file that make you intensely use inner classes. To ease your work, there is a builder that allows you to build a single file class from a main class and will grab all its dependancies. You can now split your code accross several packages and directly build your file in a single click. It is certainly a prerequisite to share easily some code between your different challenges

Another constraint is that you usually don't have access to any third party software, and you must use the JDK builtin libraries. That's why this project is not taking advantage of must have third party software and is really foccusing on this kind of challenges instead of providing state of the art artificial intelligence library. Note also that you might have a file size limit, and in this case you should not have too much dependancies

# mounting the project in eclipse

I use eclipse as IDE, and the project includes directly an eclipse project. It should not be difficult for you to use the code with any good editor.

# how to use the builder

Builder is that class in charge of building a single file from you different classes. It is located in the builder package. Have a look at the javadoc for more info

# conventions

//TODO

# hints

//TODO

# code layout

//TODO
