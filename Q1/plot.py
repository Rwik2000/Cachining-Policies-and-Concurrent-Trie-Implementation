import matplotlib.pyplot as plt
import csv
x=[]
for i in range(5,301):
    x.append(i)

# PLOTTING reader inntensive workload
time_hoh_reader=[]
with open('plot/hoh_lock/reader_intensive.csv') as csvfile:
    plots=csv.reader(csvfile,delimiter=',')
    for col in plots:
        time_hoh_reader.append(float(col[1]))
time_single_reader=[]
with open('plot/single_lock/reader_intensive.csv') as csvfile:
    plots=csv.reader(csvfile,delimiter=',')
    for col in plots:
        time_single_reader.append(float(col[1]))
time_rw_reader=[]
with open('plot/rw_lock/reader_intensive.csv') as csvfile:
    plots=csv.reader(csvfile,delimiter=',')
    for col in plots:
        time_rw_reader.append(float(col[1]))
print(time_rw_reader)
fig=plt.figure()
plt.plot(x,time_hoh_reader,marker=".")
plt.plot(x,time_rw_reader,marker="^")
plt.plot(x,time_single_reader,marker="p")
plt.legend(["hoh","rw","single"])
plt.show()

# PLOTTING writer intensive workload
time_hoh_writer=[]
with open('plot/hoh_lock/writer_intensive.csv') as csvfile:
    plots=csv.reader(csvfile,delimiter=',')
    for col in plots:
        time_hoh_writer.append(float(col[1]))
time_single_writer=[]
with open('plot/single_lock/writer_intensive.csv') as csvfile:
    plots=csv.reader(csvfile,delimiter=',')
    for col in plots:
        time_single_writer.append(float(col[1]))
time_rw_writer=[]
with open('plot/rw_lock/writer_intensive.csv') as csvfile:
    plots=csv.reader(csvfile,delimiter=',')
    for col in plots:
        time_rw_writer.append(float(col[1]))
fig=plt.figure()
plt.plot(x,time_hoh_writer,marker=".")
plt.plot(x,time_rw_writer,marker="^")
plt.plot(x,time_single_writer,marker="p")
plt.legend(["hoh","rw","single"])
plt.show()

# PLOTTING Equal intensive workload
time_hoh_equal=[]
with open('plot/hoh_lock/equal_intensive.csv') as csvfile:
    plots=csv.reader(csvfile,delimiter=',')
    for col in plots:
        time_hoh_equal.append(float(col[1]))

time_single_equal=[]
with open('plot/single_lock/equal_intensive.csv') as csvfile:
    plots=csv.reader(csvfile,delimiter=',')
    for col in plots:
        time_single_equal.append(float(col[1]))
time_rw_equal=[]
with open('plot/rw_lock/equal_intensive.csv') as csvfile:
    plots=csv.reader(csvfile,delimiter=',')
    for col in plots:
        time_rw_equal.append(float(col[1]))
fig=plt.figure()
plt.plot(x,time_hoh_equal,marker=".")
plt.plot(x,time_rw_equal,marker="^")
plt.plot(x,time_single_equal,marker="p")
plt.legend(["hoh","rw","single"])
plt.show()






    
    
