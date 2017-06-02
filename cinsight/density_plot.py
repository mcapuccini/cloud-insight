import sys
import json
import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import gaussian_kde

with open(sys.argv[1], 'r') as f:
    epsilons = json.load(f)
with open(sys.argv[2], 'r') as f:
    data = np.array(json.load(f))

xmin = data[:, :, 0].min()
xmax = data[:, :, 0].max()
ymin = data[:, :, 1].min()
ymax = data[:, :, 1].max()

f, ax = plt.subplots(1, 4, sharey=True)

for k, i in enumerate([int(s) for s in sys.argv[3:]]):
    x = data[i, :, 0]
    y = data[i, :, 1]

    xy = np.vstack([x, y])
    z = gaussian_kde(xy)(xy)

    idx = z.argsort()
    x, y, z = x[idx], y[idx], z[idx]

    ax[k].set_title(epsilons[i])
    ax[k].set_xlim(xmin, xmax)
    ax[k].set_ylim(ymin, ymax)
    ax[k].axvline(x=7.5, color='r', linewidth=3)
    ax[k].axhline(y=0.95, color='r', linewidth=3)
    ax[k].axvline(x=np.mean(x), color='g', linewidth=3, linestyle='--')
    ax[k].axhline(y=np.mean(y), color='g', linewidth=3, linestyle='--')

    ax[k].scatter(x, y, c=z, s=25, edgecolor='', cmap=plt.cm.get_cmap('Blues'))
    print(epsilons[i], np.mean(x), np.mean(y))

plt.xlabel('Birth Rate')
plt.ylabel('Death Rate')
plt.show()
