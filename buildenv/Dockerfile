FROM ubuntu:20.04

RUN apt-get update 
RUN apt-get upgrade -y
RUN apt-get install -y build-essential gnu-efi nasm python3
RUN apt-get install -y xorriso mtools dosfstools

VOLUME /env
WORKDIR /env
