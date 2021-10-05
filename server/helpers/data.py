import datetime as dt
import pandas as pd
from typing import List, Dict

from helpers import mc

COVID_QUERY = 'coronavirus OR covid OR "covid 19" OR "covid-19" OR covid19 OR pandemic'
DATE_RANGE = mc.dates_as_query_clause(dt.date(2020, 2, 1), dt.date.today())
US_TOP_2018_COLLECTION = 186572516
US_NATIONAL = 34412234


def get_historical_coverage(simplify: bool = False) -> List[Dict]:
    # get the raw data by week
    covid_stories = mc.storyCount("({}) AND tags_id_media:{}".format(COVID_QUERY, US_NATIONAL), DATE_RANGE,
                                  split=True, split_period='week')['counts']
    total_stories = mc.storyCount('* AND tags_id_media:{}'.format(US_NATIONAL), DATE_RANGE,
                                  split=True, split_period='week')['counts']
    # put it together by week
    for total_item in total_stories:
        total_item['covid_count'] = 0
        for covid_item in covid_stories:
            if total_item['date'] == covid_item['date']:
                total_item['covid_count'] = covid_item['count']
    # add rolling average
    data = []
    for item in total_stories:
        data.append(dict(date=item['date'], total=item['count'], covid=item['covid_count']))
    df = pd.DataFrame(data)
    df['rate'] = df['covid'] / df['total']
    df['rolling_rate'] = df.rolling(window=7)['rate'].mean()
    if simplify:
        # flatten it a bit for Aruduino consumption
        return [dict(week=index, coverage=row['rate']) for index, row in df.iterrows()]
    else:
        return df.to_dict('records')
