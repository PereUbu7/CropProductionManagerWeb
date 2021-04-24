#!/usr/bin/env python

""" Crop data model
"""
from typing import List
import Models.Core.Event as Event
from Models.Exceptions.Exceptions import ArgumentException
import Models.Exceptions.ErrorMessage as ErrorMessage

class Crop:
    def __init__(self):
        self.Id: int = 0
        self.Name: str = ""
        self.Variety: str = ""
        self.Family: str = ""
        self.Events: list[Event.Event] = []
        self.NumberOfPlants: int = 0
        self.NumberOfSeeds: int = 0
        self.__batchNumber: int = 0
        self.__germinationRate: float = 1
        self.PlantDistance: float = 0 
        self.RowMeters: int = 0
        self.DirectSeeding: bool = False
        self.OverWinter: bool = False
        self.UseRowCover: bool = False
    
    @property
    def BatchNumber(self) -> int:
        return self.__batchNumber
    @BatchNumber.setter
    def BatchNumber(self, value: int):
        if value < 0:
            raise ArgumentException(ErrorMessage.BatchNumberNotLowerThanZero)
        self.__batchNumber = value
    
    @property
    def GerminationRate(self) -> int:
        return self.__germinationRate
    @GerminationRate.setter
    def GerminationRate(self, value: int):
        if value < 0 or value > 1:
            raise ArgumentException(ErrorMessage.GerminationRateOutsideOfRange)
        self.__germinationRate = value

    @property
    def NumberOfPlants(self) -> int:
        return self.__numberOfPlants
    @NumberOfPlants.setter
    def NumberOfPlants(self, value: int) -> None:
        if value < 0:
            raise ArgumentException(ErrorMessage.NumberOfPlantsNotLowerThanZero)
        self.__numberOfPlants = value

    def AddEvent(self, event: Event.Event) -> None:
        pass
