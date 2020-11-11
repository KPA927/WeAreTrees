import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt('data.txt')

x = []
y1 = []
y2 = []
y3 =[]

for i in range(len(data)):
    x.append(np.log(data[i][0]))
    y1.append((data[i][1]))
    y2.append((data[i][2]))
    y3.append((data[i][3]))

inset, =  plt.plot(x, y1)
inset.set_label('Вставка')
find, = plt.plot(x, y2)
find.set_label('Поиск')
erase, = plt.plot(x, y3)
erase.set_label('Удаление')
plt.legend(loc='upper left')
plt.show()