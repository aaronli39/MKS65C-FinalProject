# Systems Final Project: *forest fire simulator*

## Team Members: Xiaojie(Aaron) Li, Jason Lin
## Project Description
Our project is a forest fire simulator that will utilize sockets to network and distribute the workload of doing such an intensive task. The user will be able to set parameters such as density of trees for an *n* by *n* grid. The fires will be placed in the center of each map, and there will be many trial runs for every single forest density to ensure that outliers are accounted for. Users will be able to **1)** calculate the individual burn time of density *x* and dimension *y*(it's a square board), **2)** use forking on the same computer to utilize multicore processing and return an answer faster. **3)** utilize networking to connect the server computer to multiple clients which the user can then use to calculate all densities from 0 to 100 of a forest with dimensions *x* specified by the user. This will return the longest burn time along with the density, the shortest burn time, and the average.

## Instructions to run
First, clone the repository into your directory by doing
'''
git clone https://github.com/aaronli39/MKS65C-FinalProject.git
'''

Change your directory so that you are inside of the MKS65C-FinalProject folder
'''
cd MKS65C-FinalProject
'''

Compile everything with make
'''
make
'''

Now, depending on what you want to do with the program, you can select between
calculating one simulation or many simulations. If you decide to pick
number 3, then you should do the following:

1. Open up many terminals and on one of them, do './server' and enter the necessary parameters.
2. Go into the directory with the other terminals and do './client'. It is important that
the number of client terminals that you have is equal to the number that you entered in the server.
3. After connecting all of the clients to the server, it will automatically start calculating simulations.
4. If you want to use the networking feature, you can ssh into any school computer, for example, bart.stuy.edu
and run the './server'
5. However, this time, when you run the clients, you do './client bart.stuy.edu' or whatever the ip address of
the host server is.
6. Enjoy!
