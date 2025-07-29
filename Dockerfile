FROM ubuntu:23.10

LABEL maintainer="deine.email@example.com"

ENV PS2DEV=/usr/local/ps2dev
ENV PS2SDK=$PS2DEV/ps2sdk
ENV GSKIT=$PS2DEV/gsKit
ENV PATH=$PATH:$PS2DEV/bin:$PS2DEV/ee/bin:$PS2DEV/iop/bin:$PS2DEV/dvp/bin:$PS2SDK/bin

# Abhängigkeiten installieren

RUN sed -i 's|http://archive.ubuntu.com/ubuntu|http://old-releases.ubuntu.com/ubuntu|g' /etc/apt/sources.list && \
    sed -i 's|http://security.ubuntu.com/ubuntu|http://old-releases.ubuntu.com/ubuntu|g' /etc/apt/sources.list


RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    texinfo \
    flex \
    bison \
    gettext \
    libgmp-dev \
    libmpfr-dev \
    libmpc-dev \
    zlib1g-dev \
    bash \
    curl \
    tar \
    xz-utils \
    python3 \
    python3-pip \
    libncurses-dev \
    libtool \
    autoconf \
    automake && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

# PS2DEV-Verzeichnis erstellen
RUN mkdir -p $PS2DEV

# Toolchain aus dem Release laden und entpacken
RUN curl -L -o ps2dev-latest.tar.gz https://github.com/ps2dev/ps2dev/releases/download/latest/ps2dev-ubuntu-latest.tar.gz && \
    tar -xzf ps2dev-latest.tar.gz -C "$PS2DEV" --strip-components=1 && \
    rm ps2dev-latest.tar.gz

CMD ["/bin/bash"]
