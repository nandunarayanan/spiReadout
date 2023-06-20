FROM hypriot/rpi-node:latest
MAINTAINER Nandu Narayanan <nanduremo@gmail.com>
#get the source code from WiringPi github
RUN git clone https://github.com/WiringPi/WiringPi.git
#get the source code from spiReadout nandu's github
RUN git clone https://github.com/nandunarayanan/spiReadout.git
#build the WiringPi inside docker image
RUN cd WiringPi && ./build
#build the spiReadout program inside docker image
RUN cd spiReadout && g++ spiReadout.c -l wiringPi -o output
#set the working directory as final spiReadout executable directory
WORKDIR /spiReadout
#Run it !!!!
CMD ["./output"]

#Docker build instruction for this Dockerfile is as below;
#########################################################
#     docker build -t <image-name> <Dockerfile-path>    #
#     ex: docker build -t mywire .                      #     
#########################################################

#Docker run instruction (for image created above) is as below;
#######################################################################################################################
#     docker run --device /dev/ttyAMA0:/dev/ttyAMA0 --device /dev/mem:/dev/mem --privileged --net=host -ti mywire     #
#     ex: docker run --device /dev/ttyAMA0:/dev/ttyAMA0 --device /dev/mem:/dev/mem --privileged --net=host -ti mywire #     
#######################################################################################################################
