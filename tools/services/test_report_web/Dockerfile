FROM node:latest

WORKDIR /app

COPY package*.json /app/

ENV http_proxy 'http://10.244.0.55:83/'
ENV HTTP_PROXY 'http://10.244.0.55:83/'
ENV https_proxy 'https://10.244.0.55:83/'
ENV HTTPS_PROXY 'https://10.244.0.55:83/'
ENV NO_PROXY '.got.volvocars.net'


RUN npm config set proxy $HTTP_PROXY
RUN npm config set proxy-https $HTTPS_PROXY

RUN npm install 

COPY . .

CMD [ "./node_modules/.bin/nodemon", "./server.js" ]

