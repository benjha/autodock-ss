*************************************************************************************
In-situ infraestructure for Autodock-GPU
*************************************************************************************

Overview
========

To supplement traditional  MD  data  analyses  that  follow  a  batch-based  approach  (i.e.,generate data, then perform data analysis and visualization post hoc), this project enables software infrastructure for in-situ processing (e.g., molecular visualization, dimensionality reduction). In-situ capabilities allows to analyze data as it is being produced and save to disk only what is really needed for  further  analysis, thus having  the  potential  to  reduce  I/O  bandwidth  and  storage  needs.  

This in-situ infrastructure is build on top of Autodock-GPU and Cray's Smartsim as a use case and provides communication and coordination mechanisms for data interchange between the different modules of this project and enables collaborative analysis via Jupyter notebooks.

Figure 

.. image:: imgs/generalDiagram.png
   :align: center
   :alt: In-situ infraestructure for Autodock-GPU

For a detailed description of Autodock-GPU and Cray's Smartsim refer to `Autodock-GPU repository <https://github.com/ccsb-scripps/AutoDock-GPU>`_ and `Smartsim documentation <https://www.craylabs.org/docs/overview.html>`_

