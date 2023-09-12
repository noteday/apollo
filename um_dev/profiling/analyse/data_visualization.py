# Load and visualize the profiling results
# Yuting Xie
# 2022.3.15

import numpy as np
import matplotlib.pyplot as plt
import os

COLORS = ['blue', 'yellowgreen', 'brown',
          'dimgray', 'deeppink', 'gold',
          'black', 'salmon', 'violet',
          'coral', 'red', 'peru',
          'plum', 'orchid', 'mediumturquoise',]

MARKERS = ['.', '^', '*', 'D', 'o', ',', '_', 's', 'p']

WANTED = [
  "CameraObstacleDetectionComponent::OnReceiveImage",
  "FusionCameraDetectionComponent::OnReceiveImage",
  "DetectionComponent::Proc",
  "PlanningComponent::Proc",
]

FUNC_TO_TASK_NAME = {
  "CameraObstacleDetectionComponent::OnReceiveImage" : "Object Detection",
  "FusionCameraDetectionComponent::OnReceiveImage" : "Configurable Sensor Fusion",
  "DetectionComponent::Proc" : "3D Objection Detection",
  "PlanningComponent::Proc" : "Path Planner",
}

# Set matplotlib to use SciencePlots
plt.style.use(['science','ieee'])

def load_data_from_file(file_path, results):
  with open(file_path) as f:
    for line in f.readlines():
      line = line.strip()
      if len(line) == 0:
        continue
      _, _, task, time = line.split()
      task = task.strip(':')
      time = float(time.strip('ms'))
      if task in results:
        results[task].append(time)
      else:
        results[task] = [time]

def load_data_from_dir(dir_path):
  results = {}
  for root, dirs, files in os.walk(dir_path):
    for file in files:
      file = os.path.join(root, file)
      load_data_from_file(file, results)
  return results

def plot_data(results):
  idx = 0
  for task_name, data_list in results.items():
    if task_name not in WANTED:
      continue
    data = np.array(data_list)
    plt.plot(range(len(data_list)), data,
             color=COLORS[idx % len(COLORS)],
             label=FUNC_TO_TASK_NAME[task_name],
             marker=MARKERS[idx % len(MARKERS)],
             linewidth=0.8,
             markersize=0.8
             )
    plt.ylabel("Execution Time(ms)")
    plt.xlabel("Time(s)")
    idx += 1
  plt.legend()
  plt.axis([0, 60, 0, 200])
  plt.tight_layout()
  # plt.savefig("result.png")
  plt.savefig("result.pdf", bbox_inches='tight', pad_inches=0)
  plt.show()

def plot_comparative(result1, result2):
  for task_name, data1 in result1.items():
    if task_name not in result2:
      continue
    data2 = result2[task_name]
    data1, data2 = np.array(data1), np.array(data2)
    plt.title(task_name)

    # Plot resul1
    plt.plot(range(len(data1)), data1,
             color='blue',
             label='solo',
             marker='*',
             linewidth=0.8,
             markersize=0.8
             )

    # Plot result2
    plt.plot(range(len(data2)), data2,
             color='red',
             label='concurrent',
             marker='^',
             linewidth=0.8,
             markersize=0.8
             )
    plt.ylabel("Latency(ms)")
    plt.xlabel("Time(s)")
    plt.legend()
    plt.axis([0, 60, 0, 200])
    plt.savefig("result_" + task_name.replace('::', "_") + ".png")
    plt.show()


if __name__ == "__main__":
  # result_solo = load_data_from_dir("/home/tt/Codes/apollo/um_dev/profiling/results/lgsvl_4_2_crash_case_solo_mode")
  # plot_data(results)
  # result_whole = load_data_from_dir("/home/tt/Codes/apollo/um_dev/profiling/results/lgsvl_4_2_crash_case_whole_mode")
  tl_result = load_data_from_dir("/home/tt/Codes/apollo/um_dev/profiling/results/lgsvl_4_2_crash_case_whole_mode")
  plot_data(tl_result)
