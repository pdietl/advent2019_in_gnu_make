FROM alpine:latest

RUN apk update && apk upgrade && apk add \
    alpine-sdk \
    bash \
    findutils \
    make \
