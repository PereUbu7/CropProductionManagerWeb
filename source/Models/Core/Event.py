#!/usr/bin/env python

""" Event data model
"""

import datetime

class Event:
    def __init__(self, id: int, name: str, _datetime: datetime.datetime):
        self.Id = id
        self.__name = name
        self.Datetime = _datetime
    
    @property
    def Name(self) -> str:
        return self.__name

class SeedingEvent(Event):
    def __init__(self, id: int, datetime: datetime.datetime):
        super().__init__(id, 'seeding', datetime)

class PlantingEvent(Event):
    def __init__(self, id: int, datetime: datetime.datetime):
        super().__init__(id, 'planting', datetime)

class RepottingEvent(Event):
    def __init__(self, id: int, datetime: datetime.datetime):
        super().__init__(id, 'repotting', datetime)

class HarvestStartEvent(Event):
    def __init__(self, id: int, datetime: datetime.datetime):
        super().__init__(id, 'harvestStart', datetime)

class HarvestEndEvent(Event):
    def __init__(self, id: int, datetime: datetime.datetime):
        super().__init__(id, 'harvestEnd', datetime)