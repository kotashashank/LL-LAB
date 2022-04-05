import subprocess
import sys
from tkinter import *
import os

if os.environ.get('DISPLAY','') == '':
    print('no display found. Using :0.0')
    os.environ.__setitem__('DISPLAY', ':0.0')

def run():
    correct = subprocess.run(
        ["./runner"],
        universal_newlines=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )
    print(correct)
    # print('outputed')

def create_canvas():


    gui = Tk(className='Python Examples - Window Color')
    # set window size
    gui.geometry("1000x1000")


    #set window color
    gui.configure(background='white')

    canvas=Canvas(gui, width=1000, height=1000, background='white')
    canvas.pack()


    # sample input  0 0 0 0 0
    #               0 0 0 0 1
    #               0 0 0 0 1
    #               0 0 0 1 0

    canvas.create_line(100, 600, 100, 100, fill='black', width=2)
    canvas.create_line(100, 600, 700, 600, fill='black', width=2)
    gui.mainloop() 

run()
create_canvas()