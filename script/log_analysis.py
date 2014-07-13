#! /usr/bin/env python
import os
from utility import *
import numpy as np
import matplotlib.pyplot as plt

#-----------------------------functions--------------------------------------------

def grepInt(log_file,keyname,default=""):
    data = grepNumberWithKey(log_file,keyname)
    if len(data) <= 0:
        return default
    return str(int(data[0]))


def grepFloat(log_file,keyname,default=""):
    data = grepNumberWithKey(log_file,keyname)
    if len(data) <= 0:
        return default
    return str('%.5g'%(data[0]))


def grepStr(log_file,keyname,default=""):
    data = grepStrWithKey(log_file,keyname)
    if len(data) <= 0:
        return default
    return str(data[0])

def grepFloatList(log_file,keyname,default=""):
    data = grepNumberWithKey(log_file,keyname)
    if len(data) <= 0:
        return default
    d = ""
    for var in data:
        d = d +str('%.3g'%(var))+", "
    return d

def count(log_file,key):
    data_file = open(log_file)
    data_lines = data_file.readlines()
    num = 0
    for line in data_lines:
        if line.find(key) >= 0:
            num = num+1
    return num

def sum(log_file,key):
    s = 0
    data = grepNumberWithKey(log_file,key)
    for var in data:
        s = s + var
    return str(s)

def sumInt(log_file,key):
    s = 0
    data = grepNumberWithKey(log_file,key)
    for var in data:
        s = s + int(var)
    return str(s)

def grep_and_save_fig(log_file,key_list,label_list,fig_name,use_log=False,use_number_pos=False,number_position=-1):
    for i in range(0,len(key_list)):
        data = grepNumberWithKey(log_file,key_list[i],use_number_pos,number_position)
        if len(data) > 0:
            if use_log:
                plt.semilogy()
            plt.plot(range(0,len(data)),data,label=label_list[i])
    legend = plt.legend(bbox_to_anchor=(0,1.01,1,20), loc=8, ncol=8, mode="expand",borderaxespad=-0.5,labelspacing=-0.2)
    plt.savefig(fig_name)
    plt.clf()
    return fig_name

#-----------------------------main------------------------------------------------
report_tex = "./tempt/report.tex"
log_fs = os.listdir("./tempt")
tex_str = open("./script/report_head.tex").read()

for log_f in log_fs:

    if not log_f.endswith(".txt"): continue

    tempt = report_tex+"tempt"
    os.system("cp "+"./script/report_data.tex "+tempt)
    log_f = "./tempt/"+log_f

    fig = grep_and_save_fig(log_f,["cg steps: ","exp steps: "," prop steps: ","MPRGP iter: ","power iter:"],
                            ["cg","exp"," prop","MPRGP","power"],
                            log_f[0:-4]+"iterations.png",True)

    changeElements(tempt,"#iter_curves#",fig.replace("_","\\string_"))

    fig = grep_and_save_fig(log_f,["constraints:"],["con nodes"],log_f[0:-4]+"con_nodes.png")

    changeElements(tempt,"#con_nodes#",fig.replace("_","\\string_"))

    fig = grep_and_save_fig(log_f,["PlaneCollisionHandling::solve","PRECONDITION>::solve(","evaluateK:","evaluateF:","PRECONDITION>::specRad("],
                            ["total","MPRGP","K(x)","f(x)","power"],
                            log_f[0:-4]+"total_time.png",False,True,-1)

    changeElements(tempt,"#time_fig#",fig.replace("_","\\string_"))

    changeElements(tempt,"#number_of_nodes#", grepInt(log_f, "number of nodes:"))
    changeElements(tempt,"#number_of_tets#", grepInt(log_f, "number of tets:"))
    changeElements(tempt,"#time_step#", grepFloat(log_f, "time step:"))

    initfilename = grepStr(log_f,"init file:")
    if len(initfilename) > 0:
        changeElements(tempt,"#init_file_name#", initfilename.split()[-1].replace("_","\\_") )
    else:
        changeElements(tempt,"#init_file_name#", "" )

    log_f = "./tempt/"+log_f
    tex_str += open(tempt).read()

open(report_tex,'w').write(tex_str+"\n\end{document}")
os.system("pdflatex "+" -output-directory="+os.path.dirname(report_tex)+" "+report_tex)
