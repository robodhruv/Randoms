# Author: Dhruv Ilesh Shah

import sys
import csv
import numpy as np
import matplotlib.pyplot as plt

# Importing the file and reading CSV
raw_data = np.recfromcsv(sys.argv[1])
print np.shape(raw_data)
time = raw_data.field(0)
tag = raw_data.field(1)
val = raw_data.field(2)

query = tag == sys.argv[2]
res = [(val[i], time[i]) for i in range(np.shape(query)[0]) if query[i] == 1]
print np.shape(res)
rel_vals = [res[i][0] for i in range(np.shape(res)[0])]

plt.plot(rel_vals)
plt.show()
