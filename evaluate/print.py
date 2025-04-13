from google.protobuf.message import DecodeError
from classifier_pb2 import TrainingDataset, SupportVertices, Hyperplanes, VerticesToLabel, LabeledVertices, chipIDmap
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
  parser.add_argument("--data", choices=["dataset", "sv", "expt", "test", "labeled", "chipidbimap"], help="data to print")
  parser.add_argument("--classifier", choices=["chip", "rchip", "nn"], help="classifier type")
  parser.add_argument("--setname", help="dataset name")
  args = parser.parse_args()

  base_path = pathlib.Path(__file__).parent

  data_path = base_path / "../data/"
  bin_path = base_path / "../bin/"
  trained_path = bin_path / "train/"
  labeled_path = bin_path / "label/"

  data = {
    "dataset": (data_path / args.setname / args.setname, TrainingDataset),
    "sv": (trained_path / f"{args.classifier}-{args.setname}", SupportVertices),
    "expt": (trained_path / f"{args.classifier}-{args.setname}", Hyperplanes),
    "test": (data_path / args.setname / "test", VerticesToLabel),
    "labeled": (labeled_path / f"{args.classifier}-{args.setname}", LabeledVertices),
    "chipidbimap": (trained_path / f"chipidbimap-{args.setname}", chipIDmap)
  }

  print_dataset(*data[args.data])
