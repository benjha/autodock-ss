
import numpy as np
from smartredis import Client

client = Client(address="127.0.0.1:6780", cluster=False)

# poll until data is available
client.poll_key("1stp_ligand.pdbqt_testing", 100, 100)

print("Dataset written")
dataset=client.get_dataset("1stp_ligand.pdbqt_testing")
print(dataset)

inputLigand = dataset.get_tensor("input_ligand")

print(inputLigand)

print ("".join([chr(char) for char in inputLigand]))
