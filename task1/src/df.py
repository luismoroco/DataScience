import pandas as pd
from typing import Tuple
import logging as log 
from error import NOT_PATH, NOT_COL

class DataFrameManager: 
    _df: pd.DataFrame
    _shape: Tuple[int, int] 

    def __init__(self, filePath: str) -> None:
      try:
        self._df = pd.read_csv(filePath)
        self._shape = self._df.shape
        self.cleanDataFrame()
      except:
        self._df = None
        self._shape = None
        log.error(NOT_PATH)
    
    def cleanDataFrame(self) -> None:
      self._df.drop_duplicates()

    def getDataFrame(self) -> pd.DataFrame:
      return self._df

    def getShape(self) -> Tuple[int, int]:
      return self._shape

    def sortUsingColumn(self, name: str) -> None:
      if name in self._df.columns:
        self._df.sort_values(by=[name])
        return 
      log.error(NOT_COL)
    
    def getInfo(self) -> None:
      print(self._df.info())
             


