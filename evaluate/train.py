import argparse
import pathlib
import subprocess

def main():
  parser = argparse.ArgumentParser("Train a classifier")
  parser.add_argument("--dataset", required=True, help="Path to the dataset")
  parser.add_argument("--classifier", required=True, choices=["chip", "rchip", "nn"], help="Classifier to train")
  parser.add_argument("--tolerance", default=1.0, help="Filter tolerance")
  args = parser.parse_args()

  

if __name__ == "__main__":
  main()
