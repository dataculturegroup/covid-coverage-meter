import logging
from flask import Flask, make_response
import csv
import io

from helpers.request import api_method
import helpers.data

logger = logging.getLogger(__name__)

app = Flask(__name__)


@app.route("/covid.json", methods=['GET'])
@api_method
def data_as_json():
    return helpers.data.get_historical_coverage(False)


@app.route("/covid.csv", methods=['GET'])
def data_as_csv():
    data = helpers.data.get_historical_coverage(True)
    output = io.StringIO()
    writer = csv.DictWriter(output, fieldnames=['week', 'coverage'], delimiter=',')
    writer.writeheader()
    writer.writerows(data)
    response = make_response(output.getvalue(), 200)
    response.mimetype = "text/plain"
    return response


if __name__ == "__main__":
    app.debug = True
    app.run()
