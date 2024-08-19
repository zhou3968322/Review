ARG BASE_IMAGE=nvidia/cuda:12.2.0-devel-ubuntu20.04
FROM ${BASE_IMAGE} as build-base
ENV LC_ALL=C
# RUN rm /etc/apt/sources.list.d/cuda-ubuntu2204-x86_64.list
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install --no-install-recommends -y ca-certificates g++ \
    python3.8-dev python3-distutils python3-venv \
    curl git build-essential cmake gcc openssh-server unzip wget vim libssh-dev lld \
    && apt-get clean && rm -rf /var/lib/apt/lists/* \
    && cd /tmp \
    && curl -O https://bootstrap.pypa.io/get-pip.py \
    && python3 get-pip.py && rm get-pip.py


RUN update-alternatives --install /usr/bin/python python /usr/bin/python3 1 \
# && update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.8 1\
&& update-alternatives --install /usr/bin/python3m python3m /usr/bin/python3 1 \
&& update-alternatives --install /usr/local/bin/pip pip /usr/local/bin/pip3 1

RUN python -m venv /home/venv
RUN pip install --no-cache CppHeaderParser && pip install --no-cache sympy pyelftools
RUN mkdir /installs && cd /installs \
    && wget https://github.com/Kitware/CMake/releases/download/v3.26.4/cmake-3.26.4.tar.gz && \
    tar zxvf cmake-3.26.4.tar.gz && cd cmake-3.26.4 && \
    cmake . && make -j 6 && make install && \
    rm -rf /installs

FROM build-base as dev

RUN mkdir -p /var/run/sshd
RUN echo 'root:root' | chpasswd
RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config
RUN echo 'PermitRootLogin yes' >> /etc/ssh/sshd_config

RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd
ENV NOTVISIBLE="in users profile"
RUN echo "export VISIBLE=now" >> /etc/profile
EXPOSE 22
ENV TZ=Asia/Shanghai

CMD /usr/sbin/sshd -D
