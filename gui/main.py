import sys, pygame
from pygame.locals import*
import math
from time import sleep, time
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

prev_key_status = 'S'
key_status = 'S'

keys = {
    'w': False,
    'a': False,
    's': False,
    'd': False
}

def process_keys():
    if(keys['w'] == True and keys['a'] == False and keys['s'] == False and keys['d'] == False):
        return 'W'
    if(keys['w'] == False and keys['a'] == True and keys['s'] == False and keys['d'] == False):
        return 'A'
    if(keys['w'] == False and keys['a'] == False and keys['s'] == True and keys['d'] == False):
        return 'X'
    if(keys['w'] == False and keys['a'] == False and keys['s'] == False and keys['d'] == True):
        return 'D'
    if(keys['w'] == True and keys['a'] == True and keys['s'] == False and keys['d'] == False):
        return 'Q'
    if(keys['w'] == True and keys['a'] == False and keys['s'] == False and keys['d'] == True):
        return 'E'
    if(keys['w'] == False and keys['a'] == True and keys['s'] == True and keys['d'] == False):
        return 'Z'
    if(keys['w'] == False and keys['a'] == False and keys['s'] == True and keys['d'] == True):
        return 'C'
    return 'S'

def main():

    last_time = 0
    prev_key_status = 'S'
    key_status = 'S'

    screen=pygame.display.set_mode((width,height))
    screen.fill(screen_color)

    #main loop
    while True:
        
        if time() - last_time > 1:
            screen.fill(screen_color)
            webstring = requests.get("http://192.168.4.1").text
            webstring = webstring.split("\r\n")[0]
            webstring = webstring[:-1]
            print(webstring.split("\r")[0])
            got_array = arrayfromstr(webstring.split("\r\n")[0])
            for i in range(37):
                length_value = min(got_array[i] / 10, 200) * 2
                if(length_value == 0):
                    length_value = 400
                #this is the standard line
                #pygame.draw.line(screen,line_color, (500, 450), (500 + math.cos(math.radians(i*5)) * 400, 450 - math.sin(math.radians(i*5)) * 400 ))
                initialx = 500 + math.cos(math.radians(i*5)) * length_value
                initialy = 450 - math.sin(math.radians(i*5)) * length_value
                finalx = 500 + math.cos(math.radians(i*5)) * 400
                finaly = 450 - math.sin(math.radians(i*5)) * 400
                pygame.draw.line(screen,line_color, (initialx, initialy), (finalx, finaly))
            
            pygame.display.flip()        
            last_time = time()
        

        for events in pygame.event.get():
            if events.type == QUIT:
                sys.exit(0)
            if events.type == pygame.KEYDOWN:
                if events.key == pygame.K_w:
                    keys["w"] = True
                if events.key == pygame.K_a:
                    keys["a"] = True
                if events.key == pygame.K_s:
                    keys["s"] = True
                if events.key == pygame.K_d:
                    keys["d"] = True
                key_status = process_keys()
                if key_status != prev_key_status:
                    webstring = requests.get("http://192.168.4.1/" + key_status).text
                prev_key_status = key_status
                print("hiii")
                print(key_status)
            elif events.type == pygame.KEYUP:
                if events.key == pygame.K_w:
                    keys["w"] = False
                if events.key == pygame.K_a:
                    keys["a"] = False
                if events.key == pygame.K_s:
                    keys["s"] = False
                if events.key == pygame.K_d:
                    keys["d"] = False
                key_status = process_keys()
                if key_status != prev_key_status:
                    webstring = requests.get("http://192.168.4.1/" + key_status).text
                prev_key_status = key_status
                print("nana")
                print(key_status)
main()
