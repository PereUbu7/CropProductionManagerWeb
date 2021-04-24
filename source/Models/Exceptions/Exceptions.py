#!/usr/bin/env python

""" Crop production manager exceptions
"""
from Models.Exceptions.Message import Message
import Models.Globals as Globals

class ArgumentException(Exception):
    def __init__(self, message: Message) -> None:
        print(message)
        messageStr = { # Se till så att vi får ut värdet på enumen här
            'swedish' : message.Swedish,
            'english' : message.English,
        }[Globals.Language]

        super().__init__(messageStr)