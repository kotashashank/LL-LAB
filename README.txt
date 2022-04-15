You must install Tnkter for Python 3 to run the frontend.
You will also need graphviz so that you can draw the circuit.

To run the frontend, type python3 driver.py.

driver.py has a makefile variable toward the top that you change depending on the file:
    - "runner" for fulladder.c
    - "runner2" for fulladder2.c
    - "clock" for testclock.c

To run the regular output, just run the makefile. 
For example, input "make clean runner" then "runner" or "./runner" in the terminal to run fulladder's main method.
