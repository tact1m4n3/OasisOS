FROM debian:latest

RUN apt-get update 
RUN apt-get upgrade -y
RUN apt-get install -y build-essential nasm python3
RUN apt-get install -y xorriso grub-common grub-pc-bin

VOLUME /env
WORKDIR /env
