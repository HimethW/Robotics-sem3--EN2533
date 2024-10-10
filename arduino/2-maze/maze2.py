import turtle
import time
t = turtle.Turtle()
t.speed(10)  
t.clear()
t.penup()
t.setpos(200, 200)
t.setheading(270)
t.pendown()
t.shape('arrow')
t.color('black')

def f():
    for x in range(100):
        t.forward(1)

def r():
    t.right(90)

def b():
    for x in range(100):
        t.backward(1)

def l():
    t.left(90)

def pickup():
    t.color('blue')

def drop():
    t.color('black')

def drawgrid():
    t.width(3)
    t.forward(100)
    t.right(90)
    for x in range(4):
        t.forward(100)
    t.right(90)
    t.forward(100)
    t.right(90)
    for x in range(4):
        t.forward(100)
    t.right(90)
    t.penup()
    t.setpos(100, 200)
    t.pendown()
    t.forward(100)
    t.write(1, font=("Verdana", 15, "normal"))
    t.penup()
    t.setpos(0, 200)
    t.pendown()
    t.forward(100)
    t.penup()
    t.setpos(-100, 200)
    t.pendown()
    t.forward(100)
    t.write(2, font=("Verdana", 15, "normal"))
    t.penup()
    t.setpos(200, 300)
    f()
    t.width(10)

def movebox():
    if result == 0:
        pickup()
        f()
        drop()
        b()
        r()
        f()
        l()
        f()
    else:
        f()
        r()
        f()
        l()

def findgate():
    time.sleep(0.5)
    t.write('check gate1', font=("Verdana", 10, "normal"), align="right")
    r()
    f()
    f()
    l()
    time.sleep(0.5)
    t.write('check gate2', font=("Verdana", 10, "normal"), align="right")

def func0():
    l()
    f()
    f()
    f()
    pickup()
    b()
    if opengate == 1:
        drop()
        b()
        l()
        f()
        r()
        f()
        r()
        f()
        pickup()
        f()
    else:
        b()
        b()
        drop()
        b()
        l()
        f()
        r()
        f()
        r()
        f()
        pickup()
        f()

def func1():
    if opengate == 1:
        l()
        f()
        f()
        l()
        f()
        pickup()
        b()
        b()
    else:
        b()
        l()
        f()
        f()
        pickup()
        b()
        b()
        drop()
        b()
        r()
        f()
        l()
        f()
        l()
        f()
        pickup()
        b()
        b()
        
def func2():
    b()
    l()
    f()
    pickup()
    if opengate == 1:
        f()
        drop()
        b()
        r()
        f()
        l()
        f()
        l()
        f()
        pickup()
        b()
        b()
    else:
        b()
        drop()
        b()
        r()
        f()
        l()
        f()
        l()
        f()
        pickup()
        b()
        b()
        
def func3():
    if opengate == 1:
        r()
        f()
        r()
        f()
        r()
        f()
        pickup()
        f()
        f()
        drop()
        b()
        r()
        f()
        l()
        f()
        l()
        f() 
        pickup()
        b()
        b()
    else:
        r()
        r()
        f()
        pickup()
        b()
        b()
        
def func4():
    r()
    r()
    f()
    l()
    f()
    pickup()
    b()
    if opengate == 1:
        b()
        b()
    drop() 
    b()
    l()
    f()
    r()
    f()
    r()
    f()
    pickup()
    b()
    b()

number = 9
opengate = 2

drawgrid()
result = number % 5

# Code starts here
movebox()
findgate()
if result == 0:
    func0()
elif result == 1:
    func1()
elif result == 2:
    func2()
elif result == 3:
    func3()
elif result == 4:
    func4()
