****************************************
In-situ infraestructure for Autodock-GPU
****************************************

Overview
========

Autodock-SS is an ongoing effort for in-situ processing (e.g., molecular visualization, dimensionality reduction) of MD simulations. In-situ capabilities allow analyzing data on the fly and save to disk only what is needed for further post-processing; thus, having the potential to reduce I/O bandwidth and storage needs.

Autodock-SS is built on top of Autodock-GPU and Cray's Smartsim. It provides communication and coordination mechanisms for data interchange between the different modules (implemented in C/C++ and python) of this project.

For a detailed description of Autodock-GPU and Cray's Smartsim refer to `Autodock-GPU repository <https://github.com/ccsb-scripps/AutoDock-GPU>`_ and `Smartsim documentation <https://www.craylabs.org/docs/overview.html>`_

The following diagram provides an overview of Smart Autodock modules. The general design follows a producer-consumer architecture, where the producer is a running simulation (Autodock-GPU) and consumers are analysis codes. Alternatively, Autodock-SS enables collaborative analysis of the running simulation via Project Jupyter. The core technology for data managing and sharing is based on the RedisAI database while SmartSim provides orchestration and communication services between simulation and analysis.  

.. image:: imgs/generalDiagram.png
   :align: center
   :alt: In-situ infraestructure for Autodock-GPU

Autodock-GPU has been extended with the ``cSmartRedisAdaptor`` class to support connecting, sending metadata, and docking steps to the RedisAI database.  ``analysis.py`` is a python script that connects to the RedisAI database to access simulation data for analysis and visualization. Finally, ``driver.py`` controls execution and orchestration. 

Data Sharing Strategy
=====================

TODO: Explains how Autodock-GPU handles data, the  ``cSmartRedisAdaptor`` and ``analysis.py``

Execution
=========

TODO: Explains ``driver.py``

SUMMIT
======

TODO: Explains LSF scripts for the clients, modules and Jupyter 
