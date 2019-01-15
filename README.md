# Systems Final Project: *forest fire simulator*

## Team Members: Xiaojie(Aaron) Li, Jason Lin
## Project Description
Our project is a forest fire simulator that will utilize sockets to network and distribute the workload of doing such an intensive task. The user will be able to set parameters such as density of trees for an *n* by *n* grid. The fires will be placed in the center of each map, and there will be many trial runs for every single forest density to ensure that outliers are accounted for. Users will be able to **1)** calculate the individual burn time of density *x* and dimension *y*(it's a square board), **2)** use forking on the same computer to utilize multicore processing and return an answer faster. **3)** utilize networking to connect the server computer to multiple clients which the user can then use to calculate all densities from 0 to 100 of a forest with dimensions *x* specified by the user. This will return the longest burn time along with the density, the shortest burn time, and the average.

## Instructions to run
Our project will guide the user through the product by using text based interactions with the user in the terminal. 
