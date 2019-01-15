# Systems Final Project: *forest fire simulator*

## Team fire_fighters: Xiaojie(Aaron) Li, Jason Lin
## Project Description
Our project is a forest fire simulator that will utilize sockets to network and distribute the workload of doing such an intensive task. The user will be able to set parameters such as density of trees for an *n* by *n* grid. The fires will be placed in the center of each map, and there will be many trial runs for every single forest density to ensure that outliers are accounted for. Users will be able to **1)** calculate the individual burn time of density *x* and dimension *y*(it's a square board), **2)** use forking on the same computer to utilize multi-core processing and return an answer faster. **3)** utilize networking to connect the server computer to multiple clients which the user can then use to calculate all densities from 0 to 100 of a forest with dimensions *x* specified by the user. This will return the longest burn time along with the density, the shortest burn time, and the average.

## Instructions to run
First, clone the repository into your directory by doing
```
git clone https://github.com/aaronli39/MKS65C-FinalProject.git
```

Change your directory so that you are inside of the MKS65C-FinalProject folder
```
cd MKS65C-FinalProject
```

Compile everything with make
```
make
```

Now, depending on what you want to do with the program, you can select between
calculating one simulation or many simulations. If you decide to pick
number 3, then you should do the following:

1. Open up many terminals and on one of them, do `./server` and enter the necessary parameters.
2. Go into the directory with the other terminals and do `./client`. It is important that
the number of client terminals that you have is equal to the number that you entered in the server.
3. After connecting all of the clients to the server, it will automatically start calculating simulations.
4. If you want to use the networking feature, you can ssh into any school computer, for example, bart.stuy.edu
and run the `./server`
5. However, this time, when you run the clients, you do `./client bart.stuy.edu` or whatever the IP address of
the host server is.
6. Enjoy!

## DEVLOG
* **01/02/19**:
Worked on the design document and planned out how we were going to tackle this project. Created the repository.

* **01/04/19**:
Built the map that would be needed to help the tree burning algorithm function.
Made the trees randomly generate.

* **01/06/19**:
First attempt at making the algorithm for spreading fires using the frontier based method.

* **01/07/19**:
Fixed some bugs with the frontier based method and changed the formatting of test
printing the map.

* **01/08/19**:
Repaired essential bugs that were not noticed before. Made the algorithm work correctly.
Added the system of selecting which methods of calculating the user wants to use.

* **01/09/19**:
Added user input on the density and dimensions of the forest when making simulations.

* **01/10/19**:
Created non-frontier method of burning forests. Fixed the simulation algorithms again
so that they match in results.

* **01/11/19**:
Added forking to the simulations so that multiple cores or threads can be used at once.

* **01/12/19**:
Added error checking with the input as well as beautifying a lot of code and made
text-interaction more intuitive and simple. Did a lot of minor fixes in code/beautifying.

* **01/14/19**:
Made the breakthrough between implementing server input and making clients do work. This was
done through switching read/write loops in the forking server and the client. We were stuck
before this breakthrough on how to actually implement servers. I worked on client.c while Jason
worked on forking server. We voice chatted and discussed the whole process, from comma separated
inputs to parsers and then feeding it in/out of the buffer in order to feed it into our calculate methods.
We worked very well.

* **01/15/19**:
Added graphs to display data and did minor bug fixes to ensure that user has best experience and that
demo doesn't mess up. Added timer for functions and prettified experience.
