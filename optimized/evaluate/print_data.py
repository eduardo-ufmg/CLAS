from google.protobuf.message import DecodeError
from classifier_pb2 import TrainingDataset, SupportVertices, Experts, VerticesToLabel, LabeledVertices
import argparse
import pathlib

def print_dataset(file_path, messageType):
  message = messageType()
  try:
    with open(file_path, "rb") as f:
      message.ParseFromString(f.read())
    print(message)
  except FileNotFoundError:
    print(f"Error: File {file_path} not found.")
  except DecodeError:
    print(f"Error: Failed to decode the file {file_path}.")
  except Exception as e:
    print(f"An unexpected error occurred: {e}")

if __name__ == "__main__":
  parser = argparse.ArgumentParser(description="Print data from protobuf files")
  parser.add_argument("--data", choices=["dataset", "sv", "expt", "tolabel", "labeled"], help="data to print")
  parser.add_argument("--classifier", choices=["chip", "rchip", "nn"], default="nn", help="classifier type")
  parser.add_argument("--setname", default="synthetic", help="dataset name")
  args = parser.parse_args()

  base_path = pathlib.Path(__file__).parent

  labeledpath = {
    "chip": base_path / "../chip/chip-clas/",
    "rchip": base_path / "../chip/rchip-clas/",
    "nn": base_path / "../nn/"
  }

  data = {
    "dataset": (base_path / f"../data/{args.setname}/{args.setname}", TrainingDataset),
    "sv": (base_path / "../nn/supportvertices", SupportVertices),
    "expt": (base_path / "../chip/experts", Experts),
    "tolabel": (base_path / f"../data/{args.setname}/tolabel", VerticesToLabel),
    "labeled": (labeledpath[args.classifier] / "labeled", LabeledVertices)
  }

  print_dataset(*data[args.data])
