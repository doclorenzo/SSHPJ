import hug
import datetime
from influxdb_client import InfluxDBClient

token = "gQbaJqp00M7Uu-cnLva175b3QWiTGCXRiYipov-lv1cJp-sZZr5dBq_18nbWtAjNjPh3I1FffWw_-jPpNS8YxA=="
org = "a"
bucket = "a"

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
def values(agestart:hug.types.text,agestop:hug.types.text,yesy:hug.types.text='yield(name: "mean")'):
    s=""
    with InfluxDBClient(url="http://127.0.0.1:8087", token=token, org=org) as client:
        query = 'from(bucket: "a") \
                    |> range(start: '+agestart+' , stop: '+agestop+') \
                    |> filter(fn: (r) => r["_measurement"] == "CO2MIS") \
                    |> filter(fn: (r) => r["SSID"] == "Mannosproz") \
                    |> filter(fn: (r) => r["_field"] == "CO2") \
                    |> filter(fn: (r) => r["device"] == "ESP32") \
                    |> aggregateWindow(every: 10s, fn: mean, createEmpty: false) \
                    |> '+yesy
        try:
            tables = client.query_api().query(query, org=org)
        except:
           return s 
        for table in tables:
            for record in table.records:
                s+=record["_field"]+","+str(record["_value"])+","+str(record["_time"])+",  <br>  "
        return s
    
