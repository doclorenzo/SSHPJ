FROM python:3.8-slim-buster

WORKDIR /app
COPY requirements.txt requirements.txt
COPY . .
RUN python3 -m venv /app
RUN /bin/bash -c "source /app/bin/activate"
RUN python3 -m pip install -r requirements.txt
CMD hug -f server.py
