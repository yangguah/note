'''def floatrange(start,stop,steps):
    return [start+float(i)*(stop-start)/(float(steps)-1) for i in range(steps)]







import matplotlib.pyplot as plt
e=2.718
def a(n):
    return 2*(e**(-1*n)-e**(-2*n))
A=[]
B=[]
for i in floatrange(0,10,1000):
    A.append(i)
    B.append(a(i))

plt.ylabel("y axis   Probability")
plt.xlabel("x axis   total waiting time")
plt.plot(A,B)

plt.show()


use twitter;
select  count(*)
from ds_tweet t
where t.text like '%banana%' and coordinate is not null;


    

use twitter;
select  *
from ds_tweet t
where t.text like '%and%' and coordinate is not null;



use twitter;
select  coordinate, geo_tag.stateID
from ds_tweet t
limit 10;'''
def fac(n):
    a=1
    for i in range(1,n+1):
        a=a*i
    return a
def another(n):
    return (2**n)*(n*n)

print (fac(8))
print (another(9))
        


use twitter;
select  *
from ds_tweet t
where contains(t.text,'and')
limit 10 ;
