import numpy as np
import matplotlib.pyplot as plt

from smartredis import Client
from smartsim import Experiment
from smartsim.database import Orchestrator
from smartsim.settings import RunSettings


exp = Experiment("autodock", launcher="local")
db = Orchestrator(port=6780)

settings = RunSettings(exe="bash", exe_args="launch_autodock.sh")

docking = exp.create_model("docking", settings)
# tell exp.generate to include this file in the created run directory
docking.attach_generator_files(to_copy="launch_autodock.sh")

# generate directories for output, error and results
exp.generate(db, docking, overwrite=True)
# start the database and connect client to get data
exp.start(db)
client = Client(address="127.0.0.1:6780", cluster=False)

# start simulation without blocking so data can be analyzed in real time
exp.start(docking, block=False, summary=True)

# poll until data is available
client.poll_key("1stp_ligand.pdbqt_testing", 100, 100)

print("Dataset written")
dataset=client.get_dataset("1stp_ligand.pdbqt_testing")
print(dataset)