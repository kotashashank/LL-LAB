import subprocess
import sys
from signal import signal
from tkinter import *

WIDTH = 1000
HEIGHT = 1000

def parse_data(output):
    

    output_array = output.split()

    input_amount = get_ports(output_array)

    output_amount = get_outputs(output_array, input_amount)


    

    

def get_ports(output_array):
    port_amount = 0
    for s in output_array:
        if s == "port":
            port_amount += 1
        else:
            break
    return port_amount
    

def get_outputs(output_array, input_amount):
    #for s in range(input_amount * 2, len(output_array)):
    print("hi")

    


def run():
    correct = subprocess.run(
        ["./runner"],
        universal_newlines=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )
    print(correct.stdout)
    parse_data(correct.stdout)
    # print('outputed')

def create_window():

    gui = Tk(className='Python Examples - Window Color')
    gui.geometry("800x800")

    
    return gui


def create_canvas(gui):
    #set window color
    gui.configure(background='white')

    canvas=Canvas(gui, width=800, height=700, background='white')
    canvas.pack()
    
    return canvas

def create_signal_line(canvas, signals, x0, y0, graph_width, y_inc, fill, width):
    singal_width = graph_width / len(signals)

    current_x = x0
    current_y = y0
    
    for i in range(0, len(signals)):

        if(signals[i]):
            canvas.create_line(current_x, current_y + y_inc, current_x + singal_width, current_y + y_inc, fill=fill, width=width)
        else:
            canvas.create_line(current_x, current_y, current_x + singal_width, current_y, fill=fill, width=width)
       
        current_x += singal_width

        if(i + 1 < len(signals) and signals[i] != signals[i + 1]):
            if(signals[i]):
                canvas.create_line(current_x, current_y, current_x, current_y + y_inc, fill=fill, width=width)
            else:
                canvas.create_line(current_x, current_y + y_inc, current_x, current_y, fill=fill, width=width)
        




gui = create_window()
canvas = create_canvas(gui)



def create_signals(signals_array, graph_height, graph_width, graph_x, init_y):
    signal_height = graph_height / len(signals_array)
    current_y = init_y
    for signals in signals_array:
        create_signal_line(canvas, signals, graph_x, current_y, graph_width, graph_height / len(signals_array) - 20, 'green', 2)
        current_y += signal_height



signalA = [True, True, True, True, False, True]
signalB = [True, True, False, True, False, False]
signalC = [False, True, False, True, False, True]
signals = [signalA, signalA, signalB, signalC, signalA, signalA, signalA, signalA, signalA, signalA]
create_signals(signals, 500, 600, 100, 100)

canvas.create_line(100, 600, 100, 100, fill='black', width=5)
canvas.create_line(100, 600, 700, 600, fill='black', width=5)

button = Button(gui, text="Next", command=run())
button.pack(side=RIGHT)
run()


gui.mainloop() 


