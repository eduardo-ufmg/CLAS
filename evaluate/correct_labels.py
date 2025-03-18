def correct_labels(labels, idtype):
  if idtype == "int":
    label_set = [-1, 1]
  elif idtype == "str":
    label_set = ["A", "B"]
  else:
    raise ValueError("Invalid cluster ID type")
  
  return [label_set[label] for label in labels]
