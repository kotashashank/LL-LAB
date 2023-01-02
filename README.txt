Logic Lab

In this lab, we create a C backend to greedily analyze logical circuits as defined by users in code. The circuits start in an unknown state and then all ports are updated to reflect the signal propogating through the user's circuit. To aid in user debugging of their own code as well as understanding of the circuit, we employ graphviz to draw out the circuit the user defines; additionally, using tinkter we are able to create a graph visualization of ports users are interested in, allowing them to both visualize the circuit and the values of ports of interest through time.

You must install Tnkter for Python 3 to run the frontend.
You will also need graphviz so that you can draw the circuit.

To run the frontend, type python3 driver.py.

driver.py has a makefile variable toward the top that you change depending on the file:
    - "runner" for fulladder.c
    - "runner2" for fulladder2.c
    - "clock" for testclock.c

To run the regular output, just run the makefile. 
For example, input "make clean runner" then "runner" or "./runner" in the terminal to run fulladder's main method.
