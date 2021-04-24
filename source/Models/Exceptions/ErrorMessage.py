#!/usr/bin/env python

""" Error Messages
"""
from Models.Exceptions.Message import Message

NumberOfPlantsNotLowerThanZero = Message(swedish = "Plantantal kan inte vara mindre än 0", english = "Number of plants cannot be lower than 0")
BatchNumberNotLowerThanZero = Message(swedish = "Omgång kan inte vara mindre än 1", english = "Batch cannot be lower than 1")
GerminationRateOutsideOfRange = Message(swedish = "Grobarhet måste vara mellan 0 och 1", english = "Germination rate must be between 0 and 1")