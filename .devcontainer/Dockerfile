# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.194.3/containers/ubuntu/.devcontainer/base.Dockerfile

# [Choice] Ubuntu version: hirsute, bionic, focal
ARG VARIANT="bionic"
FROM mcr.microsoft.com/vscode/devcontainers/base:0-${VARIANT}

# [Optional] Uncomment this section to install additional OS packages.
 RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
     && apt-get -y install --no-install-recommends \
     build-essential \
     pkg-config \
     curl \
     git \
     wget \
     libbotan-2-dev \
     qt5-default \
     libboost-serialization-dev \
     libboost-test-dev \
     libboost-filesystem-dev \
     libssl-dev \
     rustc \
     rust-src \
     cargo 

 RUN cd /opt && mkdir cmake && cd cmake \
    && wget https://github.com/Kitware/CMake/releases/download/v3.22.2/cmake-3.22.2-linux-x86_64.sh \
    && chmod +x cmake-3.22.2-linux-x86_64.sh \
    && sudo sh cmake-3.22.2-linux-x86_64.sh --prefix=/usr/local/ --exclude-subdir --skip-license

