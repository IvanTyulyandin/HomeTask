import re
import csv

from langdetect.lang_detect_exception import LangDetectException
from langdetect import detect


def get_data(filename):
    with open(filename, 'r', newline='') as f:
        data = csv.reader(f, delimiter='\t')

        for line in data:
            try:
                text = line[3].strip()
            except IndexError:
                # some of lines are not csv
                continue
            try:
                if detect(text) == 'ru':
                    yield re.sub(r'(.)(\1){2,}', r'\1',
                                 re.sub(r'[ ]{2,}', ' ', re.sub(r'[^а-яё\s]', ' ', text.lower())))
            except LangDetectException as e:
                # language is not detectable'
                pass
