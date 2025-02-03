import sys, pygame
from pygame.locals import*
import math
from time import sleep
import requests


def arrayfromstr(in_string):
    num_array = []
    thingy = in_string.split(" ")
    for i in thingy:
        num_array.append(int(i))
    return num_array


print(arrayfromstr("317 300 576 618 642 662 667 680 703 751 676 535 519 508 462 441 430 418 425 443 462 479 490 481 478 451 431 440 437 425 403 383 361 246 100 77 69"))

width = 1000
height = 500
screen_color = (255, 255, 255)
line_color = (255, 0, 0)

def main():
    screen=pygame.display.set_mode((width,height))
    screen.fill(screen_color)

    #main loop
    while True:
        screen.fill(screen_color)
        webstring = requests.get("http://192.168.4.1").text
        webstring = webstring.split("\r\n")[0]
        webstring = webstring[:-1]
        print(webstring.split("\r")[0])
        got_array = arrayfromstr(webstring.split("\r\n")[0])
        for i in range(37):
            length_value = min(got_array[i] / 10, 100) * 4
            #this is the standard line
            #pygame.draw.line(screen,line_color, (500, 450), (500 + math.cos(math.radians(i*5)) * 400, 450 - math.sin(math.radians(i*5)) * 400 ))
            initialx = 500 + math.cos(math.radians(i*5)) * length_value
            initialy = 450 - math.sin(math.radians(i*5)) * length_value
            finalx = 500 + math.cos(math.radians(i*5)) * 400
            finaly = 450 - math.sin(math.radians(i*5)) * 400
            pygame.draw.line(screen,line_color, (initialx, initialy), (finalx, finaly))
            
        pygame.display.flip()        

        sleep(1)


        for events in pygame.event.get():
            if events.type == QUIT:
                sys.exit(0)
main()
