import os
import mediacloud.api
from dotenv import load_dotenv

load_dotenv()  # load config from .env file

mc = mediacloud.api.MediaCloud(os.getenv('MC_API_KEY'))

COVID_QUERY = 'coronavirus OR covid OR "covid 19" OR "covid-19" OR covid19 OR pandemic'
US_NATIONAL = 34412234


def get_current_coverage():
    LAST_7_DAYS = "publish_date:[NOW-8DAY TO NOW-1DAY]"
    covid_stories = mc.storyCount("({}) AND tags_id_media:{}".format(COVID_QUERY, US_NATIONAL), LAST_7_DAYS)['count']
    total_stories = mc.storyCount('* AND tags_id_media:{}'.format(US_NATIONAL), LAST_7_DAYS)['count']
    current_value = covid_stories/total_stories
    return current_value


print(get_current_coverage())
