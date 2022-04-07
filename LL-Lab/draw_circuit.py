import sys
from tkinter import *
import os
sys.path.append("/u/ljv/.local/lib/python2.7/site-packages")#this is needed for my machine
import graphviz

def generate_base_graph(ports, gates, wires):
    toRet = "digraph {\n \trankdir=LR;\n\tnode [shape=record];\n\n"


    map_input_ports = {}#given address,name give struct its in 
    map_out_ports = {}#given address,name give struct its in 
    num_at = -1
    for gate in gates:
        struct_string = ""#this is the string that represents graphviz struct
        num_at +=1
        current_struct = "struct" + str(num_at)

        struct_string += current_struct + "[\n\t"

        split1 = gate.split(";")
        first_half_factored = split1[0].split(",") 
        gate_type = first_half_factored[0]
        struct_string += 'label = "' + gate_type +"|{{"

        num_in = first_half_factored[1]
        name_out = first_half_factored[2] + "," + first_half_factored[3] 

        out_ad = name_out.split(",")[0]
        if (not out_ad in map_out_ports.keys()):#making a list of all structs attatched to the ports
            map_out_ports[out_ad] = [current_struct+":m"+out_ad]
        else:
            map_out_ports[out_ad].append(current_struct+":m"+out_ad)
        #we will add the output port w/ name after we have added import

        second_half_factored = split1[1]
        second_half_factored = second_half_factored.split("]")
        for block_in in second_half_factored[:-1]:
            data = block_in.strip()[1:]#have name of each input block
            in_ad = data.split(",")[0]
            
            struct_string += "<m" + in_ad +">"+data.split(",")[1]+"|" 
            #label = "a|{{b|<port1>c}|d}"; = gate format where b and c are what we need
            if (not in_ad in map_input_ports.keys()):
                map_input_ports[in_ad] = [current_struct +":m"+in_ad]
            else:
                map_input_ports[in_ad].append(current_struct+":m"+in_ad)
        struct_string = struct_string[:-1]#remove the last or b/c its not valid
        struct_string += "}|"#now we can add the output
        struct_string += "<m"+out_ad + ">" + name_out.split(",")[1] + '}";\n];\n'
        toRet += struct_string + "\n\n"
    #print(toRet)
    num_at = 0
    portToAd = {}
    for port in ports:
        ad, name = port.split(",")
        if (ad in map_input_ports or ad in map_out_ports):
            continue
        num_at +=1
        if (name.strip()=="((null))"):
            toRet += "port" + str(num_at)+'[\n\tlabel="<m{mem}>m{mem}"\n\t]\n\n'.format(mem=ad.strip())
        else:
            toRet += "port" + str(num_at)+'[\n\tlabel="<m{mem}>{name}"\n\t]\n\n'.format(mem=ad.strip(), name = name.strip())
        if (not ad in portToAd.keys()):
            portToAd[ad] = ["port"+str(num_at)+":m"+ad.strip()]
        else:
            portToAd[ad].append("port"+str(num_at)+":m"+ad.strip())
    for wire in wires:
        first, second = wire.split(";")
        first = first.split(",")[0]
        second = second.split(",")[0]
        if (first in portToAd.keys()):
            #get all outs
            start = portToAd[first]
        elif (first.split(",")[0] in map_out_ports.keys()):
            start = map_out_ports[first]
        else:
            print("should have been one of the two")
            exit(0)
        assert(len(start) == 1)
        outs = []
        if (second in portToAd.keys()):
            outs += portToAd[second]
        if (second in map_input_ports):
            outs += map_input_ports[second]
        start = start[0]
        for out in outs:
            toRet += "\n\t" + start +"->" + out + ";\n"

    toRet += "\n}"
    return toRet

def gate_prep(output):
    nextGate = nextPort = nextWire = False
    gates = []
    ports = []
    wires = []
    for line in output:
        if (nextGate):
            gates.append(line.strip())
            nextGate = False
        elif (nextWire):
            wires.append(line.strip())
            nextWire = False
        elif (nextPort):
            ports.append(line.strip())
            nextPort = False
        elif ("creating" in line):
            if ("wire" in line):
                nextWire = True
            elif ("port" in line):
                nextPort = True
            elif ("gate" in line):
                nextGate = True 
            else:
                exit(0)
    return gates, ports, wires
