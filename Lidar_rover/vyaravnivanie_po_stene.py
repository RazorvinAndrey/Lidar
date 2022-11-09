import math

sum_proverki = 0
s = 0
for i in range(178, 183):  # считает среднее растояние справа в диапазоне пяти градусов
    if scale_X[i] != None:
        sum_proverki += scale_X[i]
        s += 1
if  s == 0:
    s = 1
dist_proverka = sum_proverki / k
angl_proverka = floor(math.atan(420/dist_proverka))
#создадим среднее из трех градусов в области проверочного градуса, чтобы избежать проскакиваний


sum_proverki_angl = 0
a = 0
for i in range(angl_proverka - 1, angl_proverka + 1):  # считает среднее растояние справа в диапазоне пяти градусов
    if scale_X[i] != None:
        sum_proverki_angl += scale_X[i]
        a += 1
if  a == 0:
    a = 1
dist_angle_proverka = sum_proverki_angl / a

if dist_angl_proverka > (dist_proverka + 100):
    #поворот налево
    method = 6
    speed = 100
elif X[angl_proverka] < (dist_proverka + 100):
    #поворот направо
    method = 5
    speed = 100
else :
    # прямо едет
    method = 1
    speed = 100

