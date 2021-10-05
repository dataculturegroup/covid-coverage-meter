import unittest
import json

import helpers.data


class TestData(unittest.TestCase):

    def test_data_full(self):
        results = helpers.data.get_historical_coverage(False)
        assert len(results) > 0
        for row in results:
            assert 'rolling_rate' in row
            assert 'date' in row
            assert 'covid' in row
            assert 'total' in row

    def test_data_simple(self):
        results = helpers.data.get_historical_coverage(True)
        assert len(results) > 0
        for row in results:
            assert 'week' in row
            assert 'coverage' in row


if __name__ == "__main__":
    unittest.main()
