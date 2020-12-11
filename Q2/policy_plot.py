import matplotlib.pyplot as plt
import csv

# PLotting for Local Workload i.e 80-20
# Initializing list 
FIFO_local=[]
LRU_local=[]
LRU_approx_local=[]
random_local=[]
# Making anf Writing in a csv file
with open('data_csv_local.csv') as csvfile:
    # iterating through the coloumns of CSV
    plots=csv.reader(csvfile,delimiter=',')
    for col in plots:
        # Adding a column to the respective lists.
        FIFO_local.append(float(col[0]))
        LRU_local.append(float(col[1]))
        LRU_approx_local.append(float(col[2]))
        random_local.append(float(col[3]))

# Looping workload
FIFO_loop=[]
LRU_loop=[]
LRU_approx_loop=[]
random_loop=[]
with open('data_csv_loop.csv') as csvfile:
    plots=csv.reader(csvfile,delimiter=',')
    for col in plots:
        FIFO_loop.append(float(col[0]))
        LRU_loop.append(float(col[1]))
        LRU_approx_loop.append(float(col[2]))
        random_loop.append(float(col[3]))

# Random Workload
FIFO_random=[]
LRU_random=[]
LRU_approx_random=[]
random_random=[]
with open('data_csv_rand.csv') as csvfile:
    plots=csv.reader(csvfile,delimiter=',')
    for col in plots:
        FIFO_random.append(float(col[0]))
        LRU_random.append(float(col[1]))
        LRU_approx_random.append(float(col[2]))
        random_random.append(float(col[3]))


x=[]
for i in range(2,110):
    x.append(i)

# Making Subplots
fig=plt.figure()
ax_local=fig.add_subplot(3,1,1)
ax_local.title.set_text("80-20 workload")
plt.plot(x,FIFO_local,marker="^")
plt.plot(x,LRU_local,marker=".")
plt.plot(x,LRU_approx_local)
plt.plot(x,random_local,marker="p")
plt.legend(["FIFO","LRU","LRU_approx","random"])


ax_loop=fig.add_subplot(3,1,2)
ax_loop.title.set_text("Loop workload")
plt.plot(x,FIFO_loop,marker="^")
plt.plot(x,LRU_loop,marker=".")
plt.plot(x,LRU_approx_loop)
plt.plot(x,random_loop,marker="p")
plt.legend(["FIFO","LRU","LRU_approx","random"])

ax_random=fig.add_subplot(3,1,3)
ax_random.title.set_text("Random workload")
plt.plot(x,FIFO_random,marker="^")
plt.plot(x,LRU_random,marker=".")
plt.plot(x,LRU_approx_random)
plt.plot(x,random_random,marker="p")
plt.legend(["FIFO","LRU","LRU_approx","random"])
plt.subplots_adjust(left=None,bottom=None,right=None,top=None,wspace=None,hspace=1)
plt.show()

    
    
