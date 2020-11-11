import matplotlib.pyplot as plt
import numpy as np
import scipy.signal

data = np.loadtxt('data.txt')

x = []
y1 = []
y2 = []
y3 =[]

for i in range(len(data)):
    x.append((data[i][0]))
    y1.append(data[i][1])
    y2.append(data[i][2])
    y3.append(data[i][3])

x = np.array(x)
y1 = np.array(y1)
y2 = np.array(y2)
y3  = np.array(y3)

x = scipy.signal.savgol_filter(x, 13, 5)
y1 = scipy.signal.savgol_filter(y1, 13, 5)
y2 = scipy.signal.savgol_filter(y2, 13, 5)
y3 = scipy.signal.savgol_filter(y3, 13, 5)

inset, =  plt.plot(x, y1)
inset.set_label('Вставка')
find, = plt.plot(x, y2)
find.set_label('Поиск')
erase, = plt.plot(x, y3)
erase.set_label('Удаление')
plt.legend(loc='upper left')
plt.show()