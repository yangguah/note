import matplotlib.pyplot as plt
def a(n):
    return ((5/6)**n)*3*n
A=[]
B=[]
for i in range(1,21):
    A.append(i)
    B.append(a(i))

plt.ylabel("y axis   Probability")
plt.xlabel("x axis   total waiting time")
plt.plot(A,B)

plt.show()



    

