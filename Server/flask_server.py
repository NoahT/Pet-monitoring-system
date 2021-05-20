from flask import Flask, request, redirect
import os
from twilio.rest import Client

from displacement import Displacement
import json

LATITUDE = -1 # latitude of house location
LONGITUDE = -1
THRESHOLD = 50 # amount of feet from house location before sending SMS
app = Flask(__name__)
esp_displacement = Displacement((LATITUDE, LONGITUDE), THRESHOLD)
# account_sid = os.environ['TWILIO_ACCOUNT_SID']
# auth_token = os.environ['TWILIO_AUTH_TOKEN']
client = Client('TWILIO_ACCOUNT_SID', 'TWILIO_AUTH_TOKEN')

@app.route("/arduino", methods=['GET', 'POST'])
def arduino():
    location = request.get_json()["location"]
    location = (location["lat"], location["lng"])
    payload = {}
    print(location)
    if esp_displacement.beyond_threshold(location):
        # message = client.messages.create(
        #                       body='Hello! Just informing you that your pet potentially escaped.',
        #                       from_='+17603345880',
        #                       to='+PERSONAL NUMBER')
        print("Escape!")
        payload['escape'] = True
    else:
        print("No escape!")
        payload['escape'] = False
    return json.dumps(payload)

@app.route("/")
def init():
    return ""

if __name__ == "flask_server":
    app.run(debug=True)
