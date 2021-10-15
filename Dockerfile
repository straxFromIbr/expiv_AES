FROM ubuntu:latest

WORKDIR /home

RUN apt update && \
    apt upgrade -y && \
    apt install gcc make -y

CMD /bin/bash -l
