'''
This module treats all possible geographic locations as a vector space
spanned by a canonical basis for latitude and longitude. To measure displacement,
we perform a linear transformation on geographic locations by converting from
geographic coordinates to measurement in feet.

We define our canonical basis as those defined in feet:
let e_0 = (1, 0) denote one foot north and e_1 = (0, 1) denote one foot east.
'''
import math

class Displacement:
    def __init__(self, center, threshold=50):
        self._center = center
        self._threshold = threshold
    
    @property
    def center(self):
        return self._center
    
    @center.setter
    def center(self, center):
        # center = self.map_to_feet(center)
        self._center = center
    
    '''
    Number of feet to serve as a threshold from a center location before beginning
    a ReST API call to inform owner of potentially-escaped pet
    '''
    @property
    def threshold(self):
        return self._threshold
    
    @threshold.setter
    def threshold(self, threshold):
        self._threshold = threshold
    
    '''
    Units in latitude and longitude have some degree of variation as specified
    here: https://www.usgs.gov/faqs/how-much-distance-does-a-degree-minute-and-second-cover-your-maps?qt-news_science_products=0#
    Specifically, for each degree of latitude, there are approximately 364000 feet.
    For each degree of longitude, there are approximately 288,200 feet.

    @param location dictionary object following the schema of the "location" object
    in ./Server/mock_data/geographical_data.json. Used to represent the location of
    the ESP8266 ESP-01S module.
    @returns location converted from geographic coordinates to feet
    '''
    def map_to_feet(self, location):
        return (location[0] * 364000, location[1] * 288200)
    
    '''
    Get the two norm (in feet) of the given location and the given center.
    @param location tuple containing the target location with respect to
    our canonical basis.
    @returns The radial distance in feet of the given locations.
    '''
    def get_radial_distance(self, location):
        mapped_center = self.map_to_feet(self._center)
        delta = (mapped_center[0] - location[0], mapped_center[1] - location[1])
        return math.hypot(*delta)

    '''
    Determine whether the given location is beyond some distance threshold
    @param location tuple containing the target location with respect to
    geographic oordinates
    @returns boolean determining whether the given location is beyond our
    distance threshold
    '''
    def beyond_threshold(self, location):
        mapped_location = self.map_to_feet(location)
        return (self.get_radial_distance(mapped_location) > self._threshold)

