import spell_corrector as sc

from nltk.corpus import stopwords
from pymystem3 import Mystem
from nltk.stem.snowball import SnowballStemmer
from collections import defaultdict


class StemmerRus:
    def __init__(self):
        self.__lem = Mystem()
        self.__spell_corrector = sc.SpellCorrectorRus()
        self.__stemmer = SnowballStemmer('russian')
        self.__russian_stopwords = stopwords.words('russian')
        self.__stem_dict = defaultdict(set)

    def __lemmatize(self, string):
        return self.__lem.lemmatize(
            self.__spell_corrector.correct(string)
        )[:-1][::2]

    def stem(self, text):
        for word in self.__spell_corrector.correct(text).split():
            if word not in self.__russian_stopwords and word is not ' ':
                self.__stem_dict[self.__stemmer.stem(word)].add(word)

    def save_results_to_file(self, filename):
        with open(filename, 'w') as f:
            for k, v in sorted(self.__stem_dict.items()):
                f.write(k + ': ' + ', '.join(v) + '\n')
