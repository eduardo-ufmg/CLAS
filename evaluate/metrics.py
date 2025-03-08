import subprocess
from sklearn.metrics import auc, roc_curve, roc_auc_score

def vertexwise_correctness(pb_labeled, expected_dict):
  correctness = []
  for entry in pb_labeled.entries:
      expected_cluster = expected_dict[entry.vertex_id]
      actual_cluster = entry.cluster_id
      correct = False
      if expected_cluster.HasField('cluster_id_int'):
          if actual_cluster.HasField('cluster_id_int'):
              correct = (expected_cluster.cluster_id_int == actual_cluster.cluster_id_int)
      elif expected_cluster.HasField('cluster_id_str'):
          if actual_cluster.HasField('cluster_id_str'):
              correct = (expected_cluster.cluster_id_str == actual_cluster.cluster_id_str)
      correctness.append(correct)

  return correctness

def str_to_int(s):
  d = 0
  for c in s:
    d += ord(c)
  return d
    
def mean_auc(pb_labeled, expected_dict):
  y_true = []
  y_score = []

  for entry in pb_labeled.entries:
    expected_cluster = expected_dict[entry.vertex_id]
    actual_cluster = entry.cluster_id
    if expected_cluster.HasField('cluster_id_int'):
      y_true.append(expected_cluster.cluster_id_int)
      y_score.append(actual_cluster.cluster_id_int)
    elif expected_cluster.HasField('cluster_id_str'):
      y_true.append(str_to_int(expected_cluster.cluster_id_str))
      y_score.append(str_to_int(actual_cluster.cluster_id_str))

  return roc_auc_score(y_true, y_score)

def run_and_measure_time(command, cwd):
  try:
    result = subprocess.run(['../evaluate/run_and_measure_time.sh'] + command, cwd=cwd, check=True, stdout=subprocess.PIPE)
    real_time = float(result.stdout)

    return real_time
  except subprocess.CalledProcessError as e:
    print(f"Error: {e}")
    return None