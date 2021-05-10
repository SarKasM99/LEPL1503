from tkinter import * 
import numpy as np


def Make_rapport(title,p,t1,t2,t3,t4):
    fenetre = Tk()
    
    canvas = Canvas(fenetre,width=600,height=700, background='light grey')
    canvas.create_line(100,100,100,800)
    canvas.create_line(200,100,200,800)
    canvas.create_line(300,100,300,800)
    canvas.create_line(400,100,400,800)
    canvas.create_line(500,100,500,800)

    canvas.create_line(0,100,700,100)
    canvas.create_line(0,200,700,200)
    canvas.create_line(0,300,700,300)
    canvas.create_line(0,400,700,400)
    canvas.create_line(0,500,700,500)
    canvas.create_line(0,600,700,600)

    canvas.create_text(300,50,text=title,font="Comic 14", fill="black")
    canvas.create_text(150,150,text="python",font="Comic 14", fill="blue")
    canvas.create_text(50,250,text="python",font="Comic 14", fill="blue")
    canvas.create_text(250,150,text="unithread",font="Comic 14", fill="green")
    canvas.create_text(50,350,text="unithread",font="Comic 14", fill="green")
    canvas.create_text(350,150,text="threads=2",font="Comic 14", fill="yellow")
    canvas.create_text(50,450,text="threads=2",font="Comic 14", fill="yellow")
    canvas.create_text(450,150,text="threads=3",font="Comic 14", fill="orange")
    canvas.create_text(50,550,text="threads=3",font="Comic 14", fill="orange")
    canvas.create_text(550,150,text="threads=4",font="Comic 14", fill="red")
    canvas.create_text(50,650,text="threads=4",font="Comic 14", fill="red")
    canvas.create_text(50,150,text="H / V",font="Comic 16", fill="black")

    canvas.create_text(150,250,text="1.00",font="Comic 12", fill="black")
    canvas.create_text(250,350,text="1.00",font="Comic 12", fill="black")
    canvas.create_text(350,450,text="1.00",font="Comic 12", fill="black")
    canvas.create_text(450,550,text="1.00",font="Comic 12", fill="black")
    canvas.create_text(550,650,text="1.00",font="Comic 12", fill="black")

    canvas.create_text(150,350,text=(np.sum(p)/np.sum(t1))*10000//1/10000,font="Comic 12", fill="black")
    canvas.create_text(250,250,text=(np.sum(t1)/np.sum(p))*10000//1/10000,font="Comic 12", fill="black")

    canvas.create_text(150,450,text=(np.sum(p)/np.sum(t2))*10000//1/10000,font="Comic 12", fill="black")
    canvas.create_text(350,250,text=(np.sum(t2)/np.sum(p))*10000//1/10000,font="Comic 12", fill="black")

    canvas.create_text(150,550,text=(np.sum(p)/np.sum(t3))*10000//1/10000,font="Comic 12", fill="black")
    canvas.create_text(450,250,text=(np.sum(t3)/np.sum(p))*10000//1/10000,font="Comic 12", fill="black")

    canvas.create_text(150,650,text=(np.sum(p)/np.sum(t4))*10000//1/10000,font="Comic 12", fill="black")
    canvas.create_text(550,250,text=(np.sum(t4)/np.sum(p))*10000//1/10000,font="Comic 12", fill="black")


    canvas.create_text(250,450,text=(np.sum(t1)/np.sum(t2))*10000//1/10000,font="Comic 12", fill="black")
    canvas.create_text(350,350,text=(np.sum(t2)/np.sum(t1))*10000//1/10000,font="Comic 12", fill="black")

    canvas.create_text(250,550,text=(np.sum(t1)/np.sum(t3))*10000//1/10000,font="Comic 12", fill="black")
    canvas.create_text(450,350,text=(np.sum(t3)/np.sum(t1))*10000//1/10000,font="Comic 12", fill="black")

    canvas.create_text(250,650,text=(np.sum(t1)/np.sum(t4))*10000//1/10000,font="Comic 12", fill="black")
    canvas.create_text(550,350,text=(np.sum(t4)/np.sum(t1))*10000//1/10000,font="Comic 12", fill="black")


    canvas.create_text(350,550,text=(np.sum(t2)/np.sum(t3))*10000//1/10000,font="Comic 12", fill="black")
    canvas.create_text(450,450,text=(np.sum(t3)/np.sum(t2))*10000//1/10000,font="Comic 12", fill="black")

    canvas.create_text(350,650,text=(np.sum(t2)/np.sum(t4))*10000//1/10000,font="Comic 12", fill="black")
    canvas.create_text(550,450,text=(np.sum(t4)/np.sum(t2))*10000//1/10000,font="Comic 12", fill="black")


    canvas.create_text(450,650,text=(np.sum(t3)/np.sum(t4))*10000//1/10000,font="Comic 12", fill="black")
    canvas.create_text(550,550,text=(np.sum(t4)/np.sum(t3))*10000//1/10000,font="Comic 12", fill="black")


    canvas.pack()
