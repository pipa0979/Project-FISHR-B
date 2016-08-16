import sys
import numpy as np
import matplotlib.pyplot as plt

# print 'Number of arguments:', len(sys.argv), 'arguments.'
# print 'Argument List:', str(sys.argv)
# print 'Argument List:', sys.argv[1]
errorlist = sys.argv[1]
errorlist = errorlist.split("/")
count = len(errorlist)

plt.plot(range(count), errorlist, 'ro--')
plt.axis([-1, count+1, -0.1, 1.5])
plt.show()