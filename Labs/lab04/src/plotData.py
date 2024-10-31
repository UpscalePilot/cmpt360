import os
import datetime
import random
# from graphics import *
import tkinter as tk
from PIL import Image


def drawrectangle(pid, cpuTime):

    totalCPUTime = 0.00
    turnaround = 0.00
    x1 = 50.00

    # this uses graphics.py******************************************************
    # rect = []
    # message = []
    # this uses graphics.py******************************************************

    for addCPUTime in cpuTime:
        totalCPUTime = totalCPUTime + float(addCPUTime)
    
    #win = GraphWin("PID Process, Plot Data", 1000, 200)
    root = tk.Tk()
    root.title("PID Process, Plot Data")
    myCanvas = tk.Canvas(height=200, width=1000)

    # number of pid processes
    for i in range(len(pid)):
        de=("%02x"%random.randint(0,255))
        re=("%02x"%random.randint(0,255))
        we=("%02x"%random.randint(0,255))
        ge="#"
        color=ge+de+re+we

        # our canvas is 1000 in width, 50 on each side will be left blank, x range (50 <-> 950)
        # x1 is ((turnaround time/total time) * canvas total width) + 50
        turnaround = turnaround + float(cpuTime[i])
        x2 = ((turnaround / totalCPUTime) * 900.00) + 50.00

        myCanvas.create_rectangle(x1, 50, x2, 150, outline='black', fill=color)
        myCanvas.create_text((x1+x2)/2, 100, text=pid[i], font=('Helvetica','20', 'bold'))


        # this uses graphics.py******************************************************
        # rect.append(Rectangle(Point(x1, 50), Point(x2, 150)))
        # rect[i].setFill(color)
        # rect[i].setOutline('black')
        # rect[i].draw(win)
        # message.append(Text(Point((x1+x2)/2.0, 100), pid[i]))
        # message[i].draw(win)
        # this uses graphics.py******************************************************

        # x1=x2 starting point from endpoint of last rectangle
        x1 = x2

    myCanvas.pack()
    myCanvas.update()

    # get directory path to log folder
    current_dir = os.getcwd()
    current_dir = current_dir + "/pSimulator/log/"
    os.chdir(current_dir)

    # get current dd/mm/yyyy
    current_time = datetime.datetime.now()
    imageFile = "img-" + str("{:02d}".format(current_time.month)) + "-" + str("{:02d}".format(current_time.day)) + "-" + str(current_time.year)

    # create .ps file, then convert .ps to .png
    myCanvas.postscript(file=imageFile + '.ps', colormode='color')
    img = Image.open(imageFile + '.ps')
    img.save(imageFile + '.png', 'png')
    img.close()

    # remove .ps file
    if os.path.exists(imageFile + '.ps'):
        os.remove(imageFile + '.ps')
    else:
        print("The file does not exist")

    # this uses graphics.py******************************************************
    # win.getMouse()
    # win.close()
    # this uses graphics.py******************************************************
    
    # print
    # root.mainloop()

    return

def main():

    # get directory path to log folder
    current_dir = os.getcwd()
    current_dir = current_dir + "/pSimulator/log/"
    files_in_dir = os.listdir(current_dir)
    
    current_time = datetime.datetime.now()

    openTxtFile = "log-" + str("{:02d}".format(current_time.month)) + "-" + str("{:02d}".format(current_time.day)) + "-" + str(current_time.year) + ".txt"
    
    pid = []
    cpuTime = []

    # iterate through the log folder, look for 'i' for img
    for file_name in files_in_dir:
        if (file_name == openTxtFile):
            txtFile = open(current_dir + file_name, "r")
            # skip the header
            next(txtFile)
            txtLines = txtFile.readlines()

            # read line by line, strip/split make tuple by empty white spaces
            for line in txtLines:
                structuredLine = [x.strip() for x in line.split()]

                # ensure the data we pull from is the running process, ensure the process is completed
                if (structuredLine[6] == "Running" and float(structuredLine[4]) >= float(structuredLine[5])):
                    pid.append(structuredLine[1])
                    cpuTime.append(structuredLine[4])

            txtFile.close()

    drawrectangle(pid, cpuTime)

    return


if __name__ == "__main__":
    main()