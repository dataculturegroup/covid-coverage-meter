import os
import sys
from dotenv import load_dotenv
import sentry_sdk
from sentry_sdk.integrations.flask import FlaskIntegration
import mediacloud.api
import logging

VERSION = '0.1.0'

logging.basicConfig(level=logging.INFO,
                    format="[%(asctime)s][%(levelname)s] %(name)s %(filename)s:%(funcName)s:%(lineno)d | %(message)s")
logger = logging.getLogger(__name__)
logger.info("---------------------------------------------------------------------------")

load_dotenv() # load in config from local file or environment variables

MC_API_KEY = os.environ.get('MC_API_KEY', None)
if MC_API_KEY:
    mc = mediacloud.api.MediaCloud(MC_API_KEY)
    logger.info("  MC_API_KEY: {}".format(MC_API_KEY))
else:
    logger.info("No MC api key found - bailing")
    sys.exit()


SENTRY_DSN = os.environ.get('SENTRY_DSN', None)  # optional centralized logging to Sentry
if SENTRY_DSN:
    sentry_sdk.init(dsn=SENTRY_DSN, release=VERSION,
                    integrations=[FlaskIntegration()])
    logger.info("  SENTRY_DSN: {}".format(SENTRY_DSN))
else:
    logger.info("Not logging errors to Sentry")
