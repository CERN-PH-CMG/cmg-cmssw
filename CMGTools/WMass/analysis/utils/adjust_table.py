import os, sys, ROOT
import os.path
import numpy as np

input_filename = "FEB15_Pos.txt"
number_of_variables_on_the_same_line = 3 # pt, mt, met

with open(input_filename, "r") as ins:
    array = []
    line_temp = ''
    var_counter = 0
    for line in ins:
        line = line.replace('\n','').replace('\t',' ')
        if(not (line.startswith("#") or line.startswith("=")) ):
          var_counter = var_counter+1
          if not line_temp == '':
            line_temp = str(line_temp)+"  "+(line)
          else:
            line_temp = line
        if(line.startswith("#") or line.startswith("=")):
          array.append(line)
        elif(var_counter == number_of_variables_on_the_same_line):
          array.append(line_temp)
          var_counter = 0
          line_temp = ''
          


output_filename = open(os.path.splitext(input_filename)[0]+"_sorted.txt", "w")
for line in array:
  output_filename.write(line+"\n")
output_filename.close()
