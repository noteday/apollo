import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

from statsmodels.stats.diagnostic import acorr_ljungbox

def load_csv(filename):
  return pd.read_csv(filename, header="infer")


if __name__ == "__main__":
  data = load_csv("../../results/lgsvl_2022_06_13_23_12_12/Timing/2022-06-13 23:12:13.823242333_17212.csv")

  # white_noise = np.random.standard_normal(size=1000)
  res = acorr_ljungbox(data["result"][0:1000], lags=range(1, 100), boxpierce=True)
  print(res)
  plt.plot(range(1000), data["result"][0:1000])
  plt.show()


