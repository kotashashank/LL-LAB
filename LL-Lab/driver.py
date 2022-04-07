import subprocess
import sys
from signal import signal
from tkinter import *
import re
import time
from draw_circuit import generate_base_graph, gate_prep
sys.path.append("/u/ljv/.local/lib/python2.7/site-packages")#this is needed for my machine
import graphviz

WIDTH = 1000
HEIGHT = 1000


def parse_data(output):
    

    output_array = output.split()

    port_counts = get_ports(output_array)

    input_amount = port_counts[0]
    output_amount = port_counts[1]

    total_ports = input_amount + output_amount
    output_array = list(filter(lambda a: a != "port", output_array))


    signals_array = []
    for i in range(0, total_ports):
        signals_array.append([])

    times_array = []

    i = 0
    
    for s in output_array:
        if i > total_ports:
            i = 0
        if(i == 0):
            times_array.append(int(re.sub(r'[\D_]+', '', s)))
            # print(times_array)
        if(i >= 1 and i <= total_ports):
            signals_array[i - 1].append(bool(int(s)))

        i += 1
        
    return (signals_array, times_array, input_amount)

    

def get_ports(output_array):
    input_amount = 0
    output_amount = 0
    for s in output_array:
        if s != "port":
            break
        input_amount += 1
    for s in output_array[input_amount * 2 + 1:len(output_array)]:
        print(s)
        if(s[0] == "["):
            break
        output_amount += 1
    return (input_amount, output_amount)
    

def run():
    correct = subprocess.run(
        ["./runner2"],
        universal_newlines=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )
    output = correct.stdout.splitlines()
       
    
    #print(correct)
    try:
        gates, ports, wires = gate_prep(output)
        base_src = generate_base_graph(ports, gates, wires)
        print(base_src)
        src = graphviz.Source(base_src)

        #dot = graphviz.Digraph("round-table",comment="trial" )
      
        #doctest_mark_exe()
        src.render("base_out")

    except:
        print("circuit generation failed for this - apologies prototype still in development")
    print(correct.stdout)
    return parse_data(correct.stdout)
    # print('outputed')

def create_window():

    gui = Tk(className='Python Examples - Window Color')
    gui.geometry("1000x1000")

    
    return gui


def create_canvas(gui):
    #set window color
    gui.configure(background='white')

    canvas=Canvas(gui, width=1000, height=1000, background='white')
    canvas.pack()
    
    return canvas

def create_signal_line(canvas, signals, x0, y0, graph_width, y_inc, fill, width):
    singal_width = graph_width / len(signals)

    current_x = x0
    current_y = y0
    
    for i in range(0, len(signals)):

        if(not signals[i]):
            canvas.create_line(current_x, current_y + y_inc, current_x + singal_width, current_y + y_inc, fill=fill, width=width)
        else:
            canvas.create_line(current_x, current_y, current_x + singal_width, current_y, fill=fill, width=width)
        current_x += singal_width

        if(i + 1 < len(signals) and signals[i] != signals[i + 1]):
            if(not signals[i]):
                canvas.create_line(current_x, current_y, current_x, current_y + y_inc, fill=fill, width=width)
            else:
                canvas.create_line(current_x, current_y + y_inc, current_x, current_y, fill=fill, width=width)


        gui.update()
        time.sleep(0.005)


        



run()#delete later
gui = create_window()
canvas = create_canvas(gui)






def create_signals(signals_array, time_array, input_amount, graph_height, graph_width, x0, y0, fillIn, fillOut, width, step):
    signal_height = graph_height / len(signals_array)
    signal_width = graph_width / len(time_array)
    current_x = x0
    y_inc = graph_height / len(signals_array) - 20
    for t in range(0, len(time_array)):
        for i in range(0, len(signals_array)):
            fill = fillIn if i < input_amount else fillOut
            current_y = y0 + i * signal_height
            
            if(not signals_array[i][t]):
                canvas.create_line(current_x, current_y + y_inc, current_x + signal_width, current_y + y_inc, fill=fill, width=width)
            else:
                canvas.create_line(current_x, current_y, current_x + signal_width, current_y, fill=fill, width=width)

            if(t + 1 < len(time_array) and signals_array[i][t] != signals_array[i][t + 1]):
                if(not signals_array[i][t]):
                     canvas.create_line(current_x + signal_width, current_y, current_x + signal_width, current_y + y_inc, fill=fill, width=width)
                else:
                    canvas.create_line(current_x + signal_width, current_y + y_inc, current_x + signal_width, current_y, fill=fill, width=width)

        gui.update()
        time.sleep(step)
        current_x += signal_width
    

def create_ticks(time_array, x_init, y, graph_width):
    x_inc = graph_width/len(time_array)

    for i in range(0, len(time_array)):
        canvas.create_line(x_init + x_inc*i + x_inc, y - 7.5, x_init + x_inc*i + x_inc, y + 10, fill='black', width=1.5)




canvas.create_line(100, 750, 100, 100, fill='black', width=5)
canvas.create_line(100, 750, 900, 750, fill='black', width=5)


c_output = run() # 0 -> signal array | #1 -> input amount
create_ticks(c_output[1], 100, 750, 800)

create_signals(c_output[0], c_output[1], c_output[2], 650, 800, 100, 100, fillIn='green', fillOut='red', width=4, step=0.015)

canvas.create_line(100, 750, 100, 100, fill='black', width=5)
canvas.create_line(100, 750, 900, 750, fill='black', width=5)






gui.mainloop()