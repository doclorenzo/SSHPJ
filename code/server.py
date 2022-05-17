import hug
import os
import datetime
from influxdb_client import InfluxDBClient

token = str(os.getenv('DOCK_IN_TOKEN'))
org = str(os.getenv('DOCK_IN_ORG'))
bucket = str(os.getenv('DOCK_IN_BUCKET'))

@hug.get("/",output=hug.output_format.html)
def index():
    with open('index.html') as myfile:
        html=myfile.read()
    return html

@hug.get("/realtime",output=hug.output_format.html)
def realtime():
    with open('realtime.html') as myfile:
        html=myfile.read()
    return html

@hug.get("/tomato")
def values(dataType:hug.types.text,agestart:hug.types.text,agestop:hug.types.text,yesy:hug.types.text='yield(name: "mean")'):
    s=""
    if not dataType:
        return s
    x=0
    d=dataType.split(".")
    datas=""
    while x < len(d)-1:
        if x==0:
            datas=' |> filter(fn: (r) => r["_field"] == "'+d[x]+'"'
        elif x!=0:
            datas+=' or r["_field"] == "'+d[x]+'"'
        x+=1
    datas+=")"

    with InfluxDBClient(url="http://172.28.5.1:8086", token=token, org=org) as client:
        query = 'from(bucket: "'+bucket+'") |> range(start: '+agestart+' , stop: '+agestop+') |> filter(fn: (r) => r["_measurement"] == "dokUniMeasures")'+ datas                
        query+= ' |> filter(fn: (r) => r["device"] == "ESP32") |> '+yesy
        try:
            tables = client.query_api().query(query, org=org)
        except:
           return s 
        for table in tables:
            for record in table.records:
                s+=record["_field"]+","+str(record["_value"])+","+str(record["_time"])+",  <br>  "
        return s
    
