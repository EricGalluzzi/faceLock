FROM python:3.9
WORKDIR /cam

COPY requirements.txt requirements.txt
RUN apt-get update
RUN apt-get install ffmpeg libsm6 libxext6  -y
RUN pip3 install -U wheel cmake
RUN pip3 install -r requirements.txt
EXPOSE 8000

COPY . .
CMD ["python3", "./socketServer.py"]