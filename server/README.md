COVID Coverage Server
=====================

Return data about the rate of COVID coverage in US newsm, for consumption by a device. 


Developing
----------

### Installation

```
pip install -r requirements.txt
```

### Running Locally

Run locally with gunicorn: `./run.sh`

### Testing

Just run *pytest* to run a small set of test on the API endpoints.


Usage
-----

### API Endpoints

`http://localhost/covid.json`

`http://localhost/covid.csv`


Deploying
---------

This is built to deploy to a containerized hosting service like Heroku or dokku. Just push it and it should
build and just do the right thing. Use those PaaS tools to scale horizontally.
