FROM nvidia/cuda:10.2-devel-ubuntu18.04
SHELL ["/bin/bash", "-c"]

WORKDIR /root
COPY .bashrc .

RUN apt update -y
RUN apt install vim -y
RUN apt install wget -y 
RUN apt install cmake -y
RUN apt install glibc-doc -y
RUN apt install libgmp3-dev -y
RUN apt install libmpfr-dev -y

# BLAS LAPACK
RUN apt install libblas-dev -y
RUN apt install liblapack-dev -y

RUN cp /usr/lib/x86_64-linux-gnu/libcublas.so /usr/local/cuda/lib64/

# Install Git
RUN apt install git -y

WORKDIR /data
RUN git clone https://github.com/DrTimothyAldenDavis/SuiteSparse.git

WORKDIR /data/SuiteSparse/SuiteSparse_config
COPY SuiteSparse_config.mk .

WORKDIR /data/SuiteSparse/SPQR/Demo
COPY A.mtx .
COPY b.mtx .
COPY Makefile_copy .
COPY qrdemo_gpi.cpp .

# Install Python 3.7
RUN apt install python3.7 -y
RUN apt install python3-distutils -y
RUN apt install libgl1-mesa-glx -y
RUN apt install libglib2.0-0 -y

# Install Pip
RUN apt install curl -y
RUN curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
RUN python3.7  get-pip.py
RUN rm get-pip.py -f

# Install Python Library
RUN pip install scipy
RUN pip install opencv-python
RUN pip install sklearn

# Install TBB
RUN wget https://apt.repos.intel.com/intel-gpg-keys/GPG-PUB-KEY-INTEL-SW-PRODUCTS-2019.PUB
RUN apt-key add GPG-PUB-KEY-INTEL-SW-PRODUCTS-2019.PUB
RUN rm GPG-PUB-KEY-INTEL-SW-PRODUCTS-2019.PUB -f
RUN sh -c 'echo deb https://apt.repos.intel.com/tbb all main > /etc/apt/sources.list.d/intel-tbb.list'
RUN apt update -y
RUN apt install intel-tbb-2020.0-088 -y

WORKDIR /data/SuiteSparse