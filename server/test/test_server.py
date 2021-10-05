import unittest
import json

from server import app


class TestServer(unittest.TestCase):

    def setUp(self) -> None:
        # This initializes the base client used by all the other tests
        app.config['TESTING'] = True
        self._client = app.test_client()

    def test_data_as_json(self):
        response = self._client.get('/covid.json')
        data = json.loads(response.data)
        assert len(data) > 0
        assert 'date' in data[0]

    def test_data_as_csv(self):
        response = self._client.get('/covid.csv')
        data = response.text
        assert len(data) > 0
        assert 'week' in data


if __name__ == "__main__":
    unittest.main()
