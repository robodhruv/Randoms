import csv
import matplotlib.pyplot as plt

store = []

with open('output.csv', 'rb') as csvfile:
	csvreader = csv.reader(csvfile)
	for row in csvreader:
		store.append(row)

plt.plot(store)
plt.legend(['Result', 'Sequence', 'Predicted'])
# plt.ion()
plt.show()